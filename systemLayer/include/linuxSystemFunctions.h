#ifndef LINUXSYSTEMFUNCTIONS_H_
#define LINUXSYSTEMFUNCTIONS_H_

#include <cstdio>
#include <cerrno>
#include <error.h>
#include <cstdlib>
#include <sstream>
#include <sys/file.h>
#include <linux/input.h>

#ifdef USE_QT
	#include <qdir.h>
	#include <qurl.h>
	#include <qprinter.h>
	#include <qabstractprintdialog.h>
	#include <qpagesetupdialog.h>
	#include <qtextstream.h>
	#include <qgraphicssceneevent.h>	
	#include <qstyleoption.h>
	#include <QX11Info>
#endif

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>


#ifndef  EXE_GREP
# define EXE_GREP "/bin/grep"
#endif

#define COMMAND_STR_DEVICES    EXE_GREP " Name /proc/bus/input/devices | " EXE_GREP " -nE "
#define COMMAND_STR_DEVICES1 ( COMMAND_STR_DEVICES "'[Kk]eyboard|kbd'" )
#define COMMAND_STR_DEVICES2 ( COMMAND_STR_DEVICES "'HID'" )

#define COMMAND_MOUSE_DEVICES1 ( COMMAND_STR_DEVICES "'Mouse'" )
#define COMMAND_MOUSE_DEVICES2 ( COMMAND_STR_DEVICES "'HID'" )

#define INPUT_EVENT_PATH  "/dev/input/"  // standard path

// executes cmd and returns string ouput or "ERR" on pipe error
static std::string execute(const char* cmd)
{
	FILE* pipe = popen(cmd, "r");
	if (!pipe)
		error(EXIT_FAILURE, errno, "Pipe error");
	char buffer[128];
	std::string result = "";
	while(!feof(pipe))
		if(fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	pclose(pipe);
	return result;
}

static std::string determine_Keboard_input_device()
{
	// better be safe than sory: while running other programs, switch user to nobody
	setegid(65534); seteuid(65534);

	// extract input number from /proc/bus/input/devices (I don't know how to do it better. If you have an idea, please let me know.)
	std::string outputKeyBoard = execute(COMMAND_STR_DEVICES1);

	int indexKeyBoard = atoi(outputKeyBoard.c_str()) - 1;
	if (indexKeyBoard == -1)
	{
		outputKeyBoard = execute(COMMAND_STR_DEVICES2);
		indexKeyBoard = atoi(outputKeyBoard.c_str()) - 1;
	}
	if (indexKeyBoard == -1)
	{
		error(0, 0, "Couldn't determine keyboard device. :/");
		error(EXIT_FAILURE, 0, "Please post contents of your /proc/bus/input/devices file as a new bug report. Thanks!");
	}

	std::stringstream input_dev_keyBoard_index;
	input_dev_keyBoard_index << INPUT_EVENT_PATH;
	input_dev_keyBoard_index << "event";
	input_dev_keyBoard_index << indexKeyBoard;  // the correct input event # is (output - 1)

	std::string keyBoardString = input_dev_keyBoard_index.str();
	//args.keyBoard = const_cast<char*>(input_dev_keyBoard_index.str().c_str());  // const_cast safe because original isn't modified

	// now we reclaim those root privileges
	seteuid(0); setegid(0);

	return keyBoardString;
}

static std::string determine_mouse_input_device()
{
	// better be safe than sory: while running other programs, switch user to nobody
	setegid(65534); seteuid(65534);

	// extract input number from /proc/bus/input/devices (I don't know how to do it better. If you have an idea, please let me know.)
	std::string outputMouse = execute(COMMAND_MOUSE_DEVICES1);

	int indexMouse = atoi(outputMouse.c_str()) - 1;
	if (indexMouse == -1)
	{
		outputMouse = execute(COMMAND_MOUSE_DEVICES2);
		indexMouse = atoi(outputMouse.c_str()) - 1;
	}
	if (indexMouse == -1)
	{
		error(0, 0, "Couldn't determine keyboard device. :/");
		error(EXIT_FAILURE, 0, "Please post contents of your /proc/bus/input/devices file as a new bug report. Thanks!");
	}

	std::stringstream input_dev_mouse_index;
	input_dev_mouse_index << INPUT_EVENT_PATH;
	input_dev_mouse_index << "event";
	input_dev_mouse_index << indexMouse;  // the correct input event # is (output - 1)

	std::string mouseString = input_dev_mouse_index.str();
	//args.mouse = const_cast<char*>(input_dev_mouse_index.str().c_str());  // const_cast safe because original isn't modified

	// now we reclaim those root privileges
	seteuid(0); setegid(0);

	return mouseString;
}

static void mouseRightReleasse()
{
	Display *display = XOpenDisplay(NULL);//QX11Info::display();

	XEvent event;

	if(display == NULL)
	{
		fprintf(stderr, "Error opening Display !!!\n");
		exit(EXIT_FAILURE);
	}

	memset(&event, 0x00, sizeof(event));

	event.type = ButtonRelease;
	event.xbutton.button = Button2;
	event.xbutton.same_screen = true;
	event.xbutton.state = 0x100;

	XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);

	event.xbutton.subwindow = event.xbutton.window;

	while(event.xbutton.subwindow)
	{
		event.xbutton.window = event.xbutton.subwindow;

		XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
	}

	if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Error sending the event !!!\n");

	XFlush(display);

	XCloseDisplay(display);

}

