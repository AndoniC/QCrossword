#ifdef WIN32

#include "windowsSystem.h"
#include <psapi.h> 
#define MAX_PROCESSES 1024 

#include <iostream>

using namespace std;
using namespace systemLayer;
HHOOK WindowsSystem::keyBoardHook = NULL;
HHOOK WindowsSystem::mouseHook = NULL;
HINSTANCE WindowsSystem::hInstance = NULL;
WindowsSystem *mySelf;

WindowsSystem::WindowsSystem()
{
	mouseCaptionEnabled = false;
	controlPressed = false;
	altPressed = false;

	mySelf = this;

	hInstance = (HINSTANCE)GetModuleHandle(NULL);

	RegisterHotKey(NULL, 1, MOD_CONTROL, 0x4D || 0x6D);

	//initTTS();

	//-- cogemos la frecuencia en la creación de este objeto para el getTicksHR --//
	QueryPerformanceFrequency(&m_freq);

	mouse_callback=0;
	keyboard_callback=0;
	mouse_hook_already_added=keyboard_hook_already_added=0;
}

WindowsSystem::~WindowsSystem()
{
	// Unhook the keyboard
	if (keyBoardHook)
	{
		UnhookWindowsHookEx(keyBoardHook);
		keyBoardHook = NULL;
	}

	// Unhook the mouse
	if (mouseHook)
	{
		UnhookWindowsHookEx(mouseHook);
		mouseHook = NULL;
	}

#ifdef TTS_SUPPORT
	//Release objects
	m_cpVoice.Release ();
	
	::CoUninitialize();
#endif
}

static int scan2ascii(DWORD scancode, ushort* result)
{
   static HKL layout=GetKeyboardLayout(0);
   static uchar State[256];

   if (GetKeyboardState(State)==FALSE)
        return 0;
   UINT vk=MapVirtualKeyEx(scancode,1,layout);
   return ToAsciiEx(vk,scancode,State,result,0,layout);
}
// Record the keyboard messages
LRESULT CALLBACK WindowsSystem::keyBoardHookProc(int nCode,WPARAM wParam,LPARAM lParam)
{
 	int i=0;
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT) (lParam);
	char ch=0x00;
	systemLayer::K_EVENT ev;
	std::string trace;

	ushort result[3];
	scan2ascii(p->scanCode, result);
	ch=char(result[0]);
	
	trace.empty();
	trace += ch;
	
	
	if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) 
	{
		ev = systemLayer::K_EVENT::KEY_DOWN;
		std::cout << "Capturado KEYDOWN : ";
	}
	else if(wParam == WM_KEYUP)	
	{
		ev = systemLayer::K_EVENT::KEY_UP;
		std::cout << "Capturado KEYUP : ";
	}

	switch(p->vkCode)
	{
		case VK_UP:			ch = VIU_UPARROW;	trace = " uparrow ";	break;
		case VK_DOWN:		ch = VIU_DOWNARROW;	trace = " downarrow ";	break;
		case VK_LEFT:		ch = VIU_LEFTARROW;	trace = " leftarrow ";	break;
		case VK_RIGHT:		ch = VIU_RIGHTARROW;trace = " rightarrow ";	break;
		case VK_NUMLOCK:	ch = VIU_NUM;		trace = " num ";	break;
		case VK_SCROLL:		ch = VIU_PGUP;		trace = " pgup ";	break;
		case VK_ESCAPE:		ch = VIU_ESCAPE;	trace = " escape ";	break;
		case VK_LCONTROL:	ch = VIU_LCONTROL;	trace = " left control ";	break;
		case VK_RCONTROL:	ch = VIU_RCONTROL;	trace = " right control ";	break;
		case VK_LMENU:		ch = VIU_LALT;	trace = " left alt ";	break;
		case VK_RMENU:		ch = VIU_RALT;	trace = " right alt ";	break;
		
	}

	std::cout <<trace << std::endl;

	//-- ALT and Control up-down detection --//
	if((wParam == WM_SYSKEYDOWN || wParam == WM_KEYDOWN) && p->vkCode == VK_LCONTROL)	mySelf->controlPressed = true;
	if((wParam == WM_SYSKEYUP || wParam == WM_KEYUP) && p->vkCode == VK_LCONTROL)		mySelf->controlPressed = false;
	if((wParam == WM_SYSKEYDOWN || wParam == WM_KEYDOWN) && p->vkCode == VK_LMENU)		mySelf->altPressed = true;
	if((wParam == WM_SYSKEYUP || wParam == WM_KEYUP) && p->vkCode == VK_LMENU)			mySelf->altPressed = false;


	#ifdef USE_QT
	//	emit mySelf->keyBoardPressed(keyMode,key.c_str());
	#endif
	LRESULT Res=0;
	if (mySelf->keyboard_callback) Res = (*(mySelf->keyboard_callback))(ch,ev);

	if (!Res)
		 Res = CallNextHookEx(keyBoardHook,nCode,wParam,lParam);

	return Res;
}

