#include <QApplication>
#include "QWidgetMainWindow.h"
#include <iostream>
#include "QWidgetSplashScreen.h"
#include "QCrosswordView.h"

using namespace std;


int main(int argc, char *argv[])
{

	// ocultar la consola
//	#ifdef WIN32
	//	HWND hWnd = GetConsoleWindow();
	//	ShowWindow( hWnd, SW_HIDE );
//	#endif


	QApplication a(argc, argv);
	
	// abrir la ventana splash
    QWidgetSplashScreen splash(":/splashscreen");
    splash.show();
	
    a.processEvents();


	QWidgetMainWindow mainWindow;
	
	QFont f;
	QSize size;
	size.setHeight(55);
	size.setWidth(55);
	QIcon icon;
	splash.PaintText(QString("Iniciando sistema"),f,20, splash.height()-30, QColor( 0, 0, 0 ),Qt::AlignLeft);

	mainWindow.setWindowIcon(icon);
	mainWindow.setIconSize(size);
	mainWindow.show();

	splash.finish(&mainWindow);

	cout << endl ;
	cout << "=============================" << endl;
	cout << "F1: Toggle Full-Screen" << endl;
	cout << "=============================" << endl << endl;

	return a.exec();
}
