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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <utime.h>
#include <sys/time.h>
#include <sys/xattr.h>

#define FUSE_USE_VERSION 26
#include <fuse.h>

#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include <QDir>
#include "netfs.h"
#include "netclient.h"

static struct fuse_operations operations;	//Function Map
NetFs *netfs=NULL;

static int fs_getattr(const char *path, struct stat *stbuf)
{
	return(netfs->fs_getattr(path, stbuf));
}

static int fs_opendir(const char *path, struct fuse_file_info *fi)
{
	return(netfs->fs_opendir(path, fi));
}

static int fs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t off, struct fuse_file_info *info)
{
	return(netfs->fs_readdir(path, buf, filler, off, info));
}

static int fs_releasedir(const char *path, struct fuse_file_info *fi)
{
	return(netfs->fs_releasedir(path,fi));
}

static int fs_readlink(const char *path, char *buf, size_t size)
{
	return(netfs->fs_readlink(path, buf, size));
}

static int fs_access(const char *path, int mask)
{
	return(netfs->fs_access(path, mask));
}


static int fs_mknod(const char *path, mode_t mode, dev_t rdev)
{
	return(netfs->fs_mknod(path, mode, rdev));
}

static int fs_mkdir(const char *path, mode_t mode)
{
	return(netfs->fs_mkdir(path, mode));
}

static int fs_unlink(const char *path)
{
	return(netfs->fs_unlink(path));
}
	
static int fs_rmdir(const char *path)
{
	return(netfs->fs_rmdir(path));
}

static int fs_symlink(const char *from, const char *to)
{
	return(netfs->fs_symlink(from, to));
}

static int fs_rename(const char *from, const char *to)
{
	return(netfs->fs_rename(from, to));
}
	
static int fs_link(const char *from, const char *to)
{
	return(netfs->fs_link(from, to));
}
	
static int fs_chmod(const char *path, mode_t mode)
{
	return(netfs->fs_chmod(path, mode));
}

static int fs_chown(const char *path, uid_t uid, gid_t gid)
{
	return(netfs->fs_chown(path, uid, gid));
}

static int fs_truncate(const char *path, off_t size)
{
	return(netfs->fs_truncate(path, size));
}

static int fs_utime(const char *path, struct utimbuf *buf)
{
	return(netfs->fs_utime(path,buf));
}

static int fs_utimens(const char *path, const struct timespec ts[2])
{
	return(netfs->fs_utimens(path, ts));
}

static int fs_statfs(const char *path, struct statvfs *stbuf)
{
	return(netfs->fs_statfs(path, stbuf));
}

static int fs_open(const char *path, struct fuse_file_info *fi)
{
	return(netfs->fs_open(path, fi));
}

static int fs_release(const char *path, struct fuse_file_info *fi)
{
	return(netfs->fs_release(path, fi));
}


static int fs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	return(netfs->fs_read(path, buf, size, offset, fi));
}


static int fs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	return(netfs->fs_write(path, buf, size, offset, fi));
}

static int fs_fsync(const char *path, int i, struct fuse_file_info *fi)
{
	return(netfs->fs_fsync(path, i, fi));
}

static void init_operators()
{
	operations.getattr        = fs_getattr;
	operations.access         = fs_access;
	operations.readlink       = fs_readlink;
	operations.opendir	  = fs_opendir;
	operations.readdir        = fs_readdir;
	operations.releasedir     = fs_releasedir;
	operations.mknod          = fs_mknod;
	operations.mkdir          = fs_mkdir;
	operations.symlink        = fs_symlink;
	operations.unlink         = fs_unlink;
	operations.rmdir          = fs_rmdir;
	operations.rename         = fs_rename;
	operations.link           = fs_link;
	operations.chmod          = fs_chmod;
	operations.chown          = fs_chown;
	operations.truncate       = fs_truncate;
	operations.utime	  = fs_utime;
	operations.utimens        = fs_utimens;
	operations.open           = fs_open;
	operations.read           = fs_read;
	operations.write          = fs_write;
	operations.statfs         = fs_statfs;
	operations.release        = fs_release;
	operations.fsync          = fs_fsync;
	//operations.setxattr       = NULL;
	//operations.getxattr       = NULL;
	//operations.listxattr      = NULL;
	//operations.removexattr    = NULL;
}

void checkForMapArgs(int argc, char *argv[])
{
	if(argc <= 1) return;
	
	if(QString(argv[1]) != "-m" && QString(argv[1]) != "-u") return;
	
	QCoreApplication app(argc,argv);
	NetClient client(&app);
	
	ReturnValue ret = client.connect("socket://" + QDir::homePath() + "/.netfs-socket:Map");
	if(ret.isError())
	{
		qDebug() << "Failed to connect to netfs:" << ret.errString();
		exit(1);
	}
	
	if(QString(argv[1]) == "-m")
	{
		if(argc < 4)
		{
			qDebug() << "Usage: netfs -m name type [source] [opt1] [opt2] [opt3] etc...";
			exit(4);
		}
		
		QString name = argv[2];
		QString type = argv[3];
		QString src;
		if(argc >= 5) src = argv[4];
			
		QStringList options;
		for(int i=5; i < argc; i++)
		{
			options << argv[i];
		}
		
		ret = client.map(name,type,src,options);
		if(ret.isError())
		{
			qDebug() << "Faile to map:" << ret.errString();
			exit(2);
		}
		
		exit(0);
	}
	else if(QString(argv[1]) == "-u")
	{
		if(argc != 3)
		{
			qDebug() << "Usage: netfs -u name";
			exit(3);
		}
		
		ret = client.unMap(argv[2]);
		if(ret.isError())
		{
			qDebug() << "Faile to unmap:" << ret.errString();
			exit(2);
		}
		exit(0);
	}
}

int main(int argc, char *argv[])
{

	checkForMapArgs(argc,argv);
	
	//Fuse variables
	init_operators();
	struct fuse *handle;
	char *mountpoint;
	int threaded;

	//initialize fuse
	handle = fuse_setup(argc, argv, &operations, sizeof(operations), &mountpoint, &threaded, NULL);
	if (handle == NULL)
	{
		qDebug() << "Failed to initialize FUSE";
		exit(1);
	}

	//we should have forked, so start the qt app
	QCoreApplication app(argc, argv);
	netfs = new NetFs(&app);
	netfs->map("test1","null","",QStringList());
	netfs->map("test2","null","",QStringList());
	netfs->map("null","null","",QStringList());
	netfs->map("bad","bad","The drive didn't map for some reason\nasdfasddf\n",QStringList());
	netfs->map("link","symlink","/etc/fstab",QStringList());
	netfs->unMap("test1");
	
	//fuse loop
	int  ret = fuse_loop_mt(handle);

	//cleanup
	fuse_teardown(handle, mountpoint);
	if (ret == -1) exit(1);

	return 0;
}
 