static void mouseRightPress()
{
	Display *display = XOpenDisplay(NULL);//QX11Info::display();

	XEvent event;

	if(display == NULL)
	{
		fprintf(stderr, "Error opening Display !!!\n");
		exit(EXIT_FAILURE);
	}

	memset(&event, 0x00, sizeof(event));

	event.type = ButtonPress;
	event.xbutton.button = Button2;
	event.xbutton.same_screen = True;

	XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);

	event.xbutton.subwindow = event.xbutton.window;

	while(event.xbutton.subwindow)
	{
		event.xbutton.window = event.xbutton.subwindow;

		XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
	}

	if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Error sending the event !!!\n");

	XFlush(display);

	usleep(100000);

	XCloseDisplay(display);
}

static void mouseLeftReleasse()
{
	Display *display = XOpenDisplay(NULL);//QX11Info::display();

	XEvent event;

	if(display == NULL)
	{
		fprintf(stderr, "Error opening Display !!!\n");
		exit(EXIT_FAILURE);
	}

	memset(&event, 0x00, sizeof(event));

	event.type = ButtonRelease;
	event.xbutton.button = Button1;
	event.xbutton.same_screen = true;
	event.xbutton.state = 0x100;

	XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);

	event.xbutton.subwindow = event.xbutton.window;

	while(event.xbutton.subwindow)
	{
		event.xbutton.window = event.xbutton.subwindow;

		XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
	}

	if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Error sending the event !!!\n");

	XFlush(display);

	XCloseDisplay(display);

}

static void mouseLeftPress()
{
	Display *display = XOpenDisplay(NULL);//QX11Info::display();

	XEvent event;

	if(display == NULL)
	{
		fprintf(stderr, "Error opening Display !!!\n");
		exit(EXIT_FAILURE);
	}

	memset(&event, 0x00, sizeof(event));

	event.type = ButtonPress;
	event.xbutton.button = Button1;
	event.xbutton.same_screen = True;

	XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);

	event.xbutton.subwindow = event.xbutton.window;

	while(event.xbutton.subwindow)
	{
		event.xbutton.window = event.xbutton.subwindow;

		XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
	}

	if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Error sending the event !!!\n");

	XFlush(display);

	usleep(100000);

	XCloseDisplay(display);
}

// Function to create a keyboard event
static XKeyEvent createKeyEvent(Display *display, Window &win, Window &winRoot, bool press, int keycode, int modifiers)
{
	XKeyEvent event;
	//memset(&event, 0x00, sizeof(event));

	event.display     = display;
	event.window      = win;
	event.root        = winRoot;
	event.subwindow   = None;
	event.time        = CurrentTime;
	event.x           = 1;
	event.y           = 1;
	event.x_root      = 1;
	event.y_root      = 1;
	event.same_screen = True;
	event.keycode     = XKeysymToKeycode(display, keycode);
	event.state       = modifiers;

	if(press)
		event.type = KeyPress;
	else
		event.type = KeyRelease;

	XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.root, &event.window, &event.x_root, &event.y_root, &event.x, &event.y, &event.state);

	event.subwindow = event.window;
/*	while(event.subwindow)
	{
		event.window = event.subwindow;

		XQueryPointer(display, event.window, &event.root, &event.subwindow, &event.x_root, &event.y_root, &event.x, &event.y, &event.state);
	}
*/

	return event;
}

#define DELAY 10

static void sendkey(/*KeySym modifier, KeySym key*/)
{
	Display *display = XOpenDisplay(NULL);
	XTestFakeKeyEvent(display, XK_A, true , CurrentTime);
	XTestFakeKeyEvent(display, XK_A, false, CurrentTime);
/*	KeyCode modifiercode;
	KeyCode keycode;
	if (modifier != 0)
	{
		modifiercode = XKeysymToKeycode(display, modifier);
		XTestFakeKeyEvent(display, modifiercode, true, DELAY);
	}
	keycode = XKeysymToKeycode(display, key);
	XTestFakeKeyEvent(display, keycode, true, DELAY);
	XTestFakeKeyEvent(display, keycode, false, DELAY);
	if (modifier != 0)
		XTestFakeKeyEvent(display, modifiercode, false, DELAY);
	XFlush(display);
*/
}

static void functionKeyBoardTESTED()
{
	Display *dpy = XOpenDisplay(NULL);
	if (!dpy) return;
	unsigned int keycode;
	keycode = XKeysymToKeycode(dpy, XK_A);
	XTestFakeKeyEvent(dpy, keycode, true, 0);
	XTestFakeKeyEvent(dpy, keycode, false, 0);
	XCloseDisplay(dpy);
}

static void pageDown()
{
	functionKeyBoardTESTED();
//	sendkey(0, 'z');
/*	Display *display = QX11Info::display();//XOpenDisplay(NULL);

	if(display == NULL)
	{
		fprintf(stderr, "Error opening Display !!!\n");
		exit(EXIT_FAILURE);
	}

	// Get the root window for the current display.
	Window winRoot = XDefaultRootWindow(display);

	// Find the window which has the current keyboard focus.
	Window winFocus;
	int    revert;
	XGetInputFocus(display, &winFocus, &revert);

	XTestFakeKeyEvent(display, KEYCODE_ALT_LEFT, KEY_DOWN, CurrentTime);
	XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_A), KEYPRESS, 0);
/*	// A full list of available codes can be found in /usr/include/X11/keysymdef.h
	XKeyEvent event = createKeyEvent(display, winFocus, winRoot, true, XK_A, 0);
	// XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event);
	if(XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event) == 0) fprintf(stderr, "Error sending DOWN event !!!\n");
	//(XSendEvent(display, PointerWindow, True, 0xfff, (XEvent *)&event)
	event           = createKeyEvent(display, winFocus, winRoot, false, XK_A, 0);
	//XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event);
	if(XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event) == 0) fprintf(stderr, "Error sending UP event !!!\n");
*/
	//XFlush(display);

	//usleep(100000);

	//XCloseDisplay(display);
}

#endif /* LINUXSYSTEMFUNCTIONS_H_ */
