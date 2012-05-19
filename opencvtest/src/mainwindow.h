/***************************************************************************
 *   Copyright (C) 2009 by Brendan Powers   *
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_MainWindow.h"
#include <cv.h>
#include <highgui.h>
#include <QList>
#include <QRect>
#include <QSettings>
#include <QPair>
#include <unicap.h>

typedef QPair<QPoint,int> Circle;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = 0, Qt::WFlags fl = 0);
	~MainWindow();

	QImage toImage(IplImage *cvimage);

protected slots:
	void getImage();

protected:
	QList<QRect> detectObjects(IplImage *frame, CvHaarClassifierCascade* cascade);
	QList<Circle> detectCircles(IplImage *frame);

private:
	unicap_handle_t openCamera(QString dev, unicap_data_buffer_t *buffer, IplImage **image, unicap_callback_t callback);
	CvCapture *_capture;
	CvMemStorage *_storage;
	CvHaarClassifierCascade* _face;
	unicap_handle_t _leftcamera;
	unicap_handle_t _rightcamera;
	unicap_data_buffer_t _leftbuffer;
	unicap_data_buffer_t _rightbuffer;
	IplImage * _leftimage;
	IplImage * _rightimage;
	QSettings *_settings;
	int _width;
	int _height;
	
	friend void left_cb( unicap_event_t event, unicap_handle_t handle, unicap_data_buffer_t *buffer, MainWindow *window );
	friend void right_cb( unicap_event_t event, unicap_handle_t handle, unicap_data_buffer_t *buffer, MainWindow *window );
};

#endif

