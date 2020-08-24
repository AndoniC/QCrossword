#pragma once


#include <QWidget>
#include <QGridLayout>

#include <QtGui>
#include <iostream>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "QCrosswordSquare.h"
#include "DataManager.h"
#include "CrosswordData.h"
#include <QGraphicsSceneMouseEvent>
#include "QSquareInfo.h"

class QCrosswordScene : public QGraphicsScene
{
	Q_OBJECT

public:
	QCrosswordScene(QGraphicsView *parent = 0);
	~QCrosswordScene();
	
	//void createGrid(int rows,int cols,int size);
	//void create(int rows,int cols, int size);
	

	/** Draw content before items are painted */
	void drawBackground(QPainter *painter, const QRectF &rect);

	/** Draw content after items are painted */
	void drawForeground(QPainter * painter, const QRectF & rect);
	
	void createGraphicalCrossword(int idx_data, int h, int w);
	void clearSemiSelected();
	void setSemiSelected(cv::Point from, cv::Point to);
	/** Change selected tile to pos_x, pos_y */
	void changeTo(int pos_x=-1, int pos_y=-1);


	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

protected:
	std::vector< std::vector< QCrosswordSquare *> > squaregrid;

	QSquareInfo m_info_widget;
public slots:
	void sltUpdate();

signals:
	void newCrossWord(int rows, int cols);
	
};

