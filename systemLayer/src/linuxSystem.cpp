#ifndef WIN32

#include "linuxSystem.h"
#ifdef USE_QT
	#include <QTimer>
#endif

#include <iostream>

using namespace std;

LinuxSystem *mySelf;

void *keyBoardHookProc(void *param); /* the thread */
void *mouseHookProc(void *param); /* the thread */

LinuxSystem::LinuxSystem()
{
	mouseCaptionEnabled = false;
	controlPressed = false;
	altPressed = false;

	mySelf = this;
}

LinuxSystem::~LinuxSystem()
{
}

void LinuxSystem::setMouseCaptionEnabled(bool enabled)
{
/*	if (enabled)
	{
		// hook the keyBoard
		pthread_attr_t attr; // set of thread attributes

		// get the default attributes
		pthread_attr_init(&attr);
		// create the thread
		pthread_create(&tMouseID, &attr, mouseHookProc, NULL);

		// now wait for the thread to exit
		//pthread_join(tid,NULL);
	}
	else
	{
		// Unhook the keyBoard
		pthread_exit(&tMouseID);
	}
	mouseCaptionEnabled = enabled;
*/
}

bool LinuxSystem::isMouseCaptionEnabled()
{
	return mouseCaptionEnabled;
}

void LinuxSystem::setKeyBoardCaptionEnabled(bool enabled)
{
	if (enabled)
	{
		// hook the keyBoard
		pthread_attr_t attr; /* set of thread attributes */

		/* get the default attributes */
		pthread_attr_init(&attr);
		/* create the thread */
		pthread_create(&tKeyBoardID, &attr, keyBoardHookProc, NULL);

		/* now wait for the thread to exit */
		//pthread_join(tid,NULL);
	}
	else
	{
		// Unhook the keyBoard
		pthread_exit(&tKeyBoardID);
	}
	keyboardCaptionEnabled = enabled;
}

bool LinuxSystem::isKeyBoardCaptionEnabled()
{
	return keyboardCaptionEnabled;
}

void LinuxSystem::keyBoardPressedFunction(/*SystemLayer::KeyBoard*/int action, std::string key)
{
	std::cout << action << endl;
#ifdef USE_QT
	emit mySelf->keyBoardPressed(action, QString(key.c_str()));
#endif
}

void LinuxSystem::leftMouseButtonDownFunction(POINT p)
{
	#ifdef USE_QT
		emit mySelf->leftMouseButtonDown(p);
	#endif
}

void LinuxSystem::leftMouseButtonUpFunction(POINT p)
{
	#ifdef USE_QT
		emit mySelf->leftMouseButtonUp(p);
	#endif
}

void LinuxSystem::rightMouseButtonDownFunction(POINT p)
{
	#ifdef USE_QT
	emit mySelf->rightMouseButtonDown(p);
	#endif
}

void LinuxSystem::mousemovedFunction(POINT p)
{
	#ifdef USE_QT
	emit mySelf->mousemoved(p);
	#endif
}

void *keyBoardHookProc(void *param)
{
	std::string keyBoardString = determine_Keboard_input_device();
	char* keyBoard = const_cast<char*>(keyBoardString.c_str());  // const_cast safe because original isn't modified

	// open input device for reading
	int input_keyboard_fd = open(keyBoard, O_RDONLY);
	if (input_keyboard_fd == -1)
	{
		return NULL;
		//error(EXIT_FAILURE, errno, "Error opening input event device '%s'", args.keyBoard);
	}

	struct input_event event;
	bool ctrlPressed = false;
	bool altPressed = false;
	while (read(input_keyboard_fd, &event, sizeof(struct input_event)) > 0)
	{
	    if (event.type != EV_KEY) continue;  // keyboard events are always of type EV_KEY

	    switch(event.code)
	    {
			case KEY_R:
			{
				if(event.value && ctrlPressed && altPressed)
					LinuxSystem::keyBoardPressedFunction(ENABLEEYE, "");
				break;
			}
			case KEY_E:
			{
				if(event.value && ctrlPressed && altPressed)
					LinuxSystem::keyBoardPressedFunction(CLOSEAPP, "");
				break;
			}
			case KEY_LEFTCTRL://left or right, both are accepted
			case KEY_RIGHTCTRL:
			{
				if(event.value)
					ctrlPressed = true;
				else
					ctrlPressed = false;
				break;
			}
			case KEY_LEFTALT://left or right, both are accepted
			case KEY_RIGHTALT:
			{
				if(event.value)
					altPressed = true;
				else
					altPressed = false;
				break;
			}
			case KEY_SCROLLLOCK:
			{
				if(event.value)
					LinuxSystem::keyBoardPressedFunction(MOVESIM, "");
				break;
			}
			case KEY_NUMLOCK:
			{
				if(event.value)
					LinuxSystem::keyBoardPressedFunction(CLICKSIM, "");
				break;
			}
	    }

	}
	return NULL;
}

