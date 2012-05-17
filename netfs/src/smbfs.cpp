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
#include "smbfs.h"
#include <errno.h>

#include <QDebug>
#include <QMutexLocker>
#include <sys/statvfs.h>

void auth_fn(SMBCCTX *c, const char *srv, const char *shr, char *wg, int wglen, char *un, int unlen, char *pw, int pwlen)
{
	SmbFs *fs = (SmbFs *) smbc_getOptionUserData(c);
	fs->auth(srv,shr,wg,wglen,un,unlen,pw,pwlen);
}


SmbFs::SmbFs(QString name, QObject *parent)
		: BaseFs(name,parent)
{
	_ctx = smbc_new_context();
	smbc_setTimeout(_ctx,10000);
	smbc_setOptionUserData(_ctx,this); //set this class as the user data, so we can get it from the auth function
	smbc_setOptionOneSharePerServer(_ctx,true);
	smbc_setFunctionAuthDataWithContext(_ctx,auth_fn); //set the authentication function
	_ctx = smbc_init_context(_ctx);
	if(_ctx == NULL) qDebug() << "Null Context!" << strerror(errno);
}


SmbFs::~SmbFs()
{
}

ReturnValue SmbFs::map(QString src, QStringList args)
{
	//make sure the path has / and \, and that it doesn't end with a /
	src = src.replace("\\","/");
	if(src.endsWith("/")) src.mid(0,src.length()-1);
	
	_path=src;
	
	if(args.size() == 2)
	{
		_user = args[0];
		_pass = args[1];
	}
	
	struct stat *buf = new struct stat();
	
	int ret=0;
	
	if((ret = fs_getattr("/",buf)) == 0)
	{
		qDebug() << "Success!";
		return(true);
	}
	else
	{
		qDebug() << "Failure!" << strerror(errno);
		return(ReturnValue(1,QString("Failed to map: ") + strerror(errno)));
	}
	
	return(false);
}

ReturnValue SmbFs::unMap()
{
	return(true);
}

QString SmbFs::getPath(QString path)
{
	return("smb:" + _path + path);
}

void SmbFs::auth(const char *srv, const char *shr, char *wg, int wglen, char *un, int unlen, char *pw, int pwlen)
{
	strncpy(un,qPrintable(_user),unlen);
	strncpy(pw,qPrintable(_pass),pwlen);
}

int SmbFs::fs_getattr(const char *path, struct stat *stbuf)
{
	QMutexLocker locker(&_mutex);
	
	if((smbc_getFunctionStat(_ctx))(_ctx, qPrintable(getPath(path)),stbuf) == 0)
		return(0);
	else
		return(-errno);
}

int SmbFs::fs_opendir(const char *path, struct fuse_file_info *fi)
{
	QMutexLocker locker(&_mutex);

	SMBCFILE *dir = (smbc_getFunctionOpendir(_ctx))(_ctx, qPrintable(getPath(path)));
	
	if(dir == NULL) return(-errno);
	
	fi->fh = (unsigned long)dir;
	
	return(0);
}

int SmbFs::fs_readdir(const char *, void *buf, fuse_fill_dir_t filler, off_t, struct fuse_file_info *fi)
{
	QMutexLocker locker(&_mutex);
	smbc_dirent *ent;
	
	while((ent = (smbc_getFunctionReaddir(_ctx))(_ctx, (SMBCFILE *)fi->fh)) != NULL)
	{
		filler(buf,ent->name,0,0);
	}
	
	return(0);
}

int SmbFs::fs_releasedir(const char *path, struct fuse_file_info *fi)
{
	QMutexLocker locker(&_mutex);
	(smbc_getFunctionClosedir(_ctx))(_ctx, (SMBCFILE *)fi->fh);
	return(0);
}

int SmbFs::fs_readlink(const char *, char *, size_t)
{
	return(-ENOSYS);
}

int SmbFs::fs_access(const char *, int)
{
	return(-ENOSYS);
}

int SmbFs::fs_mknod(const char *path, mode_t mode, dev_t dev)
{
	QMutexLocker locker(&_mutex);

	SMBCFILE *file;
	
	if(( file = (smbc_getFunctionCreat(_ctx))(_ctx, qPrintable(getPath(path)),mode) ) != NULL)
	{
		(smbc_getFunctionClose(_ctx))(_ctx, file);
		return(0);
	}
	else
		return(-errno);
}

