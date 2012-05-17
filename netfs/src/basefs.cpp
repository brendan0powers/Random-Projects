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
#include "basefs.h"

#include <errno.h>

BaseFs::BaseFs(QString name, QObject *parent)
		: QObject(parent)
{
	_name = name;
}


BaseFs::~BaseFs()
{
}


int BaseFs::fs_getattr(const char *, struct stat *)
{
	return(-ENOSYS);
}

int BaseFs::fs_opendir(const char *, struct fuse_file_info *)
{
	return(0);
}

int BaseFs::fs_readdir(const char *, void *, fuse_fill_dir_t, off_t, struct fuse_file_info *)
{
	return(-ENOSYS);
}

int BaseFs::fs_releasedir(const char *, struct fuse_file_info *)
{
	return(0);
}

int BaseFs::fs_readlink(const char *, char *, size_t)
{
	return(-ENOSYS);
}

int BaseFs::fs_access(const char *, int)
{
	return(-ENOSYS);
}

int BaseFs::fs_mknod(const char *, mode_t, dev_t)
{
	return(-ENOSYS);
}

int BaseFs::fs_mkdir(const char *, mode_t)
{
	return(-ENOSYS);
}

int BaseFs::fs_unlink(const char *)
{
	return(-ENOSYS);
}

int BaseFs::fs_rmdir(const char *)
{
	return(-ENOSYS);
}

int BaseFs::fs_symlink(const char *, const char *)
{
	return(-ENOSYS);
}

int BaseFs::fs_rename(const char *, const char *)
{
	return(-ENOSYS);
}

int BaseFs::fs_link(const char *, const char *)
{
	return(-ENOSYS);
}

int BaseFs::fs_chmod(const char *, mode_t)
{
	return(-ENOSYS);
}
int BaseFs::fs_chown(const char *, uid_t, gid_t)
{
	return(-ENOSYS);
}

int BaseFs::fs_truncate(const char *, off_t)
{
	return(-ENOSYS);
}

int BaseFs::fs_utime(const char *, struct utimbuf *)
{
	return(-ENOSYS);
}

int BaseFs::fs_utimens(const char *, const struct timespec *)
{
	return(-ENOSYS);
}

int BaseFs::fs_statfs(const char *, struct statvfs *)
{
	return(-ENOSYS);
}

int BaseFs::fs_open(const char *, struct fuse_file_info *)
{
	return(-ENOSYS);
}

int BaseFs::fs_release(const char *, struct fuse_file_info *)
{
	return(-ENOSYS);
}

int BaseFs::fs_read(const char *, char *, size_t, off_t, struct fuse_file_info *)
{
	return(-ENOSYS);
}

int BaseFs::fs_write(const char *, const char *, size_t, off_t, struct fuse_file_info *)
{
	return(-ENOSYS);
}
int BaseFs::fs_fsync(const char *, int, struct fuse_file_info *)
{
	return(-ENOSYS);
}
