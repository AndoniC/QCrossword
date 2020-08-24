#pragma once 
#ifndef SYSTEMLAYER_H
#define SYSTEMLAYER_H

#ifdef WIN32
	#include <windows.h>
#else
	#include <stdio.h>
	#include <sys/time.h>
	#include <time.h>
	#include <unistd.h>
	#include <termios.h>
	#include <X11/Xlib.h>
	#include <dirent.h>
#endif

#ifdef USE_QT
	#include <QObject>
	#include <QPoint>
#endif

#include "opencv2/core/core.hpp"
#include <iostream>
#include <fstream>
#include "systemLayer_exp.h"
									
#define		VIU_F1			 0xCE					
#define		VIU_F2			 0xCF				
#define		VIU_F3			 0xD0				
#define		VIU_F4			 0xD1				
#define		VIU_F5			 0xD2				
#define		VIU_F6			 0xD3				
#define		VIU_F7			 0xD4				
#define		VIU_F8			 0xD5				
#define		VIU_F9			 0xD6				
#define		VIU_F10			 0xD7				
#define		VIU_F11			 0xD8				
#define		VIU_F12			 0xD9
#define		VIU_LCONTROL	 0xDA		
#define		VIU_LALT		 0xDB		
#define		VIU_RCONTROL	 0xDC		
#define		VIU_RALT		 0xDD		
#define		VIU_LSHIFT		 0xDE		
#define		VIU_RSHIFT		 0xDF		
#define		VIU_SPACE		 0xE0		
#define		VIU_ENTER		 0xE1		
#define		VIU_ESCAPE		 0xE2		
#define		VIU_INSERT		 0xE3
#define		VIU_BKSP		 0xE4
#define		VIU_TAB			 0xE5
#define		VIU_HOME		 0xE6
#define		VIU_PGUP		 0xE7
#define		VIU_DELETE		 0xE8
#define		VIU_END			 0xE9
#define		VIU_PDDOWN		 0xEA
#define		VIU_UPARROW		 0xEB
#define		VIU_LEFTARROW	 0xEC
#define		VIU_DOWNARROW	 0xED
#define		VIU_RIGHTARROW	 0xEE
#define		VIU_NUM			 0xEF
#define		VIU_KPDIV		 0xF0
#define		VIU_KPMUL		 0xF1
#define		VIU_KPSUB		 0xF2
#define		VIU_KPADD		 0xF3
#define		VIU_KPEN		 0xF4
#define		VIU_KPDOT		 0xF5
#define		VIU_KP0			 0xF6
#define		VIU_KP1			 0xF7
#define		VIU_KP2			 0xF8
#define		VIU_KP3			 0xF9
#define		VIU_KP4			 0xFA
#define		VIU_KP5			 0xFB
#define		VIU_KP6			 0xFC
#define		VIU_KP7			 0xFD
#define		VIU_KP8			 0xFE
#define		VIU_KP9			 0xFF

namespace systemLayer {