int SmbFs::fs_mkdir(const char *path, mode_t mode)
{
	QMutexLocker locker(&_mutex);
	
	if((smbc_getFunctionMkdir(_ctx))(_ctx, qPrintable(getPath(path)),mode) == 0)
		return(0);
	else
		return(-errno);
}

int SmbFs::fs_unlink(const char *path)
{
	QMutexLocker locker(&_mutex);
	
	if((smbc_getFunctionUnlink(_ctx))(_ctx, qPrintable(getPath(path))) == 0)
		return(0);
	else
		return(-errno);
}

int SmbFs::fs_rmdir(const char *path)
{
	QMutexLocker locker(&_mutex);
	
	if((smbc_getFunctionRmdir(_ctx))(_ctx, qPrintable(getPath(path))) == 0)
		return(0);
	else
		return(-errno);
}

int SmbFs::fs_symlink(const char *, const char *)
{
	return(-ENOSYS);
}

int SmbFs::fs_rename(const char *from, const char *to)
{
	QMutexLocker locker(&_mutex);
	
	if((smbc_getFunctionRename(_ctx))(_ctx, qPrintable(getPath(from)),_ctx,qPrintable(getPath(to))) == 0)
		return(0);
	else
		return(-errno);
}

int SmbFs::fs_link(const char *, const char *)
{
	return(-ENOSYS);
}

int SmbFs::fs_chmod(const char *, mode_t)
{
	return(-ENOSYS);
}
int SmbFs::fs_chown(const char *, uid_t, gid_t)
{
	return(-ENOSYS);
}

int SmbFs::fs_truncate(const char *path, off_t size)
{
	if(size == 0)
	{
		return(fs_mknod(path,0666,0));
	}
	else
	{
		struct stat *buf = new struct stat();
		int ret=0;
		if((ret = fs_getattr(path,buf)) != 0) return(ret);
		if(size == buf->st_size) return(0);
	}
	
	return(-ENOSYS);
}

int SmbFs::fs_utime(const char *path, struct utimbuf *buf)
{
	QMutexLocker locker(&_mutex);
	
	struct timeval tbuf[2];
	tbuf[0].tv_sec = buf->actime;
	tbuf[0].tv_usec = 0;
	tbuf[1].tv_sec = buf->modtime;
	tbuf[1].tv_usec = 0;
	
	if((smbc_getFunctionUtimes(_ctx))(_ctx, qPrintable(getPath(path)),tbuf) == 0)
		return(0);
	else
		return(-errno);
}

int SmbFs::fs_statfs(const char *, struct statvfs *buf)
{
	//Statfs on remote fs is too expensive, so we just stat the local fs
	memset(buf,0,sizeof(struct statvfs));
	if(statvfs("/",buf) != 0)
		return(-errno);
	else 
		return(0);
}

int SmbFs::fs_open(const char *path, struct fuse_file_info *fi)
{
	QMutexLocker locker(&_mutex);
	
	SMBCFILE *file = (smbc_getFunctionOpen(_ctx))(_ctx, qPrintable(getPath(path)),fi->flags,0);
	
	if(file != NULL)
	{
		fi->fh = (unsigned long)file;
		return(0);
	}
	else
		return(-errno);
}

int SmbFs::fs_release(const char *, struct fuse_file_info *fi)
{
	QMutexLocker locker(&_mutex);
	
	if((smbc_getFunctionClose(_ctx))(_ctx, (SMBCFILE *)fi->fh) == 0)
		return(0);
	else
		return(-errno);
}

int SmbFs::fs_read(const char *, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	QMutexLocker locker(&_mutex);
	size_t s;
	
	if((smbc_getFunctionLseek(_ctx))(_ctx, (SMBCFILE *)fi->fh, offset, SEEK_SET) == (off_t)-1)
		return(-errno);
	
	if((s = (smbc_getFunctionRead(_ctx))(_ctx, (SMBCFILE *)fi->fh, buf,size)) < 0)
		return(-errno);
	
	return(s);
}

int SmbFs::fs_write(const char *, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	QMutexLocker locker(&_mutex);
	size_t s;
	
	if((smbc_getFunctionLseek(_ctx))(_ctx, (SMBCFILE *)fi->fh, offset, SEEK_SET) == (off_t)-1)
		return(-errno);
	
	if((s = (smbc_getFunctionWrite(_ctx))(_ctx, (SMBCFILE *)fi->fh, (void *)buf,size)) < 0)
		return(-errno);
	
	return(s);
}
int SmbFs::fs_fsync(const char *, int, struct fuse_file_info *)
{
	return(0);
}
