#include <QApplication>
#include <QDesktopWidget>
#include "QWidgetCentral.h"
#include <iostream>

QWidgetCentral::QWidgetCentral( QWidget *parent)
	: QWidget(parent)	  
{
	//ui.setupUi(this);

	
	
	m_left = new QSpacerItem(100,100);
	m_right= new QSpacerItem(100,100);
	
	
}

QWidgetCentral::~QWidgetCentral()
{
}
void QWidgetCentral::createLayouts(int rows, int cols, int size)
{
	m_widget_crossword = new QWidgetCrossword();
	m_widget_crossword->create(rows,cols,size);
	
	//m_layout_horizontal.addSpacerItem(m_left);
	m_layout_horizontal.addWidget(m_widget_crossword);
	//m_layout_horizontal.addSpacerItem(m_right);
	this->setLayout(&m_layout_horizontal);

	
	
}
void QWidgetCentral::newCrossword(int rows,int cols,int size)
{

	createLayouts(rows,cols,size);



}
