#include "headphonectl.h"

#include <QTimer>
#include <iostream>
#include <QPixmap>
#include <QIcon>
#include <QApplication>
#include <QKeyEvent>
#include <QDebug>

#define KEY_LEFT 171
#define KEY_MIDDLE 172
#define KEY_RIGHT 173

headphonectl::headphonectl()
: QSystemTrayIcon(QPixmap("/usr/share/icons/oxygen/22x22/devices/audio-headset.png"))
{
	show();
	qApp->installEventFilter(this);

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


bool headphonectl::eventFilter(QObject* obj, QEvent* event)
{
	if((event->type() == QEvent::KeyPress) || (event->type() == QEvent::KeyRelease))
	{
		QKeyEvent *keyevent = static_cast<QKeyEvent *>(event);
		if(keyevent->isAutoRepeat()) return(false);

		//qDebug() << keyevent->nativeScanCode();

		if(keyevent->nativeScanCode() == KEY_LEFT)
		{
			if(event->type() == QEvent::KeyPress)
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
		else if(keyevent->nativeScanCode() == KEY_MIDDLE)
		{
			if(event->type() == QEvent::KeyPress)
			{
				_middle = true;
			}
			else
			{
				if(_middle) emit(middlePressed());
				_middle = false;
			}
		}
		else if(keyevent->nativeScanCode() == KEY_RIGHT)
		{
			if(event->type() == QEvent::KeyPress)
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
	else
	{
		return QObject::eventFilter(obj, event);
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
	system(qPrintable(QString("espeak -v en '%1' --stdout | aplay -- &").arg(name)));

	_mode = _modes[name];
	_mode->start();
}

#include "headphonectl.moc"
