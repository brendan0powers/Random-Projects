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


#include "mainwindow.h"
#include <QMessageBox>
#include <QTimer>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QApplication>
#include <QMutex>
#include <QDateTime>
#include <ucil.h>
#define UCIL_FOURCC(a,b,c,d) (unsigned int)((((unsigned int)d)<<24)+(((unsigned int)c)<<16)+(((unsigned int)b)<<8)+a)

QMutex framelock;



void left_cb(unicap_event_t event, unicap_handle_t handle, unicap_data_buffer_t *buffer, MainWindow *window)
{
	Q_UNUSED(handle);
	Q_UNUSED(event);
	framelock.lock();
	//memcpy(window->_leftbuffer.data, buffer->data, window->_leftbuffer.buffer_size);
	ucil_convert_buffer(&window->_leftbuffer, buffer);
	framelock.unlock();
}

void right_cb(unicap_event_t event, unicap_handle_t handle, unicap_data_buffer_t *buffer, MainWindow *window)
{
	Q_UNUSED(handle);
	Q_UNUSED(event);
	framelock.lock();
	//memcpy(window->_rightbuffer.data, buffer->data, window->_rightbuffer.buffer_size);
	ucil_convert_buffer(&window->_rightbuffer, buffer);
	framelock.unlock();
}

MainWindow::MainWindow(QWidget* parent, Qt::WFlags fl)
		: QMainWindow(parent, fl), Ui::MainWindow()
{
	setupUi(this);

	_settings = new QSettings("./cameras.ini", QSettings::IniFormat);
	_width = _settings->value("width").toInt();
	_height = _settings->value("height").toInt();
	_leftcamera = openCamera(_settings->value("leftcamera").toString(), &_leftbuffer, &_leftimage, (unicap_callback_t) left_cb);
	_rightcamera = openCamera(_settings->value("rightcamera").toString(), &_rightbuffer, &_rightimage, (unicap_callback_t) right_cb);

	if (!_leftcamera)
	{
		QMessageBox::critical(this, "Error", "Unable to open the left camera!");
		exit(1);
	}
	if (!_rightcamera)
	{
		QMessageBox::critical(this, "Error", "Unable to open the right camera!");
		exit(1);
	}

	_storage = cvCreateMemStorage(0);

	_face = (CvHaarClassifierCascade*) cvLoad("/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml", 0, 0, 0);
	if (!_face)
	{
		QMessageBox::critical(this, "Error", "Unable to load cascade");
		exit(1);
	}

	QTimer *timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(getImage()));
	timer->start(0);
}

unicap_handle_t MainWindow::openCamera(QString dev, unicap_data_buffer_t *buffer, IplImage **image, unicap_callback_t callback)
{
	unicap_handle_t handle;
	unicap_device_t device, device_spec;
	unicap_format_t format, format_spec;

	unicap_void_device(&device_spec);
	strncpy(device_spec.identifier, qPrintable(dev), 127);
	if (!SUCCESS(unicap_enumerate_devices(&device_spec, &device, 0)))
	{
		qWarning() << "Failed to look up device from device string!";
		return (false);
	}

	if (!SUCCESS(unicap_open(&handle, &device)))
	{
		qDebug() << "Failed to open device!";
		return (false);
	}

	unicap_void_format(&format_spec);
	format_spec.fourcc = UCIL_FOURCC('Y', 'U', 'Y', 'V');
	format_spec.size.width = _width;
	format_spec.size.height  = _height;

	if (!SUCCESS(unicap_enumerate_formats(handle, &format_spec, &format, 0)))
	{
		qDebug() << "Failed to enumerate formats¡";
		return (false);
	}

	format.buffer_type = UNICAP_BUFFER_TYPE_SYSTEM;
	format.size.width = _width;
	format.size.height  = _height;

	if (!SUCCESS(unicap_set_format(handle, &format)))
	{
		qDebug() << "Failed to set format!";
		return (false);
	}

	if (!SUCCESS(unicap_get_format(handle, &format)))
	{
		qDebug() << "Failed to get format!";
		return (false);
	}

	qDebug() << format.size.width << format.size.height << format.fourcc;

	unicap_copy_format(&buffer->format, &format);
	buffer->format.fourcc = UCIL_FOURCC('Y', '8', '0', '0');
	buffer->format.bpp = 8;
	buffer->format.buffer_size = buffer->buffer_size = (format.size.width * format.size.height);
	buffer->data = (uchar *) malloc(buffer->buffer_size);

	*image = cvCreateImage(cvSize(format.size.width, format.size.height), 8, 1);
	(*image)->imageData = (char *) buffer->data;

	unicap_start_capture(handle);
	unicap_register_callback(handle, UNICAP_EVENT_NEW_FRAME, callback, this);
	//unicap_queue_buffer(handle, buffer);

	qDebug() << "Camera Opened";

	return (handle);
}

