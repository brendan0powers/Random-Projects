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
#ifndef BADFS_H
#define BADFS_H

#include <basefs.h>
#include <QStringList>

/**
	@author Brendan Powers <brendan@resara.com>
*/
class BadFs : public BaseFs
{
	Q_OBJECT
public:
	BadFs(QString name, QObject *parent = 0);
	~BadFs();

	ReturnValue map(QString src, QStringList options);
	ReturnValue unMap();
	int fs_getattr(const char *path, struct stat *stbuf);
	int fs_open(const char *path, struct fuse_file_info *fi);
	int fs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t, struct fuse_file_info *);
	int fs_read(const char *, char *buf, size_t size, off_t offset, struct fuse_file_info *fi);

private:
	QString _msg;
};

#endif