	typedef enum  M_EVENT {LEFT_CLICK = 0, RIGHT_CLICK = 1, LEFT_DOUBLE_CLICK = 2, RIGHT_DOUBLE_CLICK = 3,MOUSE_MOVE = 4, LEFT_DOWN = 5,LEFT_UP=6, RIGHT_DOWN=7, RIGHT_UP=8,SCROLL_UP=9, SCROLL_DOWN = 10};
	typedef enum  K_EVENT {KEY = 0, KEY_DOWN = 1, KEY_UP = 2, PAGEDOWN = 3, PAGEUP = 4, PROGRAM_LEFT = 5,PROGRAM_RIGHT = 6, KEY_SCROLL_UP=7, KEY_SCROLL_DOWN = 8};
	
//	enum SL_LIB vkEVENT {KEY_DOWN = 0, KEY_UP = 1};
//	enum SL_LIB vmEVENT {MOUSE_LEFT_DOWN = 0, MOUSE_LEFT_UP = 1, MOUSE_RIGHT_DOWN=2, MOUSE_RIGHT_UP=3,MOUSE_MOVE=4};

//	enum KeyBoard{CLOSEAPP = 0, ENABLEEYE = 1, KEY = 2, CLICKSIM = 3, MOVESIM = 4, MOVEUP = 5, MOVEDOWN = 6, MOVELEFT = 7, MOVERIGHT = 8, ESC = 9, NO_KEY = -1};
};

	#ifndef WIN32
		typedef struct POINT
		{
			int x;
			int y;
			POINT(){x=0;y=0;};
			POINT(int _x, int _y) {x = _x; y = _y;};
		}POINT;

		typedef struct RECT
		{
			int x;
			int y;
			int widht;
			int height;
			RECT(){x=0;y=0;widht=0;height=0;};
			RECT(int _x, int _y, int _widht, int _height){x=_x;y=_y;widht=_widht;height=_height;};
		}RECT;
		typedef unsigned char BYTE;
		typedef unsigned long DWORD;
		typedef  const char *LPCSTR;
		typedef LPCSTR LPCTSTR; 
		typedef uint64_t LARGE_INTEGER;
	#endif

	#ifdef USE_QT
		class SL_LIB SystemLayer : public QObject
	#else
		class SL_LIB SystemLayer
	#endif
		{
		#ifdef USE_QT
			Q_OBJECT
		#endif

		public:
			SystemLayer(){};
			virtual ~SystemLayer(){};

			//-- HOOK THE SYSTEM --//
			virtual void listenMouse(bool enabled,int (*callback)(cv::Point,systemLayer::M_EVENT))=0;
			virtual bool isMouseCaptureEnabled()=0;
			virtual void listenKeyboard(bool enabled,int (*callback)(int,systemLayer::K_EVENT))=0;
			virtual bool isKeyBoardCaptureEnabled()=0;


			virtual cv::Mat getPrintScreen()=0;

			virtual void sendMouseMove(POINT p)=0;

			virtual void sendSingleLeftClick(POINT p)=0;
			virtual void sendDoubleLeftClick(POINT p)=0;
			virtual void sendSingleLeftDown(POINT p)=0;
			virtual void sendSingleLeftUp(POINT p)=0;

			virtual void sendSingleRightClick(POINT p)=0;
			virtual void sendDoubleRightClick(POINT p)=0;
			virtual void sendSingleRightDown(POINT p)=0;
			virtual void sendSingleRightUp(POINT p)=0;

			virtual void sendScrollUp(POINT p)=0;
			virtual void sendScrollDown(POINT p)=0;

			virtual void sendPageUp()=0;
			virtual void sendPageDown()=0;

			virtual void sendProgramLeft()=0;
			virtual void sendProgramRight()=0;

			virtual void resizeDesktopArea(RECT rect)=0;

			#ifndef WIN32
				int kbhit (void)
				{
				  struct timeval tv;
				  fd_set rdfs;
				 
				  tv.tv_sec = 0;
				  tv.tv_usec = 0;
				 
				  FD_ZERO(&rdfs);
				  FD_SET (STDIN_FILENO, &rdfs);
				 
				  select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
				  return FD_ISSET(STDIN_FILENO, &rdfs);
				 
				};
				void changemode(int dir)
				{
				  static struct termios oldt, newt;
				 
				  if ( dir == 1 )
				  {
					tcgetattr( STDIN_FILENO, &oldt);
					newt = oldt;
					newt.c_lflag &= ~( ICANON | ECHO );
					tcsetattr( STDIN_FILENO, TCSANOW, &newt);
				  }
				  else
					tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
				};
			#endif


#ifdef USE_QT
		signals:
			void leftMouseButtonDown(QPoint);
			void leftMouseButtonUp(QPoint);
			void rightMouseButtonDown(QPoint);
			void mousemoved(QPoint);
	//		void keyBoardPressed(KeyBoard, QString);
#endif
	
	
};
#endif // SYSTEMLAYER_H