MainWindow::~MainWindow()
{
}

void MainWindow::getImage()
{
	IplImage *lefttmp = cvCreateImage(cvSize(_width, _height), IPL_DEPTH_8U, 3);
	IplImage *leftcanny = cvCreateImage(cvSize(_width, _height), IPL_DEPTH_8U, 1);
	IplImage *righttmp = cvCreateImage(cvSize(_width, _height), IPL_DEPTH_8U, 3);
	IplImage *rightcanny = cvCreateImage(cvSize(_width, _height), IPL_DEPTH_8U, 1);
	QImage left, right;
	QList<QRect> leftfaces, rightfaces;

	qDebug() << "Processing Frame";

	framelock.lock();

	qDebug() << "BEFORE";
		cvSmooth( _leftimage, _leftimage, CV_GAUSSIAN, 11,11 );
	cvSmooth( _rightimage, _rightimage, CV_GAUSSIAN, 11, 11 );
	cvCanny(_leftimage,leftcanny,A->text().toInt(),B->text().toInt(),C->text().toInt());
	cvCanny(_rightimage,rightcanny,A->text().toInt(),B->text().toInt(),C->text().toInt());
	qDebug() << "AFTER";
	
	QList<Circle> leftcircles = detectCircles(_leftimage);
	QList<Circle> rightcircles = detectCircles(_rightimage);
	
	cvCvtColor(leftcanny, lefttmp, CV_GRAY2RGB);
	cvCvtColor(rightcanny, righttmp, CV_GRAY2RGB);
	qDebug() << "Color";
	
	
	
	
	//leftfaces = detectObjects(lefttmp, _face);
	//rightfaces = detectObjects(righttmp, _face);

	qDebug() << "Unlock";
	
	left = QImage((uchar *) lefttmp->imageData, _width, _height, QImage::Format_RGB888);
	right = QImage((uchar *) righttmp->imageData, _width, _height, QImage::Format_RGB888);
	
	framelock.unlock();
	
	qDebug() << "Image";
	/*
	time_t time = QDateTime::currentDateTime().toTime_t();
	left.save(QString("left-%1.jpg").arg(time));
	right.save(QString("right-%1.jpg").arg(time));
	*/
	QPainter painter;
	painter.begin(&left);
	painter.setPen(QPen(QBrush(QColor(0, 0, 255)), 2));
	foreach(Circle circle, leftcircles)
	{
		painter.drawEllipse(circle.first,circle.second,circle.second);
	}
	painter.end();

	painter.begin(&right);
	painter.setPen(QPen(QBrush(QColor(0, 0, 255)), 2));
	foreach(Circle circle, rightcircles)
	{
		painter.drawEllipse(circle.first,circle.second,circle.second);
	}

	/*
	
	bool foundface = false; //if we found a face for any matching rect
	
	//for each face in the left image
	foreach(QRect leftrect, leftfaces)
	{
		//see if there is a face in the right image that matches
		bool found = false;
		QRect rightrect;
		foreach(rightrect, rightfaces)
		{
			if (leftrect.intersects(rightrect))
			{
				found = true;
				foundface = true;
				break;
			}
		}

		//We found a matching face, do calculations
		if (found)
		{
			painter.setPen(QPen(QBrush(QColor(0, 0, 255)), 2, Qt::DashLine));
			painter.drawRect(leftrect);
			painter.setPen(QPen(Qt::SolidLine));
			painter.setFont(QFont("Helvetica", 48));
			painter.setPen(QPen(QBrush(QColor(0, 255, 0)), 2));
			painter.drawRect(rightrect);
			QPoint a = leftrect.center();
			QPoint b = rightrect.center();
			int disparity = b.x() - a.x();
			if (disparity > 0)
				painter.drawText(b, QString("%1").arg(disparity));
			else
				painter.drawText(b, QString("INF"));
		}
	}

	//if we found a face, save the image
	if(foundface)
	{
		right.save(QString("found-%1.jpg").arg(QDateTime::currentDateTime().toTime_t()));
	}
	
	painter.end();

	*/
	
	QPixmap leftpixmap = QPixmap::fromImage(left);
	QPixmap rightpixmap = QPixmap::fromImage(right);
	
	LeftImage->setPixmap(leftpixmap);
	RightImage->setPixmap(rightpixmap);
	
	qDebug() << "SetImage";
	
	cvReleaseImage(&lefttmp);
	cvReleaseImage(&righttmp);
	
	qDebug() << "Release";

	/*QImage image = toImage(frame);

	QPainter painter;
	painter.begin(&image);


	painter.setPen(QPen(QBrush(QColor(0,0,255)),2));
	foreach(QRect rect, detectObjects(frame,_face))
	{
		painter.drawRect(rect);
	}

	painter.end();

	LeftImage->setPixmap(QPixmap::fromImage(image));
	*/

	//unicap_queue_buffer(_leftcamera,&_leftbuffer);
	//unicap_queue_buffer(_rightcamera,&_rightbuffer);

	QApplication::processEvents();
}

