#include <QApplication>
#include "headphonectl.h"
#include "nullmode.h"
#include "vlcmode.h"
#include "radiomode.h"
#include "media.h"
#include <QX11Info>
#include <X11/Xlib.h>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

	Display* display = QX11Info::display();
	Window window = QX11Info::appRootWindow();
	Bool owner = True;
	int pointer = GrabModeAsync;
	int keyboard = GrabModeAsync;
	
	XGrabKey(display, 171, 0, window, owner, pointer, keyboard);
	XGrabKey(display, 171, Mod2Mask, window, owner, pointer, keyboard); // allow numlock
	XGrabKey(display, 172, 0, window, owner, pointer, keyboard);
	XGrabKey(display, 172, Mod2Mask, window, owner, pointer, keyboard); // allow numlock
	XGrabKey(display, 173, 0, window, owner, pointer, keyboard);
	XGrabKey(display, 173, Mod2Mask, window, owner, pointer, keyboard); // allow numlock
	XSync(display, False);
	
    headphonectl foo;
	foo.addMode("VLC",new VLCMode());
	foo.addMode("Radio",new RadioMode());
	foo.addMode("Media",new Media());
	foo.addMode("Null",new NullMode());
	
    return app.exec();
}
