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
#include "symlinkfs.h"
#include <string.h>
#include <errno.h>

SymlinkFs::SymlinkFs(QString name, QObject *parent)
		: BaseFs(name, parent)
{
}


SymlinkFs::~SymlinkFs()
{
}

/*!
    \fn SymlinkFs::map(QString src, QStringList options)
 */
ReturnValue SymlinkFs::map(QString src, QStringList)
{
	_src=src;
	return(true);
}


/*!
    \fn SymlinkFs::unMap()
 */
ReturnValue SymlinkFs::unMap()
{
	return(true);
}


/*!
    \fn SymlinkFs::fs_getattr(const char *path, struct stat *stbuf)
 */
int SymlinkFs::fs_getattr(const char *path, struct stat *stbuf)
{
	if(QString(path) == "/")
	{
		stbuf->st_mode = S_IFLNK | 0777;
		stbuf->st_nlink = 1;
		stbuf->st_uid = getuid();
		stbuf->st_gid = getgid();
		stbuf->st_atime = stbuf->st_mtime = stbuf->st_ctime = time(NULL);
		
		return(0);
	}
	

	return(-ENOENT);
}


/*!
    \fn SymlinkFs::fs_readlink(const char *path, char *buf, size_t size)
 */
int SymlinkFs::fs_readlink(const char *path, char *buf, size_t size)
{
	if(QString(path) == "/")
	{
		strncpy(buf,qPrintable(_src),size);
		
		return(0);
	}
	

	return(-ENOENT);
}
