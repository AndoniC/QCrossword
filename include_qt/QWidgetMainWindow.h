#pragma once

#include <QMainWindow>
#include <QListWidget>
#include <QLabel>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QToolButton>
#include <QAction>
#include <QToolBar>
#include <QFileDialog>
#include <QStatusBar>
#include <QKeyEvent>
#include <QMenuBar>
#include "QSquareInfo.h"
#include "QRowsColsWidget.h"
#include "ZoomWidget.h"
#include "ext/ext_sys.h"
#include "QCrosswordView.h"

class QWidgetMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	QWidgetMainWindow( QWidget *parent = 0);
	~QWidgetMainWindow();
	void createActions();
	void createMenu();
	void createStatusBar();

	void fullScreen(bool fs);	
	void connectAllSquares();
	void disconnectAllSquares();
	void resizegrid();
	virtual void resizeEvent ( QResizeEvent * event );
	void keyPressEvent(QKeyEvent *event );
	
	int load(QString file_name);
	int save(QString file_name);
	int savegame(QString file_name);

	void calculateSquareSize(int rows, int cols, int &height, int &width, double &marginy, double &marginx, double &square_size);
	///void selectWord(DIRECTION dir,int y,int x,bool f);

	//void showZoom(int row,int col,TSquareInfo &info);

	void modifySquareLetter(QString t);


	QCrosswordView m_crossword_viewer;

	//QWidgetCrossword *m_widget_crossword;

	double m_screen_width;
	double m_screen_height;
	QMenuBar *m_menuBar;
	QAction *m_acNew;
	QAction *m_acLoadDir;
	QAction *m_acLoad;
	QAction *m_acSave;
	QAction *m_acSaveGame;
	QAction *m_acSolution;
	QAction *m_acCheck;
	QAction *m_acPlayMode;
	QAction *m_acEditionMode;
	QAction *m_acSolutionMode;
	QAction *m_acWithClues;
	QAction *m_acExit;
	QAction *m_acClear;
	QAction *m_acPreviousCrossword;
	QAction *m_acNextCrossword;

	double m_marginy;
	double m_marginx;
	int m_grid_sizex;
	int m_grid_sizey;
	bool m_new;

	QZoomWidget m_zoomWidget;

	//QSquareInfo m_info_widget;
	QRowsColsWidget m_rowscolswidget;
	int m_rows, m_cols;
	
	CrossWordData::crossword_desc_t new_crossword_desc; // description obtained from QRowsColsWidget

	std::vector<std::string> m_crosswords_paths_list;
	int m_current_crossword_index;

	//QSquare *m_currentSquare;

	int m_current_row, m_current_col; // current char
	int m_over_row, m_over_col;
	int m_initial_square_x, m_initial_square_y; // initial char of the current word
	int m_end_square_x,m_end_square_y; // end char of the current word

	//DIRECTION m_current_advance_direction;
	bool m_fullScreen_flag;
	QActionGroup *m_viewGroup;

	std::string last_dir;


	QLabel *m_statusLeft;
	QLabel *m_statusState;
	QLabel *m_statusMiddle;
	QLabel *m_statusRight;

private slots:

	//-- Actions --//
	void acNew();
	void acLoad();
	void acSave();
	void acSaveGame();
	void acLoadDir();
	void acSolution();
	void acPlayMode();
	void acEditionMode();
	void acSolutionMode();
	void acWithClues();
	void acCheck();
	void acClear();
	void acExit();
	void acNextCrossword();
	void acPreviousCrossword();

	void sltNew(int rows, int cols);

	//void sltSquarePressed(int row,int col,TSquareInfo &info,int where_clicked);
	//void sltSquareOver(int row,int col,TSquareInfo &info);

	//void sltUpdate(TSquareInfo &info);
	void sltUpdate(int rows, int cols, std::string title, std::string topic, std::string units);
	void sltDefPressed(int row, int col, int def_selected);
	

};

