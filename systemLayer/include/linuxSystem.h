#ifndef LINUXSYSTEM_H
#define LINUXSYSTEM_H

#ifndef WIN32

#include <pthread.h>
#include <linux/types.h>

#ifdef USE_QT
	#include <QObject>
#endif

#include "systemLayer.h"
#include "linuxSystemFunctions.h"

//
//	int kbhit (void)
//	{
//	  struct timeval tv;
//	  fd_set rdfs;
//	 
//	  tv.tv_sec = 0;
//	  tv.tv_usec = 0;
//	 
//	  FD_ZERO(&rdfs);
//	  FD_SET (STDIN_FILENO, &rdfs);
//	 
//	  select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
//	  return FD_ISSET(STDIN_FILENO, &rdfs);
//		 
//	};
//	void changemode(int dir)
//	{
//	  static struct termios oldt, newt;
//	 
//	  if ( dir == 1 )
//	  {
//		tcgetattr( STDIN_FILENO, &oldt);
//		newt = oldt;
//		newt.c_lflag &= ~( ICANON | ECHO );
//		tcsetattr( STDIN_FILENO, TCSANOW, &newt);
//	  }
//	  else
//		tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
//	};
//
	class LinuxSystem : public SystemLayer
	{
		#ifdef USE_QT
			Q_OBJECT
		#endif

		

	public:
		LinuxSystem();
		~LinuxSystem();
		//void setupSystem();

		POINT getCursorPos();
		void setCursorPos(POINT p);

		void setMouseCaptionEnabled(bool enabled);
		bool isMouseCaptionEnabled();
		void setKeyBoardCaptionEnabled(bool enabled);
		bool isKeyBoardCaptionEnabled();
		cv::Mat getPrintScreen();
		

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

		void sendPageUp();
		void sendPageDown();

		void sendProgramLeft();
		void sendProgramRight();

		void changeCursor();
		void resizeDesktopArea(RECT rect);

		static void keyBoardPressedFunction(/*SystemLayer::KeyBoard*/int action, std::string key);

		static void leftMouseButtonDownFunction(POINT);
		static void leftMouseButtonUpFunction(POINT);
		static void rightMouseButtonDownFunction(POINT);
		static void mousemovedFunction(POINT);

		DWORD FindProcess( LPCTSTR lpcszFileName);

		unsigned long getTicks();
		double getTicksHR();
		std::string time_stamp(std::string format);

		//-- Soporte para voz TTS --//
		void initTTS();
		std::vector<std::string> getVoiceList();
		int changeVoice(int i);
		void speak(std::string texto);

		void getScreenSize(int &rows, int &cols);

		int getdir (std::string dir, std::vector<std::string> &files);
		bool fexists(const char *filename);
		std::string extension(std::string file);


	private:

		bool mouseCaptionEnabled;
		bool keyboardCaptionEnabled;

		bool controlPressed;
		bool altPressed;

		pthread_t tKeyBoardID; /* the thread identifier */
		pthread_t tMouseID; /* the thread identifier */

		static const int shotTime = 200;

	


};


#endif
#endif // LINUXSYSTEM_H
