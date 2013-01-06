/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "vlcmode.h"
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/extensions/XTest.h>
#include <unistd.h>

static void KeyDown (Display * disp, KeySym keysym, KeySym modsym)
{
    KeyCode keycode = 0, modcode = 0;
    keycode = XKeysymToKeycode (disp, keysym);
    if (keycode == 0) return;

    XTestGrabControl (disp, True);

    /* Generate modkey press */
    if (modsym != 0)
    {
        modcode = XKeysymToKeycode(disp, modsym);
        XTestFakeKeyEvent (disp, modcode, True, 0);
    }

    /* Generate regular key press and release */
    XTestFakeKeyEvent (disp, keycode, True, 0);

    /* Generate modkey release */
    if (modsym != 0)
        XTestFakeKeyEvent (disp, modcode, False, 0);

    XSync (disp, False);
    XTestGrabControl (disp, False);
}

static void KeyUp (Display * disp, KeySym keysym, KeySym modsym)
{
	KeyCode keycode = 0, modcode = 0;
	keycode = XKeysymToKeycode (disp, keysym);
	if (keycode == 0) return;
	
	XTestGrabControl (disp, True);
	
	/* Generate modkey press */
	if (modsym != 0)
	{
		modcode = XKeysymToKeycode(disp, modsym);
		XTestFakeKeyEvent (disp, modcode, True, 0);
	}
	
	/* Generate regular key press and release */
	XTestFakeKeyEvent (disp, keycode, False, 0);
	
	/* Generate modkey release */
	if (modsym != 0)
		XTestFakeKeyEvent (disp, modcode, False, 0);
	
	XSync (disp, False);
	XTestGrabControl (disp, False);
}

void VLCMode::left() //69
{
	system("qdbus org.mpris.MediaPlayer2.vlc /org/mpris/MediaPlayer2 org.mpris.MediaPlayer2.Player.Previous");
}

void VLCMode::middle() //68
{
	 system("qdbus org.mpris.MediaPlayer2.vlc /org/mpris/MediaPlayer2 org.mpris.MediaPlayer2.Player.PlayPause");
}

void VLCMode::right() //70
{
	 system("qdbus org.mpris.MediaPlayer2.vlc /org/mpris/MediaPlayer2 org.mpris.MediaPlayer2.Player.Next");
}

