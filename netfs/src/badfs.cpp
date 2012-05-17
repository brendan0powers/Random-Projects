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
#include "badfs.h"
#include <errno.h>

const char *data="[Desktop Entry]\nEncoding=UTF-8\nType=Directory\nBgImage=\nIcon=folder_grey\n";

BadFs::BadFs(QString name, QObject *parent)
		: BaseFs(name, parent)
{
}


BadFs::~BadFs()
{
}

/*!
    \fn BadFs::map(QString src, QStringList options)
 */
ReturnValue BadFs::map(QString src, QStringList)
{
	_msg = src;
	return(true);
}


/*!
    \fn BadFs::unMap()
 */
ReturnValue BadFs::unMap()
{
	return(true);
}

/*!
    \fn BadFs::fs_getattr(const char *path, struct stat *stbuf)
 */
int BadFs::fs_getattr(const char *path, struct stat *stbuf)
{
	memset(stbuf, 0, sizeof(struct stat));
	stbuf->st_nlink = 1;
	if(QString(path) == "/")
	{
		stbuf->st_mode = S_IFDIR | 0755;
	}
	else if(QString(path) == "/.directory")
	{
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_size = strlen(data);
	}
	else if(QString(path) == "/READ ME")
	{
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_size = _msg.size();
	}
	else return(-ENOENT);
	
	return(0);
}


/*!
    \fn BadFs::fs_open(const char *path, struct fuse_file_info *fi)
 */
int BadFs::fs_open(const char *path, struct fuse_file_info *fi)
{
	if(QString(path) != "/.directory" && QString(path) != "/READ ME") return(-ENOENT);
	if ((fi->flags & 3) != O_RDONLY) return(-EACCES);
	
	return(0);

}


/*!
    \fn BadFs::fs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t, struct fuse_file_info *)
 */
int BadFs::fs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t, struct fuse_file_info *)
{
	if(QString(path) != "/") return(-ENOENT);
	
	filler(buf, ".", NULL,0);
	filler(buf,"..", NULL,0);
	filler(buf,".directory", NULL,0);
	filler(buf,"READ ME", NULL,0);
	
	return(0);
}


/*!
    \fn BadFs::fs_read(const char *, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
 */
int BadFs::fs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *)
{
	if(QString(path) == "/.directory")
	{
		size_t len = strlen(data);
		if(offset < len)
		{
			if(offset + size > len)
			{
				size = len - offset;
				memcpy(buf,data + offset, size);
			}
		}
		else return(0);
		
		return(size);
	}
	else if(QString(path) == "/READ ME")
	{
		size_t len = strlen(qPrintable(_msg));
		if(offset < len)
		{
			if(offset + size > len)
			{
				size = len - offset;
				memcpy(buf,qPrintable(_msg) + offset, size);
			}
		}
		else return(0);
		
		return(size);
	}
	else
	{
		return(-ENOENT);
	}
}
