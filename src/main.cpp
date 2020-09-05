#include <QApplication>
#include "QWidgetMainWindow.h"
#include <iostream>
#include "QWidgetSplashScreen.h"
#include "QCrosswordView.h"
#include "ext/ext_parser.h"

using namespace std;

std::vector<std::string> keys =
{ "{help h usage ? |			| print this message				}",
"{config         |			| json file								}",
"{log_verbosity  |   INFO		| verbosity level						}",
"{verbosity v    |   0		| verbosity level						}",
"{log            |			| output log file						}",
"{output o       |			| output folder or video file			}" };

int main(int argc, char *argv[])
{

	// ocultar la consola
//	#ifdef WIN32
	//	HWND hWnd = GetConsoleWindow();
	//	ShowWindow( hWnd, SW_HIDE );
//	#endif
	ext::parser::parser  parser(argc, argv, keys);

	std::string verbosity_level_str, operative_log_file;
	int verbosity;
	if (parser.has("log_verbosity"))
	{
		verbosity_level_str = parser.get<std::string>("log_verbosity");
	}
	if (parser.has("v"))
	{
		verbosity = parser.get<int>("v");
	}
	if (parser.has("log"))
	{
		operative_log_file = parser.get<std::string>("log");
	}

	if (parser.has("help"))
	{
		parser.printMessage();
		return 0;
	}

	QApplication a(argc, argv);
	
	OperativeLog::open(operative_log_file, verbosity_level_str, (!operative_log_file.empty()) ? true : false);
	LOG_SCOPE_FUNCTION(INFO);


	//{
		//LOG_SCOPE_F(INFO, "Info Scope: ...");
		//if (!set_res)	LOG_F(WARNING, "(Loading default values)...");
		//LOG_F(INFO, "N strips: %d", jsl.m_n_pattern_strips);
		//LOG_F(INFO, "Strip length : %d mm", jsl.m_strip_height_mm);
		//LOG_F(INFO, "Gap length : %d mm", jsl.m_gap_height_mm);
	//}

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
