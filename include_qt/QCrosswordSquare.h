#pragma once


#include <QWidget>
#include <QGridLayout>
#include <QtGui>
#include <iostream>
#include <QFileDialog>
#include <QGraphicsItem>
#include <QLineEdit>
#include <QGraphicsProxyWidget>
#include "ext/ext_ocv.h"
#include "StatusManager.h"
#include "DataManager.h"
class QCrosswordSquare : public  QObject, public QGraphicsItem
{
	Q_OBJECT

public:
	QCrosswordSquare(int idx, int idy, int w, int h,int key, QMenu *_menu = 0);
	~QCrosswordSquare() {};
	

	/** shape() defines selection zone.*/
	virtual QPainterPath shape() const;
	/** boundingRect() defines the area of the polygon to be shown.*/
	QRectF boundingRect() const {
		QRectF rect = QRectF(0,0, m_width, m_height).normalized();
		return rect;
	};

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	virtual QCrosswordSquare *getObject() { return this; }
	
	void setDefs(std::vector<std::string> defs)
	{
		//def_list.clear();
		//def_list = defs;
	}
	void setSize(int w, int h)
	{
		m_width = w; m_height = h;

	}

	cv::Point getIds() { return cv::Point(idx, idy); }
	void setSemiSelected(bool v) { semiselected = v; }
	
protected:
	//virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

	//virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	//virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

	//virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);

	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	//void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

protected:
	int		m_isSelected;
	bool    m_fill;
	float   m_alpha;

	int idx, idy;

	int m_width, m_height;
	
	int m_state;

	//std::vector<std::string> def_list;

	bool iskey;

	/** Menu which is going to be shown after right mouse button click. */
	QMenu* m_menu;

	//QLineEdit* m_text; 

	bool semiselected;
	
};