QList<Circle> MainWindow::detectCircles(IplImage *frame)
{
	CvMemStorage* cstorage = cvCreateMemStorage(0);
	QList<Circle> circlelist;
	
	CvSeq* circles =  cvHoughCircles(frame, cstorage, CV_HOUGH_GRADIENT, 4, frame->height/50.0);
	
	qDebug() << "Total:" << circles->total;
	
	for(int i=0; i < circles->total; i++)
	{
		float* p = (float*)cvGetSeqElem(circles, i);

		circlelist << QPair<QPoint,int>(QPoint(p[0],p[1]),p[2]);
	}

	return(circlelist);
}

QList<QRect> MainWindow::detectObjects(IplImage *frame, CvHaarClassifierCascade* cascade)
{
	QList<QRect> rects;

	cvClearMemStorage(_storage);

	CvSeq *faces = cvHaarDetectObjects(frame, cascade, _storage, 1.1, 2, CV_HAAR_DO_CANNY_PRUNING, cvSize(20, 20));

	for (int i = 0; i < (faces ? faces->total : 0); i++)
	{
		CvRect* r = (CvRect*) cvGetSeqElem(faces, i);
		QRect rect(r->x, r->y, r->width, r->height);
		rects << rect;
	}

	return (rects);
}

QImage MainWindow::toImage(IplImage *cvimage)
{
	QImage image;
	int cvIndex, cvLineStart;
	// switch between bit depths
	switch (cvimage->depth)
	{
		case IPL_DEPTH_8U:
			switch (cvimage->nChannels)
			{
				case 3:
					if ((cvimage->width != image.width()) || (cvimage->height != image.height()))
					{
						QImage temp(cvimage->width, cvimage->height, QImage::Format_RGB32);
						image = temp;
					}
					cvIndex = 0;
					cvLineStart = 0;
					for (int y = 0; y < cvimage->height; y++)
					{
						unsigned char red, green, blue;
						cvIndex = cvLineStart;
						for (int x = 0; x < cvimage->width; x++)
						{
							// DO it
							red = cvimage->imageData[cvIndex+2];
							green = cvimage->imageData[cvIndex+1];
							blue = cvimage->imageData[cvIndex+0];

							image.setPixel(x, y, qRgb(red, green, blue));
							cvIndex += 3;
						}
						cvLineStart += cvimage->widthStep;
					}
					break;
				default:
					printf("This number of channels is not supported\n");
					break;
			}
			break;
		default:
			printf("This type of IplImage is not implemented in QOpenCVWidget\n");
			break;
	}

	return (image);
}
