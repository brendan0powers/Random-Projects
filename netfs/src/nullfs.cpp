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
#include "nullfs.h"
#include <errno.h>

NullFs::NullFs(QString name, QObject *parent)
		: BaseFs(name,parent)
{
}


NullFs::~NullFs()
{
}

ReturnValue NullFs::map(QString, QStringList)
{
	return(true);
}

ReturnValue NullFs::unMap()
{
	return(true);
}


int NullFs::fs_getattr(const char *path, struct stat *stbuf)
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
	

	return(-ENOENT);
}

int NullFs::fs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t, struct fuse_file_info *)
{
	if(QString(path) == "/")
	{
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);

		return(0);
	}
	return(-ENOENT);
}

int NullFs::fs_readlink(const char *, char *, size_t)
{
	return(-EACCES);
}

int NullFs::fs_access(const char *, int)
{
	return(-EACCES);
}

int NullFs::fs_mknod(const char *, mode_t, dev_t)
{
	return(-EACCES);
}

int NullFs::fs_mkdir(const char *, mode_t)
{
	return(-EACCES);
}

int NullFs::fs_unlink(const char *)
{
	return(-EACCES);
}

int NullFs::fs_rmdir(const char *)
{
	return(-EACCES);
}

int NullFs::fs_symlink(const char *, const char *)
{
	return(-EACCES);
}

int NullFs::fs_rename(const char *, const char *)
{
	return(-EACCES);
}

int NullFs::fs_link(const char *, const char *)
{
	return(-EACCES);
}

int NullFs::fs_chmod(const char *, mode_t)
{
	return(-EACCES);
}
int NullFs::fs_chown(const char *, uid_t, gid_t)
{
	return(-EACCES);
}

int NullFs::fs_truncate(const char *, off_t)
{
	return(-EACCES);
}

int NullFs::fs_utimens(const char *, const struct timespec *)
{
	return(-EACCES);
}

int NullFs::fs_statfs(const char *, struct statvfs *)
{
	return(-EACCES);
}

int NullFs::fs_open(const char *, struct fuse_file_info *)
{
	return(-EACCES);
}

int NullFs::fs_release(const char *, struct fuse_file_info *)
{
	return(-EACCES);
}

int NullFs::fs_read(const char *, char *, size_t, off_t, struct fuse_file_info *)
{
	return(-EACCES);
}

int NullFs::fs_write(const char *, const char *, size_t, off_t, struct fuse_file_info *)
{
	return(-EACCES);
}
int NullFs::fs_fsync(const char *, int, struct fuse_file_info *)
{
	return(-EACCES);
}
