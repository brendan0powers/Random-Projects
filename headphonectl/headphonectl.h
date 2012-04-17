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

	virtual bool eventFilter(QObject *obj, QEvent *event);
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
	
};

#endif // headphonectl_H
