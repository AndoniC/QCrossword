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
#include <QSpacerItem>
#include "QWidgetCrossword.h"
#include <QPushButton>

class QWidgetCentral : public QWidget
{
	Q_OBJECT

public:
	QWidgetCentral( QWidget *parent = 0);
	~QWidgetCentral();
	void createLayouts(int rows, int cols, int size);
	void newCrossword(int rows,int cols,int size);

	
	QHBoxLayout m_layout_horizontal;
	QWidgetCrossword *m_widget_crossword;
	QSpacerItem *m_left,*m_right;
	
	

};

