#ifndef QWIDGETSPLASHSCREEN_H
#define QWIDGETSPLASHSCREEN_H

#include <QSplashScreen>
#include <QPainter>
class QWidgetSplashScreen : public QSplashScreen
{
	Q_OBJECT

public:
	QWidgetSplashScreen(const QString & file , Qt::WindowFlags f = 0 );
	QWidgetSplashScreen(const QPixmap & pixmap , Qt::WindowFlags f = 0 );
	QWidgetSplashScreen(QWidget *parent, const QPixmap & pixmap = QPixmap(), Qt::WindowFlags f = 0 );
	QWidgetSplashScreen(QWidget *parent, const QString & file = QString(), Qt::WindowFlags f = 0 );
	~QWidgetSplashScreen();

	void PaintText(QString texto,QFont f,int x, int y, QColor color,int align);
	void drawContents ( QPainter * painter );

private:
	QFont __font;
	QColor __color;
	QString __text;
	int __alignment;
	int __x;
	int __y;
};

#endif // CSPLASHSCREEN_H
