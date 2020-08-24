#include "QWidgetSplashScreen.h"

QWidgetSplashScreen::QWidgetSplashScreen(const QString & file , Qt::WindowFlags f  )
: QSplashScreen(QPixmap(file),f)
{

}
QWidgetSplashScreen::QWidgetSplashScreen(const QPixmap & pixmap , Qt::WindowFlags f )
	: QSplashScreen(pixmap,f)
{

}
QWidgetSplashScreen::QWidgetSplashScreen(QWidget *parent, const QPixmap & pixmap , Qt::WindowFlags f  )
	: QSplashScreen(parent,pixmap,f)
{

}
QWidgetSplashScreen::QWidgetSplashScreen(QWidget *parent, const QString & file , Qt::WindowFlags f  )
	: QSplashScreen(parent,QPixmap(file),f)
{
}


QWidgetSplashScreen::~QWidgetSplashScreen()
{

}
void QWidgetSplashScreen::PaintText(QString texto,QFont f,int x, int y, QColor color,int align)
{
	__text=texto;
	__font=f;
	__color=color;
	__alignment=align;
	__x=x;
	__y=y;
	repaint();
}
void QWidgetSplashScreen::drawContents ( QPainter * painter )
{

	painter->setBrush( __color );
	painter->setPen(__color);
	painter->setFont( __font );
	QFontMetrics fm(__font);
	
	painter->drawText(QRect(__x,__y,fm.width(__text),fm.height()), __alignment, __text );

}