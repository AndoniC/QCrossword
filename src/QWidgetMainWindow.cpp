#include <QApplication>
#include <QDesktopWidget>
#include "QWidgetMainWindow.h"
#include <iostream>
#include "ext/ext_fs.h"
#include "DataManager.h"


QWidgetMainWindow::QWidgetMainWindow( QWidget *parent)
	: QMainWindow(parent)
{
	//ui.setupUi(this);

	QRect lastScreenRect = QApplication::desktop()->screenGeometry();
	m_screen_width=(double)lastScreenRect.width();
	m_screen_height=(double)lastScreenRect.height();
	setWindowTitle(tr("CrossWord"));
	
	//-- poner el fondo en gris --/
	QPalette pal = this->palette();
	pal.setColor(this->backgroundRole(), Qt::gray);
	this->setPalette(pal);


//	m_widget_crossword=NULL;
	m_rows=m_cols=0;

//	m_currentSquare=NULL;
//	m_current_advance_direction=NONE_DIR;
	m_statusLeft = m_statusState = m_statusMiddle=m_statusRight = NULL;
	createStatusBar();
	createActions();
	createMenu();
	
	//connect(&m_info_widget,SIGNAL(update(TSquareInfo &)),this, SLOT(sltUpdate(TSquareInfo &)));
	connect(&m_rowscolswidget,SIGNAL(update(int ,int )),this, SLOT(sltUpdate(int , int )));
	connect(&m_zoomWidget,SIGNAL(press(int , int , int )),this, SLOT(sltDefPressed(int , int , int )));
	
	resize(1024, 768);	
	m_new=false;
	
	m_fullScreen_flag=false;
	
	this->setCentralWidget(&m_crossword_viewer);
}

QWidgetMainWindow::~QWidgetMainWindow()
{
//	if (m_widget_crossword!=NULL) delete m_widget_crossword;
	delete m_acNew;
	delete m_acLoad;
	delete m_acSave;
	delete m_acSolution;
	delete m_acPlayMode;
	delete m_acEditionMode;
	delete m_acExit;
	

}

void QWidgetMainWindow::createActions()
{
	
	m_acNew = new QAction(QIcon(":/new"),QObject::tr("&New Crossword"), this);
    m_acNew->setShortcut(QObject::tr("Ctrl+D"));
    connect(m_acNew, SIGNAL(triggered()), this, SLOT(acNew()));

	m_acLoadDir = new QAction(QIcon(":/load"),QObject::tr("&Load Crossword Directory"), this);
    m_acLoadDir->setShortcut(QObject::tr("Ctrl+D"));
    connect(m_acLoadDir, SIGNAL(triggered()), this, SLOT(acLoadDir()));


	m_acLoad = new QAction(QIcon(":/load"),QObject::tr("&Load Crossword"), this);
    m_acLoad->setShortcut(QObject::tr("Ctrl+D"));
    connect(m_acLoad, SIGNAL(triggered()), this, SLOT(acLoad()));

	m_acSave = new QAction(QIcon(":/save"),QObject::tr("&Save Crossword"), this);
    m_acSave->setShortcut(QObject::tr("Ctrl+D"));
    connect(m_acSave, SIGNAL(triggered()), this, SLOT(acSave()));

	m_acSolution = new QAction(QIcon(":/solution"),QObject::tr("&Solution"), this);
	m_acSolution->setShortcut(QObject::tr("Ctrl+V"));
    connect(m_acSolution, SIGNAL(triggered()), this, SLOT(acSolution()));


	m_acExit = new QAction(QIcon(":/exit"),QObject::tr("&Exit"), this);
	m_acExit->setShortcut(QObject::tr("Ctrl+X"));
    connect(m_acExit, SIGNAL(triggered()), this, SLOT(acExit()));


	m_acPlayMode = new QAction(/*QIcon(":/exit"),*/QObject::tr("&Play Mode"), this);
	m_acPlayMode->setShortcut(QObject::tr("Ctrl+P"));
	m_acPlayMode ->setCheckable(true);
    connect(m_acPlayMode, SIGNAL(triggered()), this, SLOT(acPlayMode()));

	m_acEditionMode = new QAction(/*QIcon(":/exit"),*/QObject::tr("&Edition Mode"), this);
	m_acEditionMode->setShortcut(QObject::tr("Ctrl+P"));
	m_acEditionMode ->setCheckable(true);
    connect(m_acEditionMode, SIGNAL(triggered()), this, SLOT(acEditionMode()));

	m_acSolutionMode = new QAction(/*QIcon(":/exit"),*/QObject::tr("&Solution Mode"), this);
	m_acSolutionMode->setShortcut(QObject::tr("Ctrl+P"));
	m_acSolutionMode->setCheckable(true);
	connect(m_acSolutionMode, SIGNAL(triggered()), this, SLOT(acSolutionMode()));

	m_acWithClues = new QAction(/*QIcon(":/exit"),*/QObject::tr("&Clues Activation"), this);
	m_acWithClues->setShortcut(QObject::tr("Ctrl+P"));
	m_acWithClues->setCheckable(true);
	connect(m_acWithClues, SIGNAL(triggered()), this, SLOT(acWithClues()));

	
	m_acClear = new QAction(QIcon(":/exit"),QObject::tr("&Clear Crossword"), this);
	m_acClear->setShortcut(QObject::tr("Ctrl+P"));
    connect(m_acClear, SIGNAL(triggered()), this, SLOT(acClear()));

	m_acCheck = new QAction(QIcon(":/exit"),QObject::tr("&Check Crossword"), this);
	m_acCheck->setShortcut(QObject::tr("Ctrl+P"));
	m_acCheck ->setCheckable(true);
    connect(m_acCheck, SIGNAL(triggered()), this, SLOT(acCheck()));

	m_acPreviousCrossword = new QAction(QIcon(":/previous"),QObject::tr("&Previous Crossword"), this);
	m_acPreviousCrossword->setShortcut(QObject::tr("Ctrl+P"));
	m_acPreviousCrossword ->setCheckable(true);
    connect(m_acPreviousCrossword, SIGNAL(triggered()), this, SLOT(acPreviousCrossword()));

	m_acNextCrossword = new QAction(QIcon(":/next"),QObject::tr("&Next Crossword"), this);
	m_acNextCrossword->setShortcut(QObject::tr("Ctrl+P"));
	m_acNextCrossword ->setCheckable(true);
    connect(m_acNextCrossword, SIGNAL(triggered()), this, SLOT(acNextCrossword()));

	QActionGroup *m_viewGroup;
	m_viewGroup = new QActionGroup(this);
	m_viewGroup->addAction(m_acPlayMode);
	m_viewGroup->addAction(m_acEditionMode);
		
	
}