// Record the mouse messages
LRESULT CALLBACK WindowsSystem::mouseHookProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	PKBDLLHOOKSTRUCT k = (PKBDLLHOOKSTRUCT)(lParam);
	POINT p;
	if(wParam == WM_RBUTTONDOWN)
	{
		GetCursorPos(&p); 
		#ifdef USE_QT
			QPoint point = QPoint(p.x,p.y);
			emit mySelf->rightMouseButtonDown(point);
		#endif
	}
	if(wParam == WM_LBUTTONDOWN)
	{
		GetCursorPos(&p);
		#ifdef USE_QT
			QPoint point = QPoint(p.x,p.y);
			emit mySelf->leftMouseButtonDown(point);
		#endif
	}
	if(wParam == WM_LBUTTONUP)
	{
		GetCursorPos(&p);
		#ifdef USE_QT
			QPoint point = QPoint(p.x,p.y);
			emit mySelf->leftMouseButtonUp(point);
		#endif
	}
	if(wParam == WM_MOUSEMOVE)
	{
		GetCursorPos(&p);
		#ifdef USE_QT
			QPoint point = QPoint(p.x,p.y);
			emit mySelf->mousemoved(point);
		#endif
	}
	if(wParam == WM_LBUTTONDBLCLK)
	{
		GetCursorPos(&p);
		#ifdef USE_QT
			QPoint point = QPoint(p.x,p.y);
			emit mySelf->leftMouseButtonDown(point);
		#endif
	}

	GetCursorPos(&p);
	LRESULT res=0;
	if (mySelf->mouse_callback) res=(*(mySelf->mouse_callback))(cv::Point(p.x,p.y),systemLayer::M_EVENT::LEFT_DOWN);

	if (!res) res = CallNextHookEx(mouseHook,nCode,wParam,lParam);
	return res;
}

void WindowsSystem::listenMouse(bool enabled,int (*callback)(cv::Point,systemLayer::M_EVENT))
{
	
	if (enabled)
	{
		if (mouse_hook_already_added) 
		{	std::cout << "removing Mouse Hook."<< std::endl;
			UnhookWindowsHookEx(mouseHook);
		}
		// hook the mouse
		std::cout << "adding Mouse Hook."<< std::endl;
		mouseHook    = SetWindowsHookEx(WH_MOUSE_LL,mouseHookProc,hInstance,0);
		mouse_callback = callback;
		mouse_hook_already_added=true;
	}
	else
	{
		// Unhook the mouse
		std::cout << "removing Mouse Hook."<< std::endl;
		UnhookWindowsHookEx(mouseHook);
		mouseHook = NULL;
		mouse_callback = NULL;
		mouse_hook_already_added=false;

	}
}

bool WindowsSystem::isMouseCaptureEnabled()
{
	return mouseCaptionEnabled;
}

