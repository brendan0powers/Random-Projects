#ifndef __UTIL_H
#define __UTIL_H

#include <QList>
#include <QStringList>



class TickListener
{
	public:
	TickListener();
	~TickListener();
		
	virtual void tick()=0;
	static void runTick();
	
private:
	static QList<TickListener *> _list;
};


class ConsoleListener
{
public:
	ConsoleListener();
	~ConsoleListener();
		
	virtual void consoleCommand(QStringList &params)=0;
	static void runCommand(QStringList &params);
private:
	static QList<ConsoleListener *> _list;
};






#endif