void QWidgetMainWindow::createMenu()
{
	//m_menuBar = new QMenuBar(0);
	m_menuBar = this->menuBar();
	QMenu *fileMenu = m_menuBar->addMenu(tr("File"));
	QMenu *toolsMenu = m_menuBar->addMenu(tr("Tools")); 
	QMenu *viewMenu = m_menuBar->addMenu(tr("View"));
	m_menuBar->addSeparator();
	m_menuBar->addAction(m_acCheck);
	m_menuBar->addSeparator();
	m_menuBar->addAction(m_acClear);
	m_menuBar->addSeparator();
	m_menuBar->addAction(m_acPreviousCrossword);
	m_menuBar->addAction(m_acNextCrossword);
	QMenu *exitMenu = m_menuBar->addMenu(tr("Exit"));
	
		
	//fileMenu->addMenu("&New");
	//fileMenu->addMenu("Load");
	//fileMenu->addSeparator();
	fileMenu->addAction(m_acNew);
	fileMenu->addSeparator();
	fileMenu->addAction(m_acLoad);
	fileMenu->addAction(m_acSave);
	fileMenu->addSeparator();
	fileMenu->addAction(m_acLoadDir);
	fileMenu->addSeparator();
	fileMenu->addAction(m_acExit);
//	fileMenu->addMenu("Save");
	viewMenu->addAction(m_acPlayMode);
	viewMenu->addAction(m_acEditionMode);
	viewMenu->addSeparator();
	viewMenu->addAction(m_acWithClues);



	toolsMenu->addAction(m_acSolution);
	m_acPlayMode->setChecked(true);
	acPlayMode();
}

void QWidgetMainWindow::createStatusBar()
{
	if (m_statusLeft) delete m_statusLeft;
	m_statusLeft = new QLabel("Left", this);
	//m_statusLeft->setFrameStyle(QFrame::Panel );

	if (m_statusMiddle) delete m_statusMiddle;
	m_statusMiddle = new QLabel("Middle", this);
	m_statusMiddle->setFrameStyle(QFrame::Panel );

	if (m_statusRight) delete m_statusRight;
	m_statusRight = new QLabel("Right", this);
	m_statusRight->setFrameStyle(QFrame::Panel );

	if (m_statusState) delete m_statusState;
	m_statusState = new QLabel("State", this);
	m_statusState->setFrameStyle(QFrame::Panel | QFrame::Sunken);


	statusBar()->addPermanentWidget(m_statusLeft, 1);
	statusBar()->addPermanentWidget(m_statusState, 0);
	statusBar()->addPermanentWidget(m_statusMiddle, 0);
	statusBar()->addPermanentWidget(m_statusRight, 0);

	statusBar()->setStyleSheet("QStatusBar{padding-left:8px;background:rgba(220,220,220,255);color:black;font-weight:bold;}");

	m_statusLeft->setText("Left");
	m_statusState->setText("State");
	m_statusMiddle->setText("Middle");
	m_statusRight->setText("Right");

	//if (m_showStatus->isChecked())
		this->statusBar()->show();
	//else
	//	this->statusBar()->hide();

}
void QWidgetMainWindow::connectAllSquares()
{

	/*for (int i=0;i<m_widget_crossword->rows();i++)
	{
		for (int j=0;j<m_widget_crossword->cols();j++)
		{
			if (m_widget_crossword->getSquare(i,j))
			{
				connect(m_widget_crossword->getSquare(i,j),SIGNAL(press(int ,int ,TSquareInfo &,int )),this,SLOT(sltSquarePressed(int ,int ,TSquareInfo &,int)));
				connect(m_widget_crossword->getSquare(i,j),SIGNAL(over(int ,int ,TSquareInfo & )),this,SLOT(sltSquareOver(int ,int ,TSquareInfo &)));
			}
			else 
				printf("         - can not connect null square %d,%d\n",i,j);
		}
	}*/
}
void QWidgetMainWindow::disconnectAllSquares()
{
	//printf("       * Desconectando señales press y over de los squares...\n");
	//for (int i=0;i<m_widget_crossword->rows();i++)
	//{
	//	for (int j=0;j<m_widget_crossword->cols();j++)
	//	{
	//		if (m_widget_crossword->getSquare(i,j))
	//		{
	//			disconnect(m_widget_crossword->getSquare(i,j),SIGNAL(press(int ,int ,TSquareInfo &,int)),this,SLOT(sltSquarePressed(int ,int ,TSquareInfo &,int)));
	//			disconnect(m_widget_crossword->getSquare(i,j),SIGNAL(over(int ,int ,TSquareInfo &)),this,SLOT(sltSquareOver(int ,int ,TSquareInfo &)));
	//		}
	//	}
	//}
}

void QWidgetMainWindow::resizegrid()
{
	//if (m_widget_crossword==NULL) return;
	//double marginy=0.0,marginx=0.0;
	//double square_size=0.0;
	//calculateSquareSize(m_rows, m_cols, m_grid_sizey, m_grid_sizex, marginy, marginx,square_size);

	//m_widget_crossword->hide();
	////-- posicionamos el widget con el grid --//
	//printf("Setting crossword geometry(x,y,cols,rows): (%.2lf,%.2lf,%d,%d)\n", marginx,marginy,m_grid_sizex, m_grid_sizey);
	//m_widget_crossword->setGeometry(marginx,marginy,m_grid_sizex, m_grid_sizey);
	//m_widget_crossword->resizeGridSquares(square_size);
	////-- No hace falta destruirlo y volver a crearlo al hacer resize, se puede redimensionar el grid redimensinando el widget padre --//
	////disconnectAllSquares();
	////m_widget_crossword->createGrid(20,20,square_size);
	////connectAllSquares();
	//m_widget_crossword->show();
}


