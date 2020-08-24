#ifdef WIN32

#ifndef WINDOWSSYSTEM_H
#define WINDOWSSYSTEM_H

//#define _WIN32_WINNT 0x0500

#ifdef TTS_SUPPORT
	#include <sapi.h>           // SAPI includes
	//#include <sphelper.h>
	#include <spuihelp.h>  // para el tts
#endif
#include "conio.h"
#include <time.h>
#include <iostream>
#include <algorithm>
#include "systemLayer.h"

#include "systemLayer_exp.h"

		class WindowsSystem : public SystemLayer
		{
			#ifdef USE_QT
				Q_OBJECT
			#endif
				//static WindowsSystem *mySelf;
		public:
			WindowsSystem();
			~WindowsSystem();
			//void setupSystem();

			//-- HOOK the system , capturing messages--//
			void listenMouse(bool enabled,int (*callback)(cv::Point,systemLayer::M_EVENT));
			bool isMouseCaptureEnabled();
			void listenKeyboard(bool enabled,int (*callback)(int,systemLayer::K_EVENT));
			bool isKeyBoardCaptureEnabled();

			cv::Mat getPrintScreen();

			//-- sending messages--//
			void sendMouseMove(POINT p);

			void sendSingleLeftClick(POINT p);
			void sendDoubleLeftClick(POINT p);
			void sendSingleLeftDown(POINT p);
			void sendSingleLeftUp(POINT p);

			void sendSingleRightClick(POINT p);
			void sendDoubleRightClick(POINT p);
			void sendSingleRightDown(POINT p);
			void sendSingleRightUp(POINT p);

			void sendScrollUp(POINT p);
			void sendScrollDown(POINT p);

			int sendKey(BYTE c);
			//void sendKeyStroke(std::string text);

			void sendPageUp();
			void sendPageDown();

			void sendProgramLeft();
			void sendProgramRight();

			void changeCursor();
			void resizeDesktopArea(RECT rect);
			static DWORD FindProcess(__in_z LPCTSTR lpcszFileName);

			unsigned long getTicks();
			double getTicksHR(std::string uds=std::string());
			
			cv::Point getMousePos(){ POINT p; GetCursorPos(&p); return cv::Point(p.x,p.y);};
			std::string time_stamp(std::string format);

			//-- Soporte para voz TTS --//
			void initTTS();
			std::vector<std::string> getVoiceList();
			int changeVoice(int i);
			void speak(std::string texto);

			void getScreenSize(int &rows, int &cols)
			{
				cols = GetSystemMetrics( SM_CXSCREEN);
				rows = GetSystemMetrics( SM_CYSCREEN);
			};

			int getdir (std::string dir, std::vector<std::string> &files,std::string filter=std::string());
			bool fexists(const char *filename);
			std::string extension(std::string file);

			LARGE_INTEGER m_freq;  
			bool controlPressed;
			bool altPressed;

		private:

			bool mouseCaptionEnabled;
			bool keyboardCaptionEnabled;

			

			static const int shotTime = 200;

			static HINSTANCE hInstance;
			static HHOOK keyBoardHook;
			static HHOOK mouseHook;
			static LRESULT CALLBACK keyBoardHookProc(int nCode,WPARAM wParam,LPARAM lParam);
			static LRESULT CALLBACK mouseHookProc(int nCode,WPARAM wParam,LPARAM lParam);

			int (*mouse_callback)(cv::Point,systemLayer::M_EVENT);
			int (*keyboard_callback)(int,systemLayer::K_EVENT);

			DWORD   dwMouseThreadIdArray;
			HANDLE  hMouseThreadArray; 
			DWORD   dwKeyBoardThreadIdArray;
			HANDLE  hKeyBoardThreadArray; 

			bool mouse_hook_already_added,keyboard_hook_already_added;


			


#ifdef TTS_SUPPORT
			CComPtr<ISpVoice>   m_cpVoice;
			CComPtr<ISpAudio>   m_cpOutAudio;
			CSpStreamFormat			cAudioFmt;
#endif
	
	
};



#endif // WINDOWSSYSTEM_H

#endif