void WindowsSystem::listenKeyboard(bool enabled,int (*callback)(int,systemLayer::K_EVENT))
{
	if (enabled)
	{
		if (keyboard_hook_already_added) 
		{	std::cout << "removing Keyboard Hook."<< std::endl;
			UnhookWindowsHookEx(mouseHook);
		}
		// hook the keyBoard
		std::cout << "adding Keyboard Hook."<< std::endl;
		keyBoardHook = SetWindowsHookEx(WH_KEYBOARD_LL,(HOOKPROC)keyBoardHookProc,hInstance,0);
		keyboard_callback = callback;
		keyboard_hook_already_added=true;
	}
	else
	{
		// Unhook the keyBoard
		std::cout << "removing Keyboard Hook."<< std::endl;
		UnhookWindowsHookEx(keyBoardHook);
		keyBoardHook = NULL;
		keyboard_callback=NULL;
		keyboard_hook_already_added=false;
	}
}

bool WindowsSystem::isKeyBoardCaptureEnabled()
{
	return keyboardCaptionEnabled;
}

cv::Mat WindowsSystem::getPrintScreen()
{
	HDC hdcScreen = GetDC(NULL);
	int screenHeight=GetDeviceCaps(hdcScreen,VERTRES);
	int screenWidth=GetDeviceCaps(hdcScreen,HORZRES);
	HDC hdcCompatible=CreateCompatibleDC(hdcScreen);
	HBITMAP hbmCompatible=CreateCompatibleBitmap(hdcScreen,screenWidth,screenHeight);
	SelectObject(hdcCompatible,hbmCompatible);


	bool bitBlt = BitBlt(hdcCompatible,0,0,screenWidth,screenHeight,hdcScreen,0,0,SRCCOPY);

	BITMAP bmp;
    GetObject(hbmCompatible,sizeof(BITMAP),&bmp);
    int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel/8;
    int depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;
    cv::Mat imgT(bmp.bmHeight, bmp.bmWidth, CV_MAKETYPE(depth, nChannels));
    memcpy(imgT.data,(char*)(bmp.bmBits),bmp.bmHeight*bmp.bmWidth*nChannels);
   
	//return QPixmap::fromWinHBITMAP(hbmCompatible);
	return imgT;
}

void WindowsSystem::sendMouseMove(POINT p)
{
	double fx, fy;

	//POINT po;
	//GetCursorPos(&po);
	//QPoint currPos = QPoint(po.x,po.y);//QCursor::pos();
	//mouse_event(MOUSEEVENTF_ABSOLUTE, /*p.x()-currPos.x()*/0 , /*p.y()-currPos.y()*/0 , 0, 0);
	//mouse_event(MOUSEEVENTF_MOVE, p.x()/*-currPos.x()*/ , p.y()/*-currPos.y()*/ , 0, 0);
	
	//-- for absolute postioning  (No va bien, en la columna 15 no va hacia la derecha si avanzas de uno en uno)--//
	/*double fScreenWidth    = ::GetSystemMetrics( SM_CXSCREEN )-1; 
	double fScreenHeight  = ::GetSystemMetrics( SM_CYSCREEN )-1; 
	fx = p.x*(65535.0f/fScreenWidth);
	fy = p.y*(65535.0f/fScreenHeight);*/

	//-- for relative positioning --//
	cv::Point p0 = getMousePos();
	fx = p.x-p0.x;
	fy = p.y-p0.y;

	INPUT minput;
	MOUSEINPUT mi;
		
	mi.dx=fx;
	mi.dy=fy;

	mi. mouseData=0;
	mi.dwFlags=MOUSEEVENTF_MOVE;//|MOUSEEVENTF_ABSOLUTE;
	mi.time=0;
	mi.dwExtraInfo=GetMessageExtraInfo();
	minput.type=INPUT_MOUSE;
	minput.mi=mi;	

	SendInput(  1,  &minput,  sizeof(minput));

	//double fScreenWidth    = ::GetSystemMetrics( SM_CXSCREEN )-1; 
	//double fScreenHeight  = ::GetSystemMetrics( SM_CYSCREEN )-1; 
	//mouse_move_event((int)(fScreenWidth*x),(int)(fScreenHeight*y));
	
	//QCursor::setPos(p);
}

void WindowsSystem::sendSingleLeftClick(POINT p)
{
	sendMouseMove(p);
	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, p.x, p.y, 0, 0);
}