void QWidgetMainWindow::calculateSquareSize(int rows, int cols, int &height, int &width, double &marginy, double &marginx,double &square_size)
{
	if (rows == 0 || cols==0) return;
	//-- Calcular el tamaño de las celdas --//

	int w = this->width();
	//-- Hay que quitar la barra de menúes , distribuiremos el grid en el espacio que hay desde el final de la barra hasta el final de la ventana--//
	int h = this->height() - 20;

	//-- tamaño de casillas en pixeles  (20 casillas)--//
	double square_width = w/m_cols;
	double square_height = h/m_rows;
	square_size = (square_width>square_height)?square_height:square_width;
	
	//-- tamaño del grid--//
	m_grid_sizex = square_size*cols;
	m_grid_sizey = square_size*rows;
	//-- x inicial e y inicial del grid--//
	marginx = (w - m_grid_sizex)/2;
	marginy = 20 + ( h - m_grid_sizey)/2;


}
int QWidgetMainWindow::load(QString file_name)
{
	
	printf("\n\nLOADING CROSSWORD\n");
	QString filename;
	if (file_name.isEmpty())
	{
		filename = QFileDialog::getOpenFileName(this,"Open json crossword" , last_dir.c_str(),
			tr("json files (*.json);;All files (*.*)"));
	}
	else filename = file_name;

	last_dir = ext::mix::path(filename.toLocal8Bit().data());

	QFile file(filename);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
	    std::cout << "   Error: Cannot read file " << qPrintable(filename)
	     << ": " << qPrintable(file.errorString())
	     << std::endl;
		return 0;
	}

	// Create data and retrieve index in the array of crosswords
	int idx = DataManager::load(filename.toLocal8Bit().data());

	// Create graphical view of crossword idx
	m_crossword_viewer.createCrosswordinScene(idx,40);
	


	return 1;
	//// version antigua



	//if (m_widget_crossword !=NULL) 
	//{
	//	m_widget_crossword->hide();

	//	printf("   Eliminando crossword anterior. \n");
	//	disconnectAllSquares(); 
	//	printf("       * Borrando Vista crossword...\n");
	//	//delete m_widget_crossword;
	//	m_widget_crossword->deleteView();
	//	printf("       * Borrando Datos crossword...\n");
	//	//m_widget_crossword->deleteData();
	//}
	//else m_widget_crossword = new QWidgetCrossword(this);
	//
	////-- Cargamos los datos en m_grid_info --//
	////-- Se vinculan los DEFS --//
	////-- Se rellena m_rows y m_cols de m_widget_crossword--//
	////QByteArray ba = file_name.toLocal8Bit();
	////const char* sz = ba.data();
	//m_widget_crossword->load(file_name);


	////-- Creamos la vista del Autodefinido --//
	//m_rows = m_widget_crossword->rows();
	//m_cols =  m_widget_crossword->cols();

	//double marginy=0.0,marginx=0.0,square_size=0.0;
	//calculateSquareSize(m_rows, m_cols, m_grid_sizey, m_grid_sizex,marginy, marginx,square_size);
	//
	//printf("   Setting crossword geometry: (%.2lf,%.2lf,%d,%d)\n", marginx,marginy,m_grid_sizex, m_grid_sizey);
	////-- posicionamos el widget con el grid --//
	////m_widget_crossword->createGrid(m_rows,m_cols,square_size);
	//m_widget_crossword->resizeGridSquares(square_size);
	//m_widget_crossword->setGeometry(marginx,marginy,m_grid_sizex, m_grid_sizey);
	//
	////-- Change the state of the QSquares --//
	//m_widget_crossword->changeState(m_acEditionMode->isChecked()?EDITION:PLAY);
	//printf("   Cambiando estado a: %s\n", m_acEditionMode->isChecked()?"EDITION":"PLAY");
	//
	//m_widget_crossword->show();
	//
	//printf("   Conectando Squares\n");
	//connectAllSquares();

	//return 1;

}

