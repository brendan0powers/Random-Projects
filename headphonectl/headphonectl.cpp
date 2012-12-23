#include "headphonectl.h"

#include <QTimer>
#include <iostream>
#include <QPixmap>
#include <QIcon>
#include <QApplication>
#include <QKeyEvent>
#include <QDebug>
#include <QAbstractEventDispatcher>
#include <QX11Info>
#include <X11/Xlib.h>

#define KEY_LEFT 171
#define KEY_MIDDLE 172
#define KEY_RIGHT 173

headphonectl *headphonectl::_instance = NULL;

headphonectl::headphonectl()
: QSystemTrayIcon(QPixmap("/usr/share/icons/oxygen/22x22/devices/audio-headset.png"))
{
	show();
	_instance = this;
	//qApp->installEventFilter(this);

	QAbstractEventDispatcher::instance()->setEventFilter(eventFilter);

	_left = false;
	_middle = false;
	_right = false;
	_curmode = 0;
}

headphonectl::~headphonectl()
{
}

void headphonectl::addMode(QString name, Mode* mode)
{
	if(_modes.size() == 0) _mode = mode;
	_modes[name] = mode;
}


bool headphonectl::eventFilter(void *message)
{
	return _instance->instanceEventFilter(message);
}

bool headphonectl::instanceEventFilter(void *message)
{
	XEvent* event = static_cast<XEvent*>(message);
	XKeyEvent* keyevent = (XKeyEvent*) event;
	if((event->type == KeyPress) || (event->type == KeyRelease))
	{
		qDebug() << "Key: " << keyevent->keycode;

		if(keyevent->keycode == KEY_LEFT)
		{
			if(event->type == KeyPress)
			{
				_left = true;
				checkModeChange();
			}
			else
			{
				if(_left) emit(leftPressed());
				_left = false;
			}
		}
		else if(keyevent->keycode == KEY_MIDDLE)
		{
			if(event->type == KeyPress)
			{
				_middle = true;
			}
			else
			{
				if(_middle) emit(middlePressed());
				_middle = false;
			}
		}
		else if(keyevent->keycode == KEY_RIGHT)
		{
			if(event->type == KeyPress)
			{
				_right = true;
				checkModeChange();
			}
			else
			{
				if(_right) emit(rightPressed());
				_right = false;
			}
		}
	}
}

void headphonectl::checkModeChange()
{
	if(_left && _right)
	{	
		emit(changeMode());
		_left = false;
		_right = false;
	}
}

void headphonectl::leftPressed()
{
	_mode->left();
}

void headphonectl::middlePressed()
{
	_mode->middle();
}

void headphonectl::rightPressed()
{
	_mode->right();
}

void headphonectl::changeMode()
{
	qDebug() << "Mode Changed";
	_mode->end();
	_curmode++;
	if(_curmode >= _modes.size()) _curmode = 0;

	QString name = _modes.keys()[_curmode];
	qDebug() << qPrintable(QString("espeak -v en '%1' --stdout | aplay -- &").arg(name));
	system(qPrintable(QString("espeak -v en '%1' --stdout | aplay -- &").arg(name)));

	_mode = _modes[name];
	_mode->start();
}

#include "headphonectl.moc"