void *mouseHookProc(void *param)
{
	std::string mouseString = determine_mouse_input_device();
	char* mouse = const_cast<char*>(mouseString.c_str());  // const_cast safe because original isn't modified

	// open input device for reading
	int input_mouse_fd = open(/*"/dev/input/event3"*/mouse, O_RDONLY);
	if (input_mouse_fd == -1)
	{
		return NULL;
		//error(EXIT_FAILURE, errno, "Error opening input event device '%s'", args.mouse);
	}

	struct input_event eventMouse;
	while (read(input_mouse_fd, &eventMouse, sizeof(struct input_event)) > 0)
	{
	    if (eventMouse.type != EV_KEY) continue;  // keyboard events are always of type EV_KEY

	    POINT point = mySelf->getCursorPos();
	    switch(eventMouse.code)
	    {
			case BTN_RIGHT:
			{
				if(eventMouse.value)//Mouse Right Press
					LinuxSystem::rightMouseButtonDownFunction(point);
				/*else //Mouse Right Release*/
				break;
			}
			case BTN_LEFT:
			{
				if(eventMouse.value)//Mouse Left Press
					LinuxSystem::leftMouseButtonDownFunction(point);
				else //Mouse Left Release
					LinuxSystem::leftMouseButtonUpFunction(point);
				break;
			}
	    }
	}

	return NULL;
}
cv::Mat LinuxSystem::getPrintScreen()
{
	return cv::Mat();
}

void LinuxSystem::sendMouseMove(POINT p)
{
	setCursorPos(p);
	
}

void LinuxSystem::sendSingleLeftClick(POINT p)
{
	setCursorPos(p);
	mouseLeftPress();
	mouseLeftReleasse();
}

void LinuxSystem::sendDoubleLeftClick(POINT p)
{
	setCursorPos(p);
	mouseLeftPress();
	mouseLeftReleasse();
	mouseLeftPress();
	mouseLeftReleasse();
}

void LinuxSystem::sendSingleLeftDown(POINT p)
{
	setCursorPos(p);
	mouseLeftPress();
}

void LinuxSystem::sendSingleLeftUp(POINT p)
{
	setCursorPos(p);
	mouseLeftReleasse();
}

void LinuxSystem::sendSingleRightClick(POINT p)
{ 
	setCursorPos(p);
	mouseRightPress();
	mouseRightReleasse();
}

void LinuxSystem::sendDoubleRightClick(POINT p)
{
	setCursorPos(p);
	mouseRightPress();
	mouseRightReleasse();
	mouseRightPress();
	mouseRightReleasse();
}

void LinuxSystem::resizeDesktopArea(RECT rect)
{
}

void LinuxSystem::sendSingleRightDown(POINT p)
{
	setCursorPos(p);
	mouseRightPress();
}

void LinuxSystem::sendSingleRightUp(POINT p)
{
	setCursorPos(p);
	mouseRightReleasse();
}

void LinuxSystem::sendScrollUp(POINT p)
{
}

void LinuxSystem::sendScrollDown(POINT p)
{
}

void LinuxSystem::sendPageUp()
{
}