void WindowsSystem::sendDoubleLeftClick(POINT p)
{
	sendSingleLeftClick(p);
	sendSingleLeftClick(p);
	//mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, p.x(), p.y(), 0, 0);
	//mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, p.x(), p.y(), 0, 0);
}
void WindowsSystem::sendSingleLeftDown(POINT p)
{
	sendMouseMove(p);
	mouse_event(MOUSEEVENTF_LEFTDOWN, p.x, p.y, 0, 0);
	
	#ifdef USE_QT
		emit mySelf->mousemoved(QPoint(p.x,p.y));
	#endif
}

void WindowsSystem::sendSingleLeftUp(POINT p)
{
	sendMouseMove(p);
	mouse_event(MOUSEEVENTF_LEFTUP, p.x, p.y, 0, 0);
}

void WindowsSystem::sendSingleRightClick(POINT p)
{ 
	sendMouseMove(p);
	mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP, p.x, p.y, 0, 0);
}

void WindowsSystem::sendDoubleRightClick(POINT p)
{
	sendSingleRightClick(p);
	sendSingleRightClick(p);
}

void WindowsSystem::resizeDesktopArea(RECT rect)
{
	RECT workarea;

	// Get the current work area
	SystemParametersInfo( SPI_GETWORKAREA, 0, &workarea, 0 );

	workarea.top    = rect.top;
	workarea.bottom = rect.bottom;
	workarea.left   = rect.left;
	workarea.right  = rect.right;


	// Set the new work area and broadcast the change to all running applications
	SystemParametersInfo( SPI_SETWORKAREA, 0, &workarea, SPIF_SENDCHANGE );

	//EnumWindows
	//FindWindow
	////RedrawWindow(
}

void WindowsSystem::sendSingleRightDown(POINT p)
{
	sendMouseMove(p);
	mouse_event(MOUSEEVENTF_RIGHTDOWN, p.x, p.y, 0, 0);
}

void WindowsSystem::sendSingleRightUp(POINT p)
{
	sendMouseMove(p);
	
	mouse_event(MOUSEEVENTF_RIGHTUP, p.x, p.y, 0, 0);
}

void WindowsSystem::sendScrollUp(POINT p)
{
	for(int i=0;i<5;i++)
	{
		keybd_event(VK_UP, 75, 0, NULL);
		keybd_event(VK_UP, 75, KEYEVENTF_KEYUP, NULL);
	//	mouse_event(MOUSEEVENTF_WHEEL, 0, 0, 120, 0);
	}
}

void WindowsSystem::sendScrollDown(POINT p)
{
	for(int i=0;i<5;i++)
	{
		keybd_event(VK_DOWN, 72, 0, NULL);
		keybd_event(VK_DOWN, 72, KEYEVENTF_KEYUP, NULL);
	//	mouse_event(MOUSEEVENTF_WHEEL, p.x(), p.y(), -120, 0);
	}
}

void WindowsSystem::sendPageUp()
{
	//PGUP
	keybd_event(VK_PRIOR , 0x21, 0, 0);
	keybd_event(VK_PRIOR ,0x21, KEYEVENTF_KEYUP,0);

	//// Simulate a key press
	//keybd_event( VK_PRIOR, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );

	//// Simulate a key release
	//keybd_event( VK_PRIOR, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
}

void WindowsSystem::sendPageDown()
{
	//PGDN
	keybd_event(VK_NEXT, 0x22, 0, 0);
	keybd_event(VK_NEXT,0x22, KEYEVENTF_KEYUP,0);

	//// Simulate a key press
	//keybd_event(VK_NEXT, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);

	//// Simulate a key release
	//keybd_event(VK_NEXT, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
}

void WindowsSystem::sendProgramLeft()
{
	//keybd_event(VK_MENU,0xb8,0 , 0); //Alt Press

	//keybd_event(VK_TAB,0x8f,0 , 0); // Tab Press

	//keybd_event(VK_TAB,0x8f, KEYEVENTF_KEYUP,0); // Tab Release

	//keybd_event(VK_MENU,0xb8,KEYEVENTF_KEYUP,0); // Alt Release
	keybd_event(VK_MENU,0xb8,0 , 0); //Alt Press

	keybd_event(VK_ESCAPE,0x01,0 , 0); // Tab Press

	keybd_event(VK_ESCAPE,0x81, KEYEVENTF_KEYUP,0); // Tab Release

	keybd_event(VK_MENU,0xb8,KEYEVENTF_KEYUP,0); // Alt Release
}

