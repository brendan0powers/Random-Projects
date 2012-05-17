#include "util.h"

QList<TickListener *> TickListener::_list;
QList<ConsoleListener *> ConsoleListener::_list;

/*****************************************************************
**** TickListener
*****************************************************************/

TickListener::TickListener()
{
	_list.append(this);
}

TickListener::~TickListener()
{
	_list.removeAll(this);
}


void TickListener::runTick()
{
	foreach(TickListener *t, _list)
	{
		t->tick();
	}
}


/*****************************************************************
**** TickListener
*****************************************************************/

ConsoleListener::ConsoleListener()
{
	_list.append(this);	
}

ConsoleListener::~ConsoleListener()
{
	_list.removeAll(this);
}

void ConsoleListener::runCommand(QStringList &params)
{
	foreach(ConsoleListener *c, _list)
	{
		c->consoleCommand(params);	
	}
}