//-- ACTIONS --//
void QWidgetMainWindow::acNew()
{
	//-- Pedir el número de casillas rellena m_rows y m_cols --//
	m_rowscolswidget.exec();

	if (m_rows == 0 || m_cols==0) return;
	//-- Calcular el tamaño de las celdas --//
	


	//Delete Scene Tiles
	m_crossword_viewer.scene()->clear();

	// Delete Data
	DataManager::clear();


	//Create New Crossword
	// Create data and retrieve index in the array of crosswords
	int idx = DataManager::createCrossword(m_cols, m_rows);
	// Create graphical view of crossword idx
	m_crossword_viewer.createCrosswordinScene(idx, 40);


	//if (m_widget_crossword !=NULL) 
	//{
	//	m_widget_crossword->hide();
	//	printf("   Eliminando crossword anterior. \n");
	//	disconnectAllSquares(); 
	//	printf("       * Borrando Vista crossword...\n");
	//	//delete m_widget_crossword;
	//	m_widget_crossword->deleteView();
	//	printf("       * Borrando Datos crossword...\n");
	//	//m_widget_crossword->deleteData();
	//}
	//else	m_widget_crossword = new QWidgetCrossword(this);

	//m_crosswords_paths_list.clear();
	//m_current_crossword_index=-1;

	//double marginy=0.0,marginx=0.0,square_size=0.0;
	//calculateSquareSize(m_rows, m_cols, m_grid_sizey, m_grid_sizex,marginy, marginx,square_size);
	//	
	//printf("Setting crossword geometry: (%.2lf,%.2lf,%d,%d)\n", marginx,marginy,m_grid_sizex, m_grid_sizey);
	////-- posicionamos el widget con el grid --//
	//m_widget_crossword->setGeometry(marginx,marginy,m_grid_sizex, m_grid_sizey);
	//
	//m_widget_crossword->create(m_rows,m_cols,square_size);
	//
	//m_widget_crossword->changeState(m_acEditionMode->isChecked()?EDITION:PLAY);
	//
	//m_widget_crossword->show();
	////setCentralWidget(m_widget_crossword);
	//
	//connectAllSquares();
	//m_new = true;
}
void QWidgetMainWindow::acLoadDir()
{
	

	QFileDialog *dlg = new QFileDialog(); 
	QDir dir = dlg->getExistingDirectory(this, tr("Choose folder"), qgetenv("HOME"));
	
	QByteArray ba = dir.path().toLocal8Bit();
	const char *s = ba.data();
	std::string crosswords_directory (s);

	if (!crosswords_directory.empty())
	{
		m_crosswords_paths_list.clear();
		m_current_crossword_index=-1;
		
		ext::fs::getdir (crosswords_directory, m_crosswords_paths_list,"*.xml");
		if (m_crosswords_paths_list.size() >0) 
		{
			m_current_crossword_index=0;
			
			const char *sz = m_crosswords_paths_list[m_current_crossword_index].c_str();
			QString file_name(sz);
			//QByteArray ba = file_name.toLocal8Bit();
			//const char *sz2 = ba.data();
			//printf("LLLLLLLLLL   %s\n",sz2);
			load(file_name);
		}
	
	}
}
void QWidgetMainWindow::acLoad()
{
	DataManager::clear();
	m_crossword_viewer.scene()->clear();

	int ret=load(QString());
	if (ret) 
	{
		m_crosswords_paths_list.clear();
		m_current_crossword_index=-1;
	}
}
void QWidgetMainWindow::acSave()
{
	//this->m_widget_crossword->write("");

}
void QWidgetMainWindow::acSolution()
{
	
}
void QWidgetMainWindow::acPlayMode() 
{
	//this->m_acNew->setEnabled(!m_acPlayMode->isChecked());
	//m_acCheck->setEnabled(m_acPlayMode->isChecked());
	//if (m_widget_crossword) m_widget_crossword->changeState(m_acPlayMode->isChecked()?PLAY:EDITION);
	//if (m_acEditionMode->isChecked()) selectWord(m_current_advance_direction,m_initial_square_y,m_initial_square_x,false);
	
	////
	StatusManager::setOn(StatusManager::ST_PLAYING);
	m_crossword_viewer.scene()->update();
	m_statusState->setText("ST_PLAYING");

}
void QWidgetMainWindow::acEditionMode()
{
	//m_acNew->setEnabled(m_acEditionMode->isChecked());
	//m_acCheck->setEnabled(!m_acEditionMode->isChecked());
	//if (m_widget_crossword) m_widget_crossword->changeState(m_acPlayMode->isChecked()?PLAY:EDITION);
	//if (m_acEditionMode->isChecked()) 
	//{
	//	
	//	selectWord(m_current_advance_direction,m_initial_square_y,m_initial_square_x,false);
	//}

	StatusManager::setOn(StatusManager::ST_EDITING);
	m_crossword_viewer.scene()->update();
	m_statusState->setText("ST_EDITING");

}
void QWidgetMainWindow::acSolutionMode()
{
	/*m_acNew->setEnabled(m_acEditionMode->isChecked());
	m_acCheck->setEnabled(!m_acEditionMode->isChecked());
	if (m_widget_crossword) m_widget_crossword->changeState(m_acPlayMode->isChecked() ? PLAY : EDITION);
	if (m_acEditionMode->isChecked())
	{

		selectWord(m_current_advance_direction, m_initial_square_y, m_initial_square_x, false);
	}*/
	StatusManager::setOn(StatusManager::ST_DISPLAY_SOLUTION);
	m_crossword_viewer.scene()->update();
	m_statusState->setText("ST_DISPLAY_SOLUTION");

}
void QWidgetMainWindow::acWithClues()
{
	StatusManager::setOn(StatusManager::ST_PLAYING_WITH_CLUES);
	m_crossword_viewer.scene()->update();
	m_statusState->setText("ST_PLAYING_WITH_CLUES");
}

void QWidgetMainWindow::acCheck()
{
	//int dif=0;
	//int number_of_characters=0;
	//if (m_widget_crossword) 
	//{
	//		dif =m_widget_crossword->compare();
	//		number_of_characters = m_widget_crossword->numberCharacters();
	//}

	////QMessageBox v("Comprobando Autodefinido",QString("Hay :") + QString().setNum(number_of_characters-dif) + QString(" letras correctas de ") + QString().setNum(number_of_characters),QMessageBox::Icon::Information,0,0,0);
	////v.exec();
}
void QWidgetMainWindow::acClear()
{/*
	if (this->m_acEditionMode->isChecked())
	{	
		if (m_widget_crossword) m_widget_crossword->clear();
	}
	else if (m_widget_crossword) m_widget_crossword->clearCharacters(m_acEditionMode->isChecked()?GAME_STATE::EDITION:GAME_STATE::PLAY);
	*/
}