void WindowsSystem::sendProgramRight()
{
	keybd_event(VK_MENU,0xb8,0 , 0); //Alt Press

	keybd_event(VK_ESCAPE,0x01,0 , 0); // Tab Press

	keybd_event(VK_ESCAPE,0x81, KEYEVENTF_KEYUP,0); // Tab Release

	keybd_event(VK_MENU,0xb8,KEYEVENTF_KEYUP,0); // Alt Release
}

void WindowsSystem::changeCursor()
{
	////HBITMAP b = LoadImage(NULL, _T("c:\test.bmp"), IMAGE_BITMAP, 0, 0, IR_LOADFROMFILE); //returns a valid HBITMAP.

	////CString c = _T("c:\test.bmp");
	////HBITMAP b = LoadImage(NULL, c, IMAGE_BITMAP, 0, 0, IR_LOADFROMFILE); //returns NULL with GetLastError of ERROR_FILE_NOT_FOUND

	////CreateCursor(hInstance,

	//HCURSOR hCursor=LoadCursor(NULL,MAKEINTRESOURCE(OCR_HAND));
	//System::Windows::Forms::Cursor* cursor=new System::Windows::Forms::Cursor(hCursor);
	//pictureBoxLiveImage->Cursor=cursor;
}

DWORD WindowsSystem::FindProcess(__in_z LPCTSTR lpcszFileName) 
{ 
  LPDWORD lpdwProcessIds; 
  LPTSTR  lpszBaseName; 
  HANDLE  hProcess; 
  DWORD   i, cdwProcesses, dwProcessId = 0; 

  lpdwProcessIds = (LPDWORD)HeapAlloc(GetProcessHeap(), 0, MAX_PROCESSES*sizeof(DWORD)); 
  if (lpdwProcessIds != NULL) 
  { 
    if (EnumProcesses(lpdwProcessIds, MAX_PROCESSES*sizeof(DWORD), &cdwProcesses)) 
    { 
      lpszBaseName = (LPTSTR)HeapAlloc(GetProcessHeap(), 0, MAX_PATH*sizeof(TCHAR)); 
      if (lpszBaseName != NULL) 
      { 
        cdwProcesses /= sizeof(DWORD); 
        for (i = 0; i < cdwProcesses; i++) 
        { 
          hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, lpdwProcessIds[i]); 
          if (hProcess != NULL) 
          { 
            if (GetModuleBaseName(hProcess, NULL, lpszBaseName, MAX_PATH) > 0) 
            { 
              if (!lstrcmpi(lpszBaseName, lpcszFileName)) 
              { 
                dwProcessId = lpdwProcessIds[i]; 
                CloseHandle(hProcess); 
                break; 
              } 
            } 
            CloseHandle(hProcess); 
          } 
        } 
        HeapFree(GetProcessHeap(), 0, (LPVOID)lpszBaseName); 
      } 
    } 
    HeapFree(GetProcessHeap(), 0, (LPVOID)lpdwProcessIds); 
  } 
  return dwProcessId; 
}
void WindowsSystem::initTTS()
{
	#ifdef TTS_SUPPORT
		HRESULT hr;
		if (FAILED(::CoInitialize(NULL)))
		{	
			std::cout << "CoInitialize ha fallado." << std::endl;
			return;
		}
		//Create a SAPI Voice
		hr = m_cpVoice.CoCreateInstance( CLSID_SpVoice );

		//Set the audio format
		if(SUCCEEDED(hr))
		{
			hr = cAudioFmt.AssignFormat(SPSF_22kHz16BitMono);
		}
		else
		{
			std::cout << "CoCreateInstance fallado." << std::endl;
			return;
		}
		/*//Call SPBindToFile, a SAPI helper method,  to bind the audio stream to the file
		if(SUCCEEDED(hr))
		{

			hr = SPBindToFile( L"c:\\ttstemp.wav",  SPFM_CREATE_ALWAYS,
				&cpStream;, & cAudioFmt.FormatId(),cAudioFmt.WaveFormatExPtr() );
		}*/




		if ( SUCCEEDED( hr ) )
		{
			hr = SpCreateDefaultObjectFromCategoryId( SPCAT_AUDIOOUT, &m_cpOutAudio );
		}
		else
		{
			std::cout << "AssignFormat fallado." << std::endl;
			return;
		}

		if ( SUCCEEDED( hr ) )
		{
			// Set default voice data 
			changeVoice(0);
		}
		else
		{
			std::cout << "SpCreateDefaultObjectFromCategoryId fallado." << std::endl;
			return;
		}
	#else
		std::cout << "No se ha añadido soporte para TTS. Compile con la directiva TTS_SUPPORT." << std::endl;
	#endif

}

