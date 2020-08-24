#include <qwidget.h>
#include <QFont>
#include <QPainter>
#include <QString>
#include "Windows.h"
#include <QMouseEvent>
class QZoomWidget : public QWidget
{
	Q_OBJECT
	
public:
	QZoomWidget( QWidget *parent = 0);
	QZoomWidget( QString def1, QString def2=QString(),QWidget *parent = 0);
	~QZoomWidget(){};
	void create();
	int fitSize(QString text,QFont &f, QRect size);
	QString m_def1;
	QString m_def2;

	QPixmap m_background;

	int m_def_selected;
	QFont m_font;
	int m_limit;
	int m_row, m_col;

	void mousePressEvent ( QMouseEvent * event );
	void paintEvent(QPaintEvent *event);
	void leaveEvent ( QEvent * event );
signals:
	void press(int , int , int );
};