#ifndef __GAMESTATE_H
#define __GAMESTATE_H

#include "netobjectbase.h"
#include "util.h"

class GameState : public NetObjectBase, public TickListener, public ConsoleListener
{
DECLARE_NETVARIABLES(GameState,NetObjectBase,Map);
public:	
	GameState();
	~GameState();
	
	void tick();
	void consoleCommand(QStringList &cmd);
	
	bool onGhostAdd(TNL::GhostConnection *theConnection);
	
private:
	int _tmp;
	QString _map;
};

#endif