/////////////////////////////////////////////////////////////////
int WindowsSystem::changeVoice(int i)
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
	HRESULT         hr ;
	
	#ifdef TTS_SUPPORT
		
		GUID*           pguidAudioFormat = NULL;
		CComPtr <IEnumSpObjectTokens>	cpEnum;
		CComPtr <ISpObjectToken>	cpToken;

		//Enumerate voice tokens with attribute "Name=Microsoft Sam"
		
		//hr = SpEnumTokens(SPCAT_VOICES, L"Name=Microsoft Sam", NULL, &cpEnum);
		hr = SpEnumTokens(SPCAT_VOICES, NULL, NULL, &cpEnum);
		

		//Get the closest token
		if(SUCCEEDED(hr))
		{
			int cont=0;
			while((hr=cpEnum ->Next(1, &cpToken, NULL))==S_OK && cont<=i) {cont++;cpToken.Release();}

		}
	
		//set the voice
		if(SUCCEEDED(hr))
		{
			hr = m_cpVoice->SetVoice( cpToken);
		}
		else 
		{
			hr = SpEnumTokens(SPCAT_VOICES, NULL, NULL, &cpEnum);
			if ((hr=cpEnum ->Next(1, &cpToken, NULL))==S_OK)
			{
				hr = m_cpVoice->SetVoice( cpToken);
			}
		}

		//set the output to the default audio device
		if(SUCCEEDED(hr))
		{
			hr = m_cpVoice->SetOutput( NULL, TRUE );
		}
			    
		//Determine if it is the current voice
		CComPtr<ISpObjectToken> pOldToken;
		hr = m_cpVoice->GetVoice( &pOldToken );

		if (SUCCEEDED(hr))
		{
			if (pOldToken != cpToken)
			{        
				// Stop speaking. This is not necesary, for the next call to work,
				// but just to show that we are changing voices.
				hr = m_cpVoice->Speak( NULL, SPF_PURGEBEFORESPEAK, 0);
				// And set the new voice on the global voice object
				if (SUCCEEDED (hr) )
				{
					hr = m_cpVoice->SetVoice( cpToken );
				}
			}
		}
		cpToken.Release();
		cpEnum.Release();
	#else
		std::cout << "No se ha añadido soporte para TTS. Compile con la directiva TTS_SUPPORT." << std::endl;
	
	#endif
	return hr;
}
std::wstring ConvertFromUtf8ToUtf16(const std::string& str)
{
    std::wstring convertedString;
    int requiredSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, 0, 0);
    if(requiredSize > 0)
    {
        std::vector<wchar_t> buffer(requiredSize);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &buffer[0], requiredSize);
        convertedString.assign(buffer.begin(), buffer.end() - 1);
    }
 
    return convertedString;
}
void WindowsSystem::speak(std::string texto)
{
	if (texto.empty()) return;
	#ifdef TTS_SUPPORT
	if (m_cpVoice==0) std::cout << " ERROR:: No hay voz cargada.\n"<<std::endl;
		HRESULT         hr = S_OK;
				
		std::vector<wchar_t> buffer(texto.size());
		//widestr=ConvertFromUtf8ToUtf16(texto);
		std::locale loc("C");
	   // loc = std::locale(loc, new std::ctype_byname<char>("es_ES.utf16"));

	    std::use_facet< std::ctype<wchar_t> >(loc).widen(texto.data(), texto.data() + texto.size(), &buffer[0]);

		std::wstring widestr(&buffer[0], buffer.size());
		if (texto.find("<")>=0 && texto.find(">") >=0)
			hr = m_cpVoice->Speak( widestr.c_str(),  SPF_IS_XML|SPF_ASYNC, NULL );	
		else
			hr = m_cpVoice->Speak( widestr.c_str(),  SPF_ASYNC, NULL );	
	#else
		std::cout << "No se ha añadido soporte para TTS. Compile con la directiva TTS_SUPPORT." << std::endl;
	#endif
}
std::vector<std::string> WindowsSystem::getVoiceList()
{
	std::vector<std::string> voices_list;
	#ifdef TTS_SUPPORT
		HRESULT hr = S_OK;
		CComPtr <IEnumSpObjectTokens>	cpEnum;
	
		

		//Enumerate voice tokens with attribute "Name=Microsoft Sam"
	
		hr = SpEnumTokens(SPCAT_VOICES, NULL, NULL, &cpEnum);
	
		//Get the closest token
		if(SUCCEEDED(hr))
		{
			ULONG number; 
			cpEnum->GetCount(&number);
			std::cout << "Number of tts voices: " << number << std::endl;
	//		hr = cpEnum->Next(1, &cpToken;, NULL);
			CComPtr <ISpObjectToken>	cpToken;
			while (cpEnum ->Next(1, &cpToken, NULL) == S_OK)
			{
				CSpDynamicString namew;
				hr = SpGetDescription( cpToken, &namew );
				std::string name(namew.CopyToChar());
				voices_list.push_back(name);
				
				// Release the token itself  
				cpToken.Release();  
			}
		}
	
		cpEnum.Release();
			
	#else 
		std::cout << "No se ha añadido soporte para TTS. Compile con la directiva TTS_SUPPORT." << std::endl;
	#endif
	return voices_list;
}
unsigned long WindowsSystem::getTicks()
{
		return ::GetTickCount();
}
double WindowsSystem::getTicksHR(std::string uds)
{
	double timeInMicroSec;                 // starting time in micro-second
	LARGE_INTEGER frequency;  
	LARGE_INTEGER startCount; 
	startCount.QuadPart = 0;

	//-- Podemos volver a consultar la frecuencia para cada lectura o usar la que cogimos en la construcción del objeto para todas --//
	QueryPerformanceFrequency(&frequency);
	
	QueryPerformanceCounter(&startCount);

    if (uds.compare("s")==0) timeInMicroSec = startCount.QuadPart * (1.0 / frequency.QuadPart);
	else if (uds.compare("ms")==0) timeInMicroSec = startCount.QuadPart * (1000.0 / frequency.QuadPart);
	else timeInMicroSec = startCount.QuadPart * (1000000.0 / frequency.QuadPart);

	return timeInMicroSec;
}

