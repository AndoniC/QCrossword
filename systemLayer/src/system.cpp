
#include "system.h"

using namespace std;
using namespace systemLayer;

#ifdef WIN32
		WindowsSystem m_system;
#else
		LinuxSystem m_system;
#endif

System::System()
{
	
}

System::~System()
{
}
#ifdef USE_QT
cv::Mat System::getPrintScreen()
{
	return m_system.getPrintScreen();
}
#endif

void System::sendMouseEvent(M_EVENT e,  cv::Point p)
{
	POINT paux;
	paux.x = p.x;
	paux.y = p.y;
	switch (e)
	{
	case MOUSE_MOVE:
		{
			m_system.sendMouseMove(paux);
			break;
		}
	case LEFT_CLICK: 
		{
			m_system.sendSingleLeftClick(paux);
			break;
		}
	case RIGHT_CLICK: 
		{
			m_system.sendSingleRightClick(paux);
			break;
		}
	case LEFT_DOUBLE_CLICK: 
		{
			m_system.sendDoubleLeftClick(paux);
			break;
		}
	case RIGHT_DOUBLE_CLICK: 
		{
			m_system.sendDoubleRightClick(paux);
			break;
		}
	case LEFT_DOWN: 
		{
			m_system.sendSingleLeftDown(paux);
			break;
		}
	case LEFT_UP: 
		{
			m_system.sendSingleLeftUp(paux);
			break;
		}
	case RIGHT_DOWN: 
		{
			m_system.sendSingleRightDown(paux);
			break;
		}
	case RIGHT_UP: 
		{
			m_system.sendSingleRightUp(paux);
			break;
		}
	case SCROLL_UP: 
		{
			m_system.sendScrollUp(paux);
			break;
		}
	case SCROLL_DOWN: 
		{
			m_system.sendScrollDown(paux);
			break;
		}
	default:
		{
			
				break;
		}
	};

}


void System::resizeDesktopArea(RECT rect)
{
	m_system.resizeDesktopArea(rect);
}

void System::sendKeyboardEvent(K_EVENT e,BYTE  c)
{
	switch (e)
	{
	case KEY:
		{
			//m_system.sendKey(c);
			break;
		}
	case PAGEDOWN:
		{
			m_system.sendPageDown();
			break;
		}
	case PAGEUP:
		{
			m_system.sendPageUp();
			break;
		}
	case PROGRAM_LEFT:
		{
			m_system.sendProgramLeft();
			break;
		}
	case PROGRAM_RIGHT:
		{
			m_system.sendProgramRight();
			break;
		}
	case KEY_SCROLL_UP:
		{
		//	m_system.sendScrollUp();
			break;
		}
	case KEY_SCROLL_DOWN:
		{
		//	m_system.sendScrollDown();
			break;
		}
	default:
		{
			break;
		}
	};

}


DWORD System::FindProcess(LPCTSTR lpcszFileName) 
{ 
	return m_system.FindProcess(lpcszFileName);
}

/////////////////////////////////////////////////////////////////
int System::changeVoice(int i)
/////////////////////////////////////////////////////////////////
//
// This function is called during initialization and whenever the 
// selection for the voice combo box changes. 
// It gets the token pointer associated with the voice.
// If the new voice is different from the one that's currently 
// selected, it first stops any synthesis that is going on and
// sets the new voice on the global voice object. 
//
{
	return m_system.changeVoice(i);
}
void System::speak(std::string texto)
{
	std::cout << "Speaking : " << texto << std::endl;
	 m_system.speak(texto);
}
void System::initTTS()
{
	 m_system.initTTS();
}

std::vector<std::string> System::getVoiceList()
{
	return m_system.getVoiceList();
}
unsigned long System::getTicks()
{
		return m_system.getTicks();
}
double System::getTicksHR()
{
	return m_system.getTicksHR("us");
}
std::string System::time_stamp(std::string format)
{
	return m_system.time_stamp(format);

}
void System::getScreenSize(int &rows, int &cols)
{
	m_system.getScreenSize(rows,cols);
}

int System::getdir (std::string dir, std::vector<std::string> &files,std::string filter)
{
	
	return m_system.getdir(dir,files,filter);
}
bool System::fexists(const char *filename)
{
	return m_system.fexists(filename);
}
std::string System::extension(std::string file)
{
	return m_system.extension(file);
}
void System::listenMouse(bool enabled,int (*callback)(cv::Point,M_EVENT))
{
	m_system.listenMouse(enabled,callback);
}
void System::listenKeyboard(bool enabled,int (*callback)(int,K_EVENT))
{
	m_system.listenKeyboard(enabled,callback);
}
bool System::isAltPressed()
{
	return m_system.altPressed;
}
bool System::isCtrlPressed()
{
	return m_system.controlPressed;
}
cv::Point System::getMousePos(){ return m_system.getMousePos();}