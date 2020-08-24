#pragma once


#include <QWidget>
#include <QGridLayout>
#include <QtGui>
#include <iostream>
#include <QFileDialog>
#include <QGraphicsView>
#include "QCrosswordScene.h"
#include <QWheelEvent>
#include <QAction>
#include <QMenu>
#include <QApplication>

class QCrosswordView : public QGraphicsView
{
	Q_OBJECT

public:
	QCrosswordView( QWidget *parent = 0, bool useOpenGL = true);
	~QCrosswordView() {};
	
	void createGrid(int rows,int cols,int size);
	void createCrosswordinScene(int idx_data, int size);
	

	/** Virtual method to update the viewer. This is equivalent to QWidget::update(). */
	virtual void Update()
	{
		QGraphicsView::update();
	}


	void zoomIn();
	void zoomOut();
	/** Set the 2D Meta Image to be displayed. This passes the image also
	* to the corresponding GraphicsItem. */
	void SetSceneSize(const QSize &size);
	QSize GetSceneSize();
	void scaleView(qreal scaleFactor);
	void drawForeground(QPainter *painter, const QRectF &rect);

	void init();

	/** Handles Qt's resizeEvent to set the scene size to that of the window and position
	* the slice on the middle scene. This implementation is for single-slice viewers
	* only and multi-slice viewers must reimplement it. */
	virtual void resizeEvent(QResizeEvent * event);
	void paintEvent(QPaintEvent * event);

	virtual void wheelEvent(QWheelEvent * event);
	void keyPressEvent(QKeyEvent *event);


	QCrosswordScene *m_Scene;
	
signals:
	void newCrossWord(int rows, int cols);
	
};

