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

#include "Ogre.h"
#include <QDebug>
#include "testconnection.h"
#include "util.h"
#include "gamestate.h"

RefPtr<NetInterface> interface;

#define MAX_TICKS 30

class Logger: public LogConsumer
{
	public:
		void logString(const char *string)
		{
			qDebug() << "TNL:" << string;
		}

};

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"


int main(int argc, char **argv);

INT WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR command_line, INT show_command)
{
	int    argc;
	char** argv;

	char*  arg;
	int    index;
	int    result;



	// count the arguments

	argc = 1;
	arg  = command_line;

	while (arg[0] != 0)
	{

		while (arg[0] != 0 && arg[0] == ' ')
		{
			arg++;
		}

		if (arg[0] != 0)
		{

			argc++;

			while (arg[0] != 0 && arg[0] != ' ')
			{
				arg++;
			}

		}

	}

	// tokenize the arguments

	argv = (char**)malloc(argc * sizeof(char*));

	arg = command_line;
	index = 1;

	while (arg[0] != 0)
	{

		while (arg[0] != 0 && arg[0] == ' ')
		{
			arg++;
		}

		if (arg[0] != 0)
		{

			argv[index] = arg;
			index++;

			while (arg[0] != 0 && arg[0] != ' ')
			{
				arg++;
			}

			if (arg[0] != 0)
			{
				arg[0] = 0;
				arg++;
			}

		}

	}

	// put the program name into argv[0]

	char filename[_MAX_PATH];

	GetModuleFileNameA(NULL, filename, _MAX_PATH);
	argv[0] = filename;

	// call the user specified main function

	result = main(argc, argv);

	free(argv);
	return result;

}

#endif

void tick()
{
        interface->checkIncomingPackets();
        interface->processConnections();
	TickListener::runTick();
}

int main(int argc, char **argv)
{
	//enable logging
	/*
	TNLLogEnable(LogConnectionProtocol,true);
	TNLLogEnable(LogNetConnection,true);
	TNLLogEnable(LogEventConnection,true);
	TNLLogEnable(LogGhostConnection,true);
	TNLLogEnable(LogNetInterface,true);
	TNLLogEnable(LogPlatform,true);
	*/
	
	Logger log;

	TestConnection *conn = new TestConnection;
	if (argc > 1)
	{
	    Address listenAddr(argv[1]);
        interface = new NetInterface(listenAddr);
    	}
	else
	{
		Address listenAddr("0.0.0.0:12000");
		interface = new NetInterface(listenAddr);
	}
	
	GameState gs;
	
    	// notify the NetInterface that it can allow connections
	interface->setAllowsConnections(true);
	U32 lastTick = 0;
	U32 curTick = 0;
	U32 tickLimit = (1000 / MAX_TICKS);
    while(true)
    {
        curTick = Platform::getRealMilliseconds();
        if((curTick - lastTick) < tickLimit)
        {
            Platform::sleep(tickLimit - (curTick - lastTick));
        }
        tick();
        lastTick = curTick;
    }

	return 0;
}