void QWidgetMainWindow::acExit()
{
}
void QWidgetMainWindow::acNextCrossword()
{
	/*if (m_crosswords_paths_list.empty()) return;
	if (m_widget_crossword) m_widget_crossword->write(QString(m_crosswords_paths_list[m_current_crossword_index].c_str()));
	if (m_current_crossword_index+1 < m_crosswords_paths_list.size()) 
		m_current_crossword_index++;
	if (m_current_crossword_index== m_crosswords_paths_list.size()) 
	{
		printf("Ultimo crossword.\n");
			return;
	}
	load(QString(m_crosswords_paths_list[m_current_crossword_index].c_str()));
	m_widget_crossword->update();*/
}
void QWidgetMainWindow::acPreviousCrossword()
{
	//if (m_crosswords_paths_list.empty()) return;
	//if (m_widget_crossword) 
	//	m_widget_crossword->write(QString(m_crosswords_paths_list[m_current_crossword_index].c_str()));
	//if (m_current_crossword_index-1 >=0) 
	//{
	//	m_current_crossword_index--;
	//	load(QString(m_crosswords_paths_list[m_current_crossword_index].c_str()));
	//	m_widget_crossword->update();

	//}
	//else
	//	{printf("Primer crossword.\n");
	//		return;
	//}
	//

}
void QWidgetMainWindow::modifySquareLetter(QString t)
{
	//if (m_acPlayMode->isChecked()) 
	//{
	//		//m_widget_crossword->m_grid_info[m_current_row][m_current_col].char_play=t.toUpper();
	//		m_currentSquare->m_info.char_play=t.toUpper();
	//}
	//else
	//{
	//		//m_widget_crossword->m_grid_info[m_current_row][m_current_col].char_edit=t.toUpper();
	//		m_currentSquare->m_info.char_edit=t.toUpper();
	//}
	
}
//-- Events --//
void QWidgetMainWindow::resizeEvent ( QResizeEvent * event )
{
	resizegrid();
	//if (m_new)
	//acNew();
}
void QWidgetMainWindow::keyPressEvent(QKeyEvent *event )
{	
	//-- KEY PRESSED --//

	int i,j;
	switch( event->key() )
    {
	case Qt::Key_Tab:
	{
		DataManager::changeDirection();
		((QCrosswordScene*)m_crossword_viewer.scene())->changeTo();
		break;
	}
	case Qt::Key_Left:
	{
		if (DataManager::getCurrentDirection() == GAME_DIRECTION::HORIZONTAL)
		{
			//update index
			DataManager::updateBwdCurrentTile();
			((QCrosswordScene*)m_crossword_viewer.scene())->changeTo();
			update();
		}
		break;
	}
	case Qt::Key_Right:
	{
		if (DataManager::getCurrentDirection() == GAME_DIRECTION::HORIZONTAL)
		{
			//update index
			DataManager::updateFwdCurrentTile();
			((QCrosswordScene*)m_crossword_viewer.scene())->changeTo();

			update();
		}
		break;
	}
	case Qt::Key_Up:
	{
		if (DataManager::getCurrentDirection() == GAME_DIRECTION::VERTICAL)
		{
			//update index
			DataManager::updateBwdCurrentTile();
			((QCrosswordScene*)m_crossword_viewer.scene())->changeTo();

			update();
		}
		break;
	}
	case Qt::Key_Down:
	{
		if (DataManager::getCurrentDirection() == GAME_DIRECTION::VERTICAL)
		{
			//update index
			DataManager::updateFwdCurrentTile();
			((QCrosswordScene*)m_crossword_viewer.scene())->changeTo();

			update();
		}
		break;
	}

	case Qt::Key_Backspace:
		if (StatusManager::is(StatusManager::ST_EDITING) || StatusManager::is(StatusManager::ST_PLAYING) || StatusManager::is(StatusManager::ST_PLAYING_WITH_CLUES))
		{
			// delete current
			DataManager::clearSquareText();

			//update index
			DataManager::updateBwdCurrentTile();

			((QCrosswordScene*)m_crossword_viewer.scene())->changeTo();

			//update graphical content
			update();
		}
		//if (m_current_advance_direction!=NONE_DIR)
		//{
		//	
		//	i = m_current_row;
		//	j = m_current_col;
		//	if (m_current_advance_direction==LEFT_DIR)	j ++;
		//	if (m_current_advance_direction==RIGHT_DIR)	j --;
		//	if (m_current_advance_direction==UP) i ++;
		//	if (m_current_advance_direction==DOWN) i--;
		//	if (i<0 || j<0 || i >=m_widget_crossword->rows() || j>=m_widget_crossword->cols()) return;

		//	if (m_widget_crossword->getSquare(i,j)->m_info.type!=DEFINITION_SQUARE)
		//	{
	
		//		if (m_currentSquare!=NULL) m_currentSquare->selected(false);
		//		m_currentSquare = m_widget_crossword->getSquare(i,j);
		//		if (m_currentSquare!=NULL) 
		//		{
		//			//-- we set the new Square to character_square because we are going forward with the word in that direction --//
		//			if (m_currentSquare->m_info.type == NEW_SQUARE) m_currentSquare->m_info.type = CHARACTER_SQUARE;
		//		
		//			m_currentSquare->selected(true);
		//		}
		//		m_current_row = i;
		//		m_current_col = j;
		//		update();
		//	}
		//}
		break;
	case Qt::Key_Delete:
		// delete current
		DataManager::clearSquareText();

		//update graphical content
		update();
		break;
	case Qt::Key_Space:
	{
		//update index
		DataManager::updateFwdCurrentTile();
		// change display to set current tile as the updated one
		((QCrosswordScene*)m_crossword_viewer.scene())->changeTo();
		update();
		break;
	}
	case Qt::Key_Control:
		//showZoom(m_over_row,m_over_col,m_widget_crossword->getInfo(m_over_row,m_over_col));
		break;
	case Qt::Key::Key_F1:
		m_fullScreen_flag=!m_fullScreen_flag;
		if (m_fullScreen_flag) this->showFullScreen();
		else this->showNormal();
		break;
	case Qt::Key::Key_A: case Qt::Key::Key_Aacute:
	case Qt::Key::Key_E: case Qt::Key::Key_Eacute:
	case Qt::Key::Key_I: case Qt::Key::Key_Iacute:
	case Qt::Key::Key_O: case Qt::Key::Key_Oacute:
	case Qt::Key::Key_U: case Qt::Key::Key_Uacute:
	case Qt::Key::Key_B: case Qt::Key::Key_C: case Qt::Key::Key_D: case Qt::Key::Key_F: case Qt::Key::Key_G: case Qt::Key::Key_H:
	case Qt::Key::Key_J: case Qt::Key::Key_K: case Qt::Key::Key_L: case Qt::Key::Key_M: case Qt::Key::Key_N: case Qt::Key::Key_Ntilde:
	case Qt::Key::Key_P: case Qt::Key::Key_Q: case Qt::Key::Key_R: case Qt::Key::Key_S: case Qt::Key::Key_T: case Qt::Key::Key_V:
	case Qt::Key::Key_W: case Qt::Key::Key_X: case Qt::Key::Key_Y: case Qt::Key::Key_Z:
	{

		// put text
		DataManager::appendSquareText(event->text().toUpper().toLocal8Bit().data());
		m_crossword_viewer.scene()->update();
		//if (m_currentSquare!=NULL)
		//{
		//	if (m_currentSquare->m_info.type!=SQUARE_TYPE::DEFINITION_SQUARE && m_current_advance_direction!=NONE_DIR)
		//	{
		//		modifySquareLetter(event->text().toUpper());

		//		//-- SELECT NEXT SQUARE --//
		//
		//		i = m_current_row;
		//		j = m_current_col;
		//		if (m_current_advance_direction==LEFT_DIR)	j --;
		//		if (m_current_advance_direction==RIGHT_DIR)	j ++;
		//		if (m_current_advance_direction==UP) i --;
		//		if (m_current_advance_direction==DOWN) i++;
		//		if (j<0 || i < 0 || j>=m_widget_crossword->m_cols || i >= m_widget_crossword->m_rows)  {update(); return;}

		//		printf("Accessing to (%d,%d) element of (%d,%d) elements\n",i,j,m_widget_crossword->m_rows,m_widget_crossword->m_cols);

		//		//-- COMPROBAR QUE EN LA DIRECCION EN LA QUE VOY NO ME HE SALIDO DEL AUTODEFINIDO --/
		//		if (i<m_widget_crossword->m_rows && j<m_widget_crossword->m_cols)
		//		{
		//			QSquare *square = m_widget_crossword->getSquare(i,j);
		//			//-- COMPROBAR QUE LA NUEVA CASILLA NO ES DEFINITION_SQUARE --/
		//			if (square->m_info.type!=DEFINITION_SQUARE && square->m_info.type!=NULL_SQUARE)
		//			{	
		//				//-- ACTUALIZAR FOCO A NUEVA CASILLA --//
		//				if (m_currentSquare!=NULL) m_currentSquare->selected(false);
		//				m_currentSquare = m_widget_crossword->getSquare(i,j);
		//				if (m_currentSquare!=NULL) 
		//				{
		//					//-- CONVERTIRLA EN CHARACTER_SQUARE SI ERA NUEVA--//
		//					//-- we set the new Square to character_square because we are going forward with the word in that direction --//
		//					if (m_currentSquare->m_info.type ==NEW_SQUARE) m_currentSquare->m_info.type = CHARACTER_SQUARE;
		//						m_currentSquare->selected(true);
		//				}
		//				//-- ACTUALIZAR INDICES DE CASILLA SELECCIONADA --//
		//				m_current_row = i;
		//				m_current_col = j;
		//			
		//			}
		//		}
		//		update();
	
		//	}
		//	else if (m_currentSquare->m_info.type!=DEFINITION_SQUARE)
		//	{
		//			m_current_advance_direction=NONE_DIR;
		//	}
		//}
	}
	break;
	}
	//event->ignore();
}

