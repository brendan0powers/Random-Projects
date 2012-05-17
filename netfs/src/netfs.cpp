/***************************************************************************
 *   Copyright (C) 2008 by Brendan Powers   *
 *   brendan@resara.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "netfs.h"
#include "rpcthread.h"
#include "nullfs.h"
#include "symlinkfs.h"
#include "badfs.h"
#include "smbfs.h"

#include <QDebug>
#include <errno.h>


NetFs::NetFs(QObject *parent)
		: QObject(parent)
{
	RpcThread *rpc = new RpcThread(this,this);
	rpc->start();
}


NetFs::~NetFs()
{
}


ReturnValue NetFs::map(QString name, QString type, QString src, QStringList options)
{
	lock.lockForWrite();
	if(_maps.contains(name)) {lock.unlock(); return(ReturnValue(1,"The map " + name + " already exists."));}
	
	BaseFs *fs = NULL;
	
	if(type == "null") fs = new NullFs(name,this);
	else if(type == "symlink") fs = new SymlinkFs(name,this);
	else if(type == "bad") fs = new BadFs(name,this);
	else if(type == "smb") fs = new SmbFs(name,NULL);
	
	if(fs == NULL) {lock.unlock(); return(ReturnValue(2,"Uknown fs type " + type));}
	
	ReturnValue ret = fs->map(src,options);
	if(ret.isError()) {lock.unlock(); return(ret);}
	
	_maps[name] = fs;
	
	lock.unlock();
	
	return(ret);
}



ReturnValue NetFs::unMap(QString name)
{
	lock.lockForWrite();
	if(!_maps.contains(name)) {lock.unlock(); return(ReturnValue(1,"No map named " + name + " found"));}
	
	ReturnValue ret = _maps[name]->unMap();
	if(ret.isError()) {lock.unlock(); return(ret);}
		
	BaseFs *fs = _maps[name];
	_maps.remove(name);
	
	delete fs;
	
	lock.unlock();
	
	return(ret);
}

BaseFs *NetFs::findFs(QString path, QString &name, QString &newpath)
{
	lock.lockForRead();
	
	newpath = path.remove(0,1);
	int index = newpath.indexOf('/');
	name = newpath;
	
	if(index == -1) 
	{
		newpath = "/";
	}
	else
	{
		name.truncate(index);
		newpath = newpath.remove(0,index);
	}
	
	if(!_maps.contains(name))
	{
		lock.unlock();
		return(NULL);
	}
	
	BaseFs *ret = _maps[name];
	
	lock.unlock();
	
	return(ret);
}

int NetFs::fs_getattr(const char *path, struct stat *stbuf)
{
	if(QString(path) == "/")
	{
		stbuf->st_mode = S_IFDIR | 0644;
		stbuf->st_nlink = 1;
		stbuf->st_uid = getuid();
		stbuf->st_gid = getgid();
		stbuf->st_atime = stbuf->st_mtime = stbuf->st_ctime = time(NULL);
		
		return(0);
	}
	
	qDebug() << "Path" << path;
	QString newpath, name;
	BaseFs *fs = findFs(path,name,newpath);
	if(fs == NULL) return(-ENOENT);
	
	return(fs->fs_getattr(qPrintable(newpath),stbuf));
}

int NetFs::fs_opendir(const char *path, struct fuse_file_info *fi)
{
	if(QString(path) == "/") return(0);
	
	QString newpath, name;
	BaseFs *fs = findFs(path,name,newpath);
	if(fs == NULL) return(-ENOENT);
	
	return(fs->fs_opendir(qPrintable(newpath),fi));	
}

int NetFs::fs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t off, struct fuse_file_info *info)
{
	if(QString(path) == "/")
	{
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		
		foreach(QString name, _maps.keys())
		{
			filler(buf,qPrintable(name),NULL,0);
		}
		
		return(0);
	}

	QString newpath, name;
	BaseFs *fs = findFs(path,name,newpath);
	if(fs == NULL) return(-ENOENT);
	
	return(fs->fs_readdir(qPrintable(newpath),buf,filler,off,info));
}

int NetFs::fs_releasedir(const char *path, struct fuse_file_info *fi)
{
	if(QString(path) == "/") return(0);
	
	QString newpath, name;
	BaseFs *fs = findFs(path,name,newpath);
	if(fs == NULL) return(-ENOENT);
	
	return(fs->fs_releasedir(qPrintable(newpath),fi));
}

int NetFs::fs_readlink(const char *path, char *buf, size_t size)
{
	if(QString(path) == "/") return(-ENOSYS);
	
	QString newpath, name;
	BaseFs *fs = findFs(path,name,newpath);
	if(fs == NULL) return(-ENOENT);
	
	return(fs->fs_readlink(qPrintable(newpath),buf,size));
}

int NetFs::fs_access(const char *path, int mask)
{
	if(QString(path) == "/") return(-ENOSYS);
	
	QString newpath, name;
	BaseFs *fs = findFs(path,name,newpath);
	if(fs == NULL) return(-ENOENT);
	
	return(fs->fs_access(qPrintable(newpath),mask));
}

int NetFs::fs_mknod(const char *path, mode_t mode, dev_t rdev)
{
	if(QString(path) == "/") return(-ENOSYS);
	
	QString newpath, name;
	BaseFs *fs = findFs(path,name,newpath);
	if(fs == NULL) return(-ENOENT);
	
	return(fs->fs_mknod(qPrintable(newpath),mode,rdev));
}

int NetFs::fs_mkdir(const char *path, mode_t mode)
{
	if(QString(path) == "/") return(-ENOSYS);
	
	QString newpath, name;
	BaseFs *fs = findFs(path,name,newpath);
	if(fs == NULL) return(-ENOENT);
	
	return(fs->fs_mkdir(qPrintable(newpath),mode));
}

int NetFs::fs_unlink(const char *path)
{
	if(QString(path) == "/") return(-ENOSYS);
	
	QString newpath, name;
	BaseFs *fs = findFs(path,name,newpath);
	if(fs == NULL) return(-ENOENT);
	
	return(fs->fs_unlink(qPrintable(newpath)));
}

int NetFs::fs_rmdir(const char *path)
{
	if(QString(path) == "/") return(-ENOSYS);
	
	QString newpath, name;
	BaseFs *fs = findFs(path,name,newpath);
	if(fs == NULL) return(-ENOENT);
	
	return(fs->fs_rmdir(qPrintable(newpath)));
}

int NetFs::fs_symlink(const char *from, const char *to)
{
	if(QString(from) == "/") return(-ENOSYS);
	
	QString newpath, name;
	BaseFs *fs = findFs(from,name,newpath);
	if(fs == NULL) return(-ENOENT);
	
	return(fs->fs_symlink(qPrintable(newpath),to));
}

int NetFs::fs_rename(const char *from, const char *to)
{
	if(QString(from) == "/") return(-ENOSYS);
	if(QString(to) == "/") return(-ENOSYS);
	
	QString newpath, name;
	BaseFs *fs = findFs(from,name,newpath);
	if(fs == NULL) return(-ENOENT);
	
	QString newpath2, name2;
	BaseFs *fs2 = findFs(to,name2,newpath2);
	if(fs2 == NULL) return(-ENOENT);
	
	if(name != name2) return(-ENOSYS);
	
	return(fs->fs_rename(qPrintable(newpath),qPrintable(newpath2)));
}

int NetFs::fs_link(const char *from, const char *to)
{
	if(QString(from) == "/") return(-ENOSYS);
	if(QString(to) == "/") return(-ENOSYS);
	
	QString newpath, name;
	BaseFs *fs = findFs(from,name,newpath);
	if(fs == NULL) return(-ENOENT);
	
	QString newpath2, name2;
	BaseFs *fs2 = findFs(to,name2,newpath2);
	if(fs2 == NULL) return(-ENOENT);
	
	if(name != name2) return(-ENOSYS);
	
	return(fs->fs_link(qPrintable(newpath),qPrintable(newpath2)));
}

int NetFs::fs_chmod(const char *path, mode_t mode)
{
	if(QString(path) == "/") return(-ENOSYS);
	
	QString newpath, name;
	BaseFs *fs = findFs(path,name,newpath);
	if(fs == NULL) return(-ENOENT);
	
	return(fs->fs_chmod(qPrintable(newpath),mode));
}

int NetFs::fs_chown(const char *path, uid_t uid, gid_t gid)
{
	if(QString(path) == "/") return(-ENOSYS);
	
	QString newpath, name;
	BaseFs *fs = findFs(path,name,newpath);
	if(fs == NULL) return(-ENOENT);
	
	return(fs->fs_chown(qPrintable(newpath),uid,gid));
}

int NetFs::fs_truncate(const char *path, off_t size)
{
	if(QString(path) == "/") return(-ENOSYS);
	
	QString newpath, name;
	BaseFs *fs = findFs(path,name,newpath);
	if(fs == NULL) return(-ENOENT);
	
	return(fs->fs_truncate(qPrintable(newpath),size));
}

int NetFs::fs_utime(const char *path, struct utimbuf *buf)
{
	if(QString(path) == "/") return(-ENOSYS);
	
	QString newpath, name;
	BaseFs *fs = findFs(path,name,newpath);
	if(fs == NULL) return(-ENOENT);
	
	return(fs->fs_utime(qPrintable(newpath),buf));
}

int NetFs::fs_utimens(const char *path, const struct timespec ts[2])
{
	if(QString(path) == "/") return(-ENOSYS);
	
	QString newpath, name;
	BaseFs *fs = findFs(path,name,newpath);
	if(fs == NULL) return(-ENOENT);
	
	return(fs->fs_utimens(qPrintable(newpath),ts));
}

int NetFs::fs_statfs(const char *path, struct statvfs *stbuf)
{
	if(QString(path) == "/") return(-ENOSYS);
	
	QString newpath, name;
	BaseFs *fs = findFs(path,name,newpath);
	if(fs == NULL) return(-ENOENT);
	
	return(fs->fs_statfs(qPrintable(newpath), stbuf));
}

int NetFs::fs_open(const char *path, struct fuse_file_info *fi)
{
	if(QString(path) == "/") return(-ENOSYS);
	
	QString newpath, name;
	BaseFs *fs = findFs(path,name,newpath);
	if(fs == NULL) return(-ENOENT);
	
	return(fs->fs_open(qPrintable(newpath), fi));
}

int NetFs::fs_release(const char *path, struct fuse_file_info *fi)
{
	if(QString(path) == "/") return(-ENOSYS);
	
	QString newpath, name;
	BaseFs *fs = findFs(path,name,newpath);
	if(fs == NULL) return(-ENOENT);
	
	return(fs->fs_release(qPrintable(newpath), fi));
}

int NetFs::fs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	if(QString(path) == "/") return(-ENOSYS);
	
	QString newpath, name;
	BaseFs *fs = findFs(path,name,newpath);
	if(fs == NULL) return(-ENOENT);
	
	return(fs->fs_read(qPrintable(newpath), buf, size, offset, fi));
}

int NetFs::fs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	if(QString(path) == "/") return(-ENOSYS);
	
	QString newpath, name;
	BaseFs *fs = findFs(path,name,newpath);
	if(fs == NULL) return(-ENOENT);
	
	return(fs->fs_write(qPrintable(newpath), buf, size, offset, fi));
}
int NetFs::fs_fsync(const char *path, int i, struct fuse_file_info *fi)
{
	if(QString(path) == "/") return(-ENOSYS);
	
	QString newpath, name;
	BaseFs *fs = findFs(path,name,newpath);
	if(fs == NULL) return(-ENOENT);
	
	return(fs->fs_fsync(qPrintable(newpath),i, fi));
}

