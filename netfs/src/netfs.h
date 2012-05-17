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
#ifndef NETFS_H
#define NETFS_H

#define FUSE_USE_VERSION 26
#include <fuse.h>

#include <QObject>
#include <QStringList>
#include <QMap>
#include <QReadWriteLock>
#include <ReturnValue>
#include "basefs.h"

using namespace QtRpc;

/**
	@author Brendan Powers <brendan@resara.com>
*/
class NetFs : public QObject
{
	Q_OBJECT
public:
	NetFs(QObject *parent = 0);
	~NetFs();

	ReturnValue map(QString name, QString type, QString src, QStringList options);
	ReturnValue unMap(QString name);;
	BaseFs *findFs(QString path, QString &name, QString &newpath);
	
	//FS functions
	int fs_getattr(const char *path, struct stat *stbuf);
	int fs_opendir(const char *path, struct fuse_file_info *fi);
	int fs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t, struct fuse_file_info *);
	int fs_releasedir(const char *path, struct fuse_file_info *fi);
	int fs_readlink(const char *path, char *buf, size_t size);
	int fs_access(const char *path, int mask);
	int fs_mknod(const char *path, mode_t mode, dev_t rdev);
	int fs_mkdir(const char *path, mode_t mode);
	int fs_unlink(const char *path);
	int fs_rmdir(const char *path);
	int fs_symlink(const char *from, const char *to);
	int fs_rename(const char *from, const char *to);
	int fs_link(const char *from, const char *to);
	int fs_chmod(const char *path, mode_t mode);
	int fs_chown(const char *path, uid_t uid, gid_t gid);
	int fs_truncate(const char *path, off_t size);
	int fs_utime(const char *path, struct utimbuf *buf);
	int fs_utimens(const char *path, const struct timespec ts[2]);
	int fs_statfs(const char *path, struct statvfs *stbuf);
	int fs_open(const char *path, struct fuse_file_info *fi);
	int fs_release(const char *, struct fuse_file_info *fi);
	int fs_read(const char *, char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
	int fs_write(const char *, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
	int fs_fsync(const char *, int, struct fuse_file_info *fi);

private:
	QMap<QString, BaseFs *> _maps;
	QReadWriteLock lock;
};

#endif