std::string WindowsSystem::time_stamp(std::string format)
{
	std::string ts;
	int pos;

	time_t rawtime;
	struct tm * timeinfo;

	char buffer [150];

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	
	strftime (buffer,sizeof(buffer),format.c_str(),timeinfo);
	
	ts = buffer;

	return ts;

}

int WindowsSystem::sendKey( BYTE c)
{
	INPUT key[2];
	key[0].type = INPUT_KEYBOARD;
    key[0].ki.wVk = 0;
    key[0].ki.wScan = (WORD)c;
    key[0].ki.dwFlags = 0;// 0 for key pressed : KEYEVENTF_UNICODE for unicode
    key[0].ki.time = 0;
    key[0].ki.dwExtraInfo = GetMessageExtraInfo();

	key[1].type = INPUT_KEYBOARD;
    key[1].ki.wVk = 0;
    key[1].ki.wScan = (WORD)c;
    key[1].ki.dwFlags = KEYEVENTF_KEYUP;
    key[1].ki.time = 0;
    key[1].ki.dwExtraInfo = GetMessageExtraInfo();
	
	return SendInput( 2, key, sizeof( key ) );
}
//
//int WindowsSystem::SendKeystrokesToNotepad( const TCHAR *const text )
//{
//    INPUT *keystroke;
//    UINT i, character_count, keystrokes_to_send, keystrokes_sent;
//    HWND notepad;
//
//    assert( text != NULL );
//
//    //Get the handle of the Notepad window.
//    notepad = FindWindow( _T( "Notepad" ), NULL );
//    if( notepad == NULL )
//        return 0;
//
//    //Bring the Notepad window to the front.
//    if( !SetForegroundWindow( notepad ) )
//        return 0;
//
//    //Fill in the array of keystrokes to send.
//    character_count = _tcslen( text );
//    keystrokes_to_send = character_count * 2;
//    keystroke = new INPUT[ keystrokes_to_send * 2 ];
//    for( i = 0; i < character_count; ++i )
//    {
//        keystroke[ i * 2 ].type = INPUT_KEYBOARD;
//        keystroke[ i * 2 ].ki.wVk = 0;
//        keystroke[ i * 2 ].ki.wScan = text[ i ];
//        keystroke[ i * 2 ].ki.dwFlags = KEYEVENTF_UNICODE;
//        keystroke[ i * 2 ].ki.time = 0;
//        keystroke[ i * 2 ].ki.dwExtraInfo = GetMessageExtraInfo();
//
//        keystroke[ i * 2 + 1 ].type = INPUT_KEYBOARD;
//        keystroke[ i * 2 + 1 ].ki.wVk = 0;
//        keystroke[ i * 2 + 1 ].ki.wScan = text[ i ];
//        keystroke[ i * 2 + 1 ].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
//        keystroke[ i * 2 + 1 ].ki.time = 0;
//        keystroke[ i * 2 + 1 ].ki.dwExtraInfo = GetMessageExtraInfo();
//    }
//
//    //Send the keystrokes.
//    keystrokes_sent = SendInput( ( UINT )keystrokes_to_send, keystroke, sizeof( *keystroke ) );
//    delete [] keystroke;
//
//    return keystrokes_sent == keystrokes_to_send;
//}