//void QWidgetMainWindow::selectWord(DIRECTION dir,int y,int x,bool f)
//{
//	//-- Marca de azul una palabra que empieza en x y si f es true --//
//	//-- si no la vuelve a dejar de blanco --//
//	if (dir==DIRECTION::RIGHT_DIR)
//	{
//		for (int j=x;j<m_widget_crossword->cols();j++)
//		{   QSquare *square = m_widget_crossword->getSquare(y,j);
//			if (square->m_info.type != SQUARE_TYPE::CHARACTER_SQUARE) break;
//			if (f) square->m_selected=2;
//			else square->m_selected=0;
//			square->updateBackground();
//		}
//	}
//	else if (dir==DIRECTION::LEFT_DIR)
//	{
//		for (int j=x;j>=0;j--)
//		{
//			QSquare *square = m_widget_crossword->getSquare(y,j);
//			if (square->m_info.type != SQUARE_TYPE::CHARACTER_SQUARE) break;
//			if (f) square->m_selected=2;
//			else square->m_selected=0;
//			square->updateBackground();
//		}
//	}
//	else if (dir==DIRECTION::UP)
//	{
//		for (int i=y;i>=0;i--)
//		{
//			QSquare *square = m_widget_crossword->getSquare(i,x);
//			if (square->m_info.type != SQUARE_TYPE::CHARACTER_SQUARE) break;
//			if (f) square->m_selected=2;
//			else square->m_selected=0;
//			square->updateBackground();
//		}
//	}
//	else if (dir==DIRECTION::DOWN)
//	{
//		for (int i=y;i<m_widget_crossword->rows();i++)
//		{
//			QSquare *square = m_widget_crossword->getSquare(i,x);
//			if (square->m_info.type != SQUARE_TYPE::CHARACTER_SQUARE) break;
//			if (f) square->m_selected=2;
//			else square->m_selected=0;
//			square->updateBackground();
//		}
//	}
//
//}
void QWidgetMainWindow::sltDefPressed(int row, int col, int def_selected)
{
	//sltSquarePressed(row,col,m_widget_crossword->getSquare(row,col)->m_info,def_selected);
}
//-- SLOTS --//
//void QWidgetMainWindow::sltSquarePressed(int row,int col,TSquareInfo &info,int where_clicked)
//{
//	//-- SQUARE CLICKED --//
//	//-- emit pressed from QSquare  --//
//	//-- Mouse Click over a QSquare --//
//	//--------------------------------//
//
//	//printf("Pressed %d  %d  %s", i, j,info.title1.c_str());
//	
//	
//	//--make current square the one clicked --//
//	if (m_currentSquare!=NULL) m_currentSquare->selected(false);
//	m_currentSquare = m_widget_crossword->getSquare(row,col);
//	//m_currentSquare->selected(1);
//	//update();
//	m_current_row = row;
//	m_current_col = col;
//
//
//
//	if (info.parents.size()>0 && m_acEditionMode->isChecked())
//	{
//		//-- si es un CHARACTER_SQUARE inicio de palabra cogeremos la dirección que marque el primero de los padres--//
//		//-- En modo edición , en modo play hay que seleccionar la palabra --//
//		if (info.parents.size()==1) m_current_advance_direction=info.parents[0].dir;
//	
//		else if (where_clicked == 1) 
//			m_current_advance_direction = info.parents[0].dir;
//		else m_current_advance_direction = info.parents[1].dir;
//		
//	}
//	else
//	{
//		
//		if (m_acEditionMode->isChecked())
//		{
//			m_current_advance_direction=NONE_DIR;
//			//-- EDITION MODE --//
//			m_info_widget.fill(info);
//			m_info_widget.show();
//		}
//		else
//		{
//			//-- PLAY MODE --//
//			//-- DESELECCIONAR PALABRA --//
//			selectWord(m_current_advance_direction,m_initial_square_y,m_initial_square_x,false);
//			m_current_advance_direction=NONE_DIR;
//
//
//			//-- Si es DEFINITION --//
//			//-- Seleccionar la palabra vinculada a la definición seleccionada con where clicked --//
//
//			//-- Buscar el número de palabras del que forma parte esta casilla --//
//			//-- izda - dcha y arriba-abajo --//
//			//-- Si solo forma parte de una seleccionar esa dirección --//
//			//-- Si forma parte de dos 
//			//-- Si se ha clickado la parte superior seleccionar la vertical --//
//			//-- Si se ha clickado la parte inferior seleccionar la horizontal --//
//			if (info.type!= DEFINITION_SQUARE && info.type!=NULL_SQUARE)
//			{
//
//				DIRECTION initSquare_found_DOWN_UP= NONE_DIR;
//				DIRECTION initSquare_found_LEFT_RIGHT= NONE_DIR;
//				QPoint initial_squares[2];
//
//				m_current_col=m_initial_square_x=col;
//				m_current_row=m_initial_square_y=row;
//				//-- Searching for the limiting squares --/
//						
//					for (int i=m_current_row;i>=0;i--)
//					{
//						QSquare *square = m_widget_crossword->getSquare(i,m_current_col);
//						//-- Comprobar si esta casilla CHAR tiene padres --//
//						//-- Si es así es una casilla de inicio de palabra --//
//						if (square->m_info.type!=SQUARE_TYPE::DEFINITION_SQUARE)
//						{
//							//--Recorrer padres en busca de una correspondencia de palabra hacia abajo --//
//							//-- ya que estamos recorriendo hacia arriba --//
//							for (int j=0;j<square->m_info.parents.size();j++)
//							{
//								if (square->m_info.parents[j].dir==DOWN )
//								{
//									initial_squares[0].setX( m_current_col );
//									initial_squares[0].setY( i );
//									initSquare_found_DOWN_UP = DOWN;
//								}
//
//							}
//							
//							
//						}
//						else break;
//					}
//
//					if (initSquare_found_DOWN_UP==NONE_DIR)
//					{
//						
//						for (int i=m_current_row;i<m_widget_crossword->rows();i++)
//						{
//							QSquare *square = m_widget_crossword->getSquare(i,m_current_col);
//							if (square->m_info.type!=SQUARE_TYPE::DEFINITION_SQUARE)
//							{
//								for (int j=0;j<square->m_info.parents.size();j++)
//								{
//									if (square->m_info.parents[j].dir==UP)
//									{		
//										initial_squares[0].setX( m_current_col );
//										initial_squares[0].setY( i );
//										initSquare_found_DOWN_UP = UP;
//									}
//								}
//								
//							
//							}
//							else break;
//						}
//					}
//					
//					for (int j=m_current_col;j>=0;j--)
//					{
//						QSquare *square = m_widget_crossword->getSquare(m_current_row,j);
//						if (square->m_info.type!=SQUARE_TYPE::DEFINITION_SQUARE)
//						{
//							for (int k=0;k<square->m_info.parents.size();k++)
//							{
//								if (square->m_info.parents[k].dir==RIGHT_DIR) 
//								{
//									initial_squares[1].setX( j );
//									initial_squares[1].setY( m_current_row );
//									initSquare_found_LEFT_RIGHT=RIGHT_DIR;
//								}
//							}
//							
//						}
//						else break;
//					}
//
//					if (initSquare_found_LEFT_RIGHT==NONE_DIR)
//					{
//						for (int j=m_current_col;j<m_widget_crossword->cols();j++)
//						{
//							QSquare *square = m_widget_crossword->getSquare(m_current_row,j);
//							if (square->m_info.type!=SQUARE_TYPE::DEFINITION_SQUARE)
//							{
//								for (int k=0;k<square->m_info.parents.size();k++)
//								{
//									if (square->m_info.parents[k].dir==LEFT_DIR)
//									{
//										initial_squares[1].setX( j );
//										initial_squares[1].setY( m_current_row );
//										initSquare_found_LEFT_RIGHT=LEFT_DIR;
//									}
//								}
//									
//								
//							}
//							else break;
//						}
//					}
//					
//					if (initSquare_found_LEFT_RIGHT==NONE_DIR && initSquare_found_DOWN_UP!=NONE_DIR)
//					{
//						m_current_advance_direction=initSquare_found_DOWN_UP;
//						m_initial_square_y=initial_squares[0].y();
//						m_initial_square_x=initial_squares[0].x();
//					}
//					else if (initSquare_found_LEFT_RIGHT!=NONE_DIR && initSquare_found_DOWN_UP==NONE_DIR)
//					{
//						m_current_advance_direction=initSquare_found_LEFT_RIGHT;
//						m_initial_square_y=initial_squares[1].y();
//						m_initial_square_x=initial_squares[1].x();
//					}
//					else  if (initSquare_found_LEFT_RIGHT!=NONE_DIR && initSquare_found_DOWN_UP!=NONE_DIR)
//					{
//						if (where_clicked==1)
//						{
//							m_current_advance_direction=initSquare_found_DOWN_UP;
//							m_initial_square_y=initial_squares[0].y();
//							m_initial_square_x=initial_squares[0].x();
//						}
//						else 
//						{
//							m_current_advance_direction=initSquare_found_LEFT_RIGHT;
//							m_initial_square_y=initial_squares[1].y();
//							m_initial_square_x=initial_squares[1].x();
//						}
//					}
//
//					selectWord(m_current_advance_direction,m_initial_square_y,m_initial_square_x,true);
//					
//			}
//			else
//			{
//				if (info.childs.size()==2)
//				{
//					if (where_clicked==1)
//					{
//						// DEF 1 clicked
//						//-- Asignar a la palabra actual la primera casilla de la palabra def1 --//
//						m_current_col=m_initial_square_x=info.childs[0].x;
//						m_current_row=m_initial_square_y=info.childs[0].y;
//						
//						m_current_advance_direction=info.childs[0].dir;
//	
//						selectWord(m_current_advance_direction,m_initial_square_y,m_initial_square_x,true);
//	
//	
//					}
//					else 
//					{  // DEF 2
//	
//						m_current_col=m_initial_square_x=info.childs[1].x;
//						m_current_row=m_initial_square_y=info.childs[1].y;
//						
//						m_current_advance_direction=info.childs[1].dir;
//	
//						selectWord(m_current_advance_direction,m_initial_square_y,m_initial_square_x,true);
//	
//					}
//				}
//				else
//				{
//						m_current_col=m_initial_square_x=info.childs[0].x;
//						m_current_row=m_initial_square_y=info.childs[0].y;
//						
//						m_current_advance_direction=info.childs[0].dir;
//	
//						selectWord(m_current_advance_direction,m_initial_square_y,m_initial_square_x,true);
//	
//				}
//					m_currentSquare = m_widget_crossword->getSquare(m_current_row,m_current_col);
//
//			}
//
//		}
//	}
//	m_currentSquare->selected(1);
//	update();
//}
//void QWidgetMainWindow::sltSquareOver(int row,int col,TSquareInfo &info)
//{
//
//	m_over_row = row;
//	m_over_col = col;
//
//}
//void QWidgetMainWindow::showZoom(int row,int col,TSquareInfo &info)
//{
//	//-- mostrar zoom de la casilla --//
//	if (info.type==SQUARE_TYPE::DEFINITION_SQUARE)
//	{
//		
//		m_zoomWidget.hide();
//		QSquare *sq = m_widget_crossword->getSquare(row,col);
//		
//		QPoint position = sq->pos();
//		QPoint position1 = m_widget_crossword->pos();
//		m_zoomWidget.setGeometry(this->pos().x() + position1.x()+position.x(),this->pos().y()+position1.y()+position.y(),100,100);
//		m_zoomWidget.m_def1= info.title1;
//		m_zoomWidget.m_def2= info.title2;
//		m_zoomWidget.m_row= row;
//		m_zoomWidget.m_col= col;
//
//		m_zoomWidget.show();
//		
//	}
//}
//void QWidgetMainWindow::sltUpdate(TSquareInfo &info)
//{
//	//-- Funcion que se llama desde la ventana de creación QSquareInfo --//
//	SQUARE_TYPE from, to;
//	from = m_widget_crossword->getSquare(m_current_row,m_current_col)->m_info.type;
//	to = info.type;
//	//info.printSquare();
//	if (m_current_col==m_cols-1 && (info.position1 == POSITION::RIGHT || info.position2 == POSITION::RIGHT)) return;
//	if (m_current_col==0 && (info.position1 == POSITION::LEFT || info.position2 == POSITION::RIGHT)) return;
//	if (m_current_row==m_rows-1 && (info.position1 == POSITION::BOTTOM || info.position2 == POSITION::BOTTOM)) return;
//	if (m_current_row==0 && (info.position1 == POSITION::TOP || info.position2 == POSITION::TOP)) return;
//
//	if (from == SQUARE_TYPE::DEFINITION_SQUARE && to == SQUARE_TYPE::CHARACTER_SQUARE)
//	{
//		//-- Desvincular o eliminar asociaciones entre la casilla actual y sus hijos o padres --//
//		//-- entre casillas definicion y sus casillas char --//
//		//-- entre casillas char y sus casilla definición --//
//		m_widget_crossword->desvincular(m_current_row,m_current_col);
//		m_widget_crossword->modifyInfo(m_current_row,m_current_col,info);
//		m_widget_crossword->vincular(m_current_row,m_current_col);
//
//	}
//	else if (from == SQUARE_TYPE::NEW_SQUARE)
//	{
//		
//		if (!info.char_edit.isEmpty()) info.char_edit=info.char_edit.toUpper();
//	//	if (!info.char_play.isEmpty()) info.char_edit=info.char_play.toUpper();
//
//		m_widget_crossword->modifyInfo(m_current_row,m_current_col,info);
//		if (to== SQUARE_TYPE::DEFINITION_SQUARE)	m_widget_crossword->vincular(m_current_row,m_current_col);
//		
//	}
//	else if (from == SQUARE_TYPE::DEFINITION_SQUARE && to == SQUARE_TYPE::NULL_SQUARE)
//	{
//		m_widget_crossword->desvincular(m_current_row,m_current_col);
//		m_widget_crossword->modifyInfo(m_current_row,m_current_col,info);
//		m_widget_crossword->vincular(m_current_row,m_current_col);
//
//	}
//	else if (from == SQUARE_TYPE::CHARACTER_SQUARE && to == SQUARE_TYPE::CHARACTER_SQUARE)
//	{
//		m_widget_crossword->fillInfo(m_current_row,m_current_col,info);
//		
//	}
//	else if (from == SQUARE_TYPE::CHARACTER_SQUARE && to == SQUARE_TYPE::DEFINITION_SQUARE)
//	{
//		m_widget_crossword->desvincular(m_current_row,m_current_col);
//		m_widget_crossword->fillInfo(m_current_row,m_current_col,info);
//		m_widget_crossword->vincular(m_current_row,m_current_col);
//		
//	}
//	else 
//	{
//		m_widget_crossword->desvincular(m_current_row,m_current_col);
//		m_widget_crossword->fillInfo(m_current_row,m_current_col,info);
//		m_widget_crossword->vincular(m_current_row,m_current_col);
//	}
//
//	m_widget_crossword->update();
//	
//}