void LinuxSystem::sendPageDown()
{
	pageDown();
}

void LinuxSystem::sendProgramLeft()
{
}

void LinuxSystem::sendProgramRight()
{
}

void LinuxSystem::changeCursor()
{
}
unsigned long LinuxSystem::getTicks()
{
		timeval ts;
		gettimeofday(&ts,0);
		return (ts.tv_sec * 1000 + (ts.tv_usec / 1000));
}
double LinuxSystem::getTicksHR()
{
	timeval ts;
	double timeInMicroSec;
    gettimeofday(&ts, NULL);
    timeInMicroSec = (ts.tv_sec * 1000000.0) + ts.tv_usec;

	return timeInMicroSec;
}

//-- Soporte para voz TTS --//
void LinuxSystem::initTTS()
{
}
std::vector<std::string> LinuxSystem::getVoiceList()
{
	return std::vector<std::string>();
}
int LinuxSystem::changeVoice(int i)
{
	return -1;
}
void LinuxSystem::speak(std::string texto)
{

}
std::string LinuxSystem::time_stamp(std::string format)
{
	std::string ts;
	int pos;
		
	timeval te;
	char time_string[40];
	struct tm* ptm;
	
	gettimeofday(&te,0);
	ptm = localtime (&te.tv_sec);
	strftime (time_string, sizeof (time_string), format.c_str(), ptm);
	ts = time_string;

	
	return ts;

}

DWORD LinuxSystem::FindProcess( LPCTSTR lpcszFileName)
{
return 0;
}


void LinuxSystem::getScreenSize(int &rows, int &cols)
{
	Display* pdsp = NULL;
	Screen* pscr = NULL;

	rows=cols=0;

	pdsp = XOpenDisplay( NULL );
	if ( !pdsp ) {
		fprintf(stderr, "Failed to open default display.\n");
		return;
	}
	pscr = DefaultScreenOfDisplay( pdsp );
	if ( !pscr ) {
		fprintf(stderr, "Failed to obtain the default screen of given display.\n");
		return;
	}

	cols = pscr->width;
	rows = pscr->height;

	XCloseDisplay( pdsp );
	return;
}

POINT LinuxSystem::getCursorPos()
{
	int rootX, rootY;
#ifdef USE_QT
	QPoint p = QCursor::pos();
	rootX= p.x();
	rootY= p.y();
#else
	Display *dpy;
	Window root, child;
	int winX, winY;
	unsigned int mask;

	dpy = XOpenDisplay(NULL);
	XQueryPointer(dpy,DefaultRootWindow(dpy),&root,&child,
              &rootX,&rootY,&winX,&winY,&mask);
#endif
	return POINT(rootX,rootY);
}
int LinuxSystem::getdir (std::string dir, std::vector<std::string> &files)
{
		DIR *dp;
		struct dirent *dirp;
		if((dp  = opendir(dir.c_str())) == NULL) {
			cout << "Error(" << errno << ") opening " << dir << endl;
			return errno;
		}

		while ((dirp = readdir(dp)) != NULL) {
			files.push_back(string(dirp->d_name));
		}
		closedir(dp);

		return 0;
	
}
bool LinuxSystem::fexists(const char *filename)
{
	std::ifstream ifile(filename);
	return ifile;
}
std::string LinuxSystem::extension(std::string file)
{
	int pos = file.find_last_of('.');
	std::string ext = file.substr(pos+1,file.length());
	std::transform(ext.begin(), ext.end(), ext.begin(), (int (*)(int))std::tolower);
	return ext;
}

void LinuxSystem::setCursorPos(POINT p)
{

#ifdef USE_QT
	QCursor::setPos(QPoint(p.x,p.y));
#else
	Display *dpy;
	Window root_window;

	dpy = XOpenDisplay(0);
	root_window = XRootWindow(dpy, 0);
	XSelectInput(dpy, root_window, KeyReleaseMask);
	XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, 100, 100);
	XFlush(dpy); // Flushes the output buffer, therefore updates the cursor's position.
#endif	

}


#endif