int WindowsSystem::getdir (std::string dir, std::vector<std::string> &files,std::string filter)
{
	printf ("Looking %s data into %s directory\n",filter.c_str(),dir.c_str());
	
	if (dir.at(dir.length()-1) != '\\' && dir.at(dir.length()-1) != '/' ) dir = dir + "/";
	
	printf ("Looking %s data into %s directory\n",filter.c_str(),dir.c_str());
	WIN32_FIND_DATA data;
	HANDLE h;
	if (filter.empty()) h = FindFirstFile(std::string(dir +"*.*").c_str(),&data);
	else h = FindFirstFile(std::string(dir +filter).c_str(),&data);

	if( h!=INVALID_HANDLE_VALUE ) 
	{
		do
		{
			std::string fichero = data.cFileName;
			if (fichero == "." || fichero == "..") continue;
			DWORD dwAttrib = GetFileAttributes((dir + fichero).c_str());
			if (dwAttrib == 0xffffffff ) continue;
			DWORD isdir = dwAttrib & FILE_ATTRIBUTE_DIRECTORY;
			if ( isdir )
				getdir(dir  + fichero,files,filter);
			else 
			{
				printf("  Added %s file\n", std::string(dir + fichero).c_str());
				files.push_back(dir + fichero);
			}
			

		} while(FindNextFile(h,&data));
	} 
	else 
		std::cout << "Error: No such folder." << std::endl;

	FindClose(h);

	return 0;
}
bool WindowsSystem::fexists(const char *filename)
{
	std::ifstream ifile(filename);
	return ifile;
}
std::string WindowsSystem::extension(std::string file)
{
	int pos = file.find_last_of('.');
	std::string ext = file.substr(pos+1,file.length());
	std::transform(ext.begin(), ext.end(), ext.begin(),::tolower);
	return ext;
}



#endif