void QWidgetMainWindow::sltUpdate(int rows, int cols)
{
	m_rows = rows;
	m_cols = cols;
}
void QWidgetMainWindow::sltNew(int rows, int cols)
{
	m_rows = rows;
	m_cols = cols;

	if (m_rows == 0 || m_cols==0) return;
	//-- Calcular el tamaño de las celdas --//
	

	//Delete Scene Tiles
	m_crossword_viewer.scene()->clear();

	// Delete Data
	DataManager::clear();


	//Create New Crossword
	// Create data and retrieve index in the array of crosswords
	int idx = DataManager::createCrossword(m_cols, m_rows);
	// Create graphical view of crossword idx
	m_crossword_viewer.createCrosswordinScene(idx, 40);



	//if (m_widget_crossword !=NULL) {disconnectAllSquares(); delete m_widget_crossword;}
	//m_widget_crossword = new QWidgetCrossword(this);

	//double marginy=0.0,marginx=0.0,square_size=0.0;
	//calculateSquareSize(m_rows, m_cols, m_grid_sizey, m_grid_sizex,marginy, marginx,square_size);
	//
	//printf("Setting crossword geometry: (%.2lf,%.2lf,%d,%d)\n", marginx,marginy,m_grid_sizex, m_grid_sizey);
	////-- posicionamos el widget con el grid --//
	//m_widget_crossword->setGeometry(marginx,marginy,m_grid_sizex, m_grid_sizey);
	//
	//m_widget_crossword->create(m_rows,m_cols,square_size);
	//m_widget_crossword->changeState(m_acEditionMode->isChecked()?EDITION:PLAY);
	//m_widget_crossword->show();
	//	
	//connectAllSquares();
	//m_new = true;

}