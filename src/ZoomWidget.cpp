#include "ZoomWidget.h"

QZoomWidget::QZoomWidget(QWidget *parent)
	: QWidget(parent)
{
	m_background.load(":/black");
	create();
		
}
QZoomWidget::QZoomWidget( QString def1, QString def2, QWidget *parent)
	: QWidget(parent)
{
	m_def1 = def1;
	m_def2 = def2;
	m_background.load(":/black");
	create();
}
void QZoomWidget::create()
{
	this->setWindowFlags(Qt::Window |  Qt::FramelessWindowHint | Qt::CustomizeWindowHint| Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint );//| Qt::WindowTitleHint);
	
	//this->setAttribute(Qt::WA_TranslucentBackground);
	//this->setAttribute( Qt::WA_TranslucentBackground, true );
	this->setMouseTracking(true);
	this->setWindowOpacity( 0.9 );
	//cursorWidget.setEnabled( false );
	//this->setFocusPolicy( Qt::NoFocus );
	//int w,h;
	//w =  this->parentWidget()->width();
	//w = QApplication::desktop()->width(); 
	//h = this->parentWidget()->height();
	//h = QApplication::desktop()->height(); 
	//this->setGeometry(0,0, w,h);
	//this->setAttribute( Qt::WA_TransparentForMouseEvents);
	/*#if _WIN32
		HWND hwnd = (HWND) winId();
		LONG styles = GetWindowLong(hwnd, GWL_EXSTYLE);
		SetWindowLong(hwnd, GWL_EXSTYLE, styles | WS_EX_TRANSPARENT);
	#endif*/

	//show();
}
int QZoomWidget::fitSize(QString text,QFont &f, QRect size)
{
	int fit = false;
		
	QFont myFont = f;	

	while (!fit && myFont.pointSize() > 6)
	{
		QFontMetrics fm( myFont );		
		QRect bound = fm.boundingRect(size,/*Qt::TextWrapAnywhere*/Qt::TextWordWrap | Qt::AlignCenter, text);
		
		if (bound.width() <= size.width() &&
		    bound.height() <= size.height())
		    fit = true;
		else
			myFont.setPointSize(myFont.pointSize() - 1);
	}
	
	return myFont.pointSize();
}

void QZoomWidget::paintEvent(QPaintEvent *event)
{
	
	
  	QPainter painter(this);
	
	QPen pen(QColor("#000000"));//Here lines are also drawn using this color
    painter.setPen(pen);
		
	QRect frameRect = m_background.rect();
	QRectF source(frameRect.left(), frameRect.top(),frameRect.width(),frameRect.height());
	
	int x = 0;
	int y = 0;
	int w = this->width();
	int h = this->height();
	QRectF target(0,0,w,h);
	
	painter.drawPixmap(target,m_background,source);
	
	


	painter.drawRect(QRect(0,0,w,h));
	
	QPen penDText(QColor("#FFFFFF"));//Here lines are also drawn using this color
    painter.setPen(penDText);

		//QFontMetrics fm( sansFont );

	if (!m_def1.isEmpty())
	{
		if (m_def2.isEmpty())
		{
			m_limit = h-1;
			int newsize= fitSize(m_def1,m_font, QRect(2,2,w-2,h-2));
			m_font.setPointSize(newsize);
			painter.setFont(m_font);
			painter.drawText(1,1,w-1,h-1,Qt::AlignCenter|Qt::TextWrapAnywhere ,m_def1);
		}
		else {
				
			int newsize= fitSize(m_def1,m_font, QRect(2,2,w-2,h/2-2));
			m_font.setPointSize(newsize);
			painter.setFont(m_font);
				
			//QRect bound = fm.boundingRect(1,1, w-1, h/2, Qt::TextWrapAnywhere | Qt::AlignLeft, m_info->title1.c_str());
			painter.drawText(1,1,w-1,h/2,Qt::AlignCenter |Qt::TextWrapAnywhere,m_def1);

			painter.drawLine(1, h/2, w-1, h/2);
			m_limit = h/2;	
			newsize= fitSize(m_def2,m_font, QRect(2,h/2+2,w-2,h-2));
			m_font.setPointSize(newsize);
			painter.setFont(m_font);

			//bound = fm.boundingRect(1,h/2+1, w-1, h/2, Qt::TextWrapAnywhere | Qt::AlignLeft, m_info->title2.c_str());
			painter.drawText(1,h/2+1,w-1,h/2,Qt::AlignCenter |Qt::TextWrapAnywhere ,m_def2);
		}
	}
	else
	{
		if (!m_def2.isEmpty()) 
		{	m_limit =0;
			painter.drawText(0,0,w-1,h-1,Qt::AlignCenter ,m_def2);
		}
	}
	

}

void QZoomWidget::mousePressEvent ( QMouseEvent * event )
{
	//printf("presionado %d %d\n",m_row,m_col);
	int w = this->width();
	int h = this->height();

	
	if (event->y()<m_limit) m_def_selected = 1;
	else m_def_selected= 0;

	emit press(m_row,m_col, m_def_selected);
	
	this->close();
}

void QZoomWidget::leaveEvent ( QEvent * event )
{
	this->close();

}
