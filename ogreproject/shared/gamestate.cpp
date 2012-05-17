#include "gamestate.h"
#include <QDebug>
#include "entity.h"


GameState::GameState()
{
	_tmp=0;
	_map="WTFHAX";
}

START_IMPLEMENT_NETVARIABLES(GameState);
IMPLEMENT_NETVARIABLE(Map, _map);
END_IMPLEMENT_NETVARIABLES;

GameState::~GameState()
{
	
}

void GameState::tick()
{
	
}

void GameState::consoleCommand(QStringList &cmd)
{
	if(cmd.size() == 0) return;
	if(cmd[0] == "spawn")
	{
		if(cmd.size() < 2) return;
		cmd.removeAt(0);
		QString name = cmd[0];
		cmd.removeAt(0);
		QHash<QString,QString> data;
		foreach(QString arg, cmd)
		{
			int index = arg.indexOf("=");
			if(index < 0) continue;
			QString left = arg.left(index).trimmed();
			QString right = arg.mid(index+1).trimmed();
			data[left]=right;
		}
		
		Entity *entity = Entity::spawn(qPrintable(name),data);
	}
}

bool GameState::onGhostAdd(TNL::GhostConnection *theConnection)
{
	if(!Parent::onGhostAdd(theConnection))
		return false; //A parent had an error
	
	
	qDebug() << "********************* ---- GAME STATE CREATED";
}