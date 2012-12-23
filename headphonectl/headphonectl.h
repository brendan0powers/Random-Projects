#ifndef headphonectl_H
#define headphonectl_H

#include <QSystemTrayIcon>
#include <QHash>
#include <QStringList>
#include "mode.h"

class headphonectl : public QSystemTrayIcon
{
Q_OBJECT
public:
    headphonectl();
    virtual ~headphonectl();

	static bool eventFilter(void *message);
	bool instanceEventFilter(void *message);
	void addMode(QString name, Mode *mode);
	
private slots:
	

private:
	void checkModeChange();
	void leftPressed();
	void middlePressed();
	void rightPressed();
	void changeMode();
	
	bool _left;
	bool _middle;
	bool _right;
	int _curmode;
	QHash<QString, Mode *> _modes;
	Mode *_mode;

	static headphonectl *_instance;
	
};

#endif // headphonectl_H
