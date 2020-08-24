#include <QtGui>
#include "QSquare.h"

DIRECTION resolveDirection(QString dir)
{
	if (dir.compare("LEFT")==0)	return LEFT_DIR;
	else if (dir.compare("RIGHT")==0) return RIGHT_DIR;
	else if (dir.compare("UP")==0) return UP;
	else if (dir.compare("DOWN")==0) return DOWN;
	else return NONE_DIR;
};

POSITION resolvePosition(QString pos)
{
	if (pos.compare("LEFT")==0)	return LEFT;
	else if (pos.compare("RIGHT")==0) return RIGHT;
	else if (pos.compare("TOP")==0) return TOP;
	else if (pos.compare("BOTTOM")==0) return BOTTOM;
	else return NONE;
};


int fitSize(QString text,QFont &f, QRect size)
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

QSquare::QSquare(int size,int row,int col, TSquareInfo info, QWidget *parent)
	: QWidget(parent)
{
	create (0,size,row,col,info);
	
}
QSquare::QSquare( int pos,int size,int row,int col, TSquareInfo info, QWidget *parent)
	: QWidget(parent)
{
	create(pos,size,row,col,info);
	
}

void QSquare::create(int pos, int size,int row,int col, TSquareInfo &info)
{
	m_row=row;
	m_col=col;
	
	this->setFixedSize(size,size);
	
	sansFont.setFamily("Helvetica [Cronyx]");
	sansFont.setBold(true);
	sansFont.setPointSize(20);
	titleFont.setFamily("Helvetica [Cronyx]");
	titleFont.setPointSize(10);

	//m_info = *info;
	copy(info);

	place=pos;
	m_size=size;
	m_selected=0;
	
	this->setMouseTracking(true);
	
}

QSquare::~QSquare()
{
}
void QSquare::setBackground(std::string path)
{
	m_background.load(path.c_str());
}
void QSquare::setPosAndDir(DIRECTION d1,POSITION p1,DIRECTION d2, POSITION p2)
{
	m_info.direction1=d1;
	m_info.position1 =p1;
	m_info.direction2=d2;
	m_info.position2 =p2;
}

void QSquare::paintEvent(QPaintEvent *event)
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
	
	

	//painter.drawPixmap(target,currentFrame,source);
	if (place == 1) painter.drawRect(QRect(0,0,w-1,h));
	else if (place == 2) painter.drawRect(QRect(0,0,w,h-1));
	else if (place == 3) painter.drawRect(QRect(0,0,w-1,h-1));
	else painter.drawRect(QRect(0,0,w,h));
	
	if (m_selected==1)
	{
		QPen penrect(QColor("#FF0000"));//Here lines are also drawn using this color
	    painter.setPen(penrect);
		painter.drawRect(QRect(1,1,w-2,h-2));
		painter.setPen(pen);
	}

	if (m_info.type==CHARACTER_SQUARE)
		{
			QPen penCText(QColor("#000000"));//Here lines are also drawn using this color
			painter.setPen(penCText);
			if (!m_info.char_edit.isEmpty())
			{	
				painter.setFont(sansFont);
				if (m_state==PLAY) painter.drawText(0,0,w-1,h-1,Qt::AlignCenter ,m_info.char_play);
				else if (m_state==EDITION) painter.drawText(0,0,w-1,h-1,Qt::AlignCenter ,m_info.char_edit);
			}
		}
		else if (m_info.type==DEFINITION_SQUARE)
		{
			QPen penDText(QColor("#FFFFFF"));//Here lines are also drawn using this color
			painter.setPen(penDText);

			//QFontMetrics fm( sansFont );

			if (!m_info.title1.isEmpty())
			{
				if (m_info.title2.isEmpty())
				{
					int newsize= fitSize(m_info.title1,titleFont, QRect(2,2,w-2,h-2));
					titleFont.setPointSize(newsize);
					painter.setFont(titleFont);
					painter.drawText(1,1,w-1,h-1,Qt::AlignCenter|Qt::TextWrapAnywhere ,m_info.title1);
				}
				else {
				
					int newsize= fitSize(m_info.title1,titleFont, QRect(2,2,w-2,h/2-2));
					titleFont.setPointSize(newsize);
					painter.setFont(titleFont);
				
					//QRect bound = fm.boundingRect(1,1, w-1, h/2, Qt::TextWrapAnywhere | Qt::AlignLeft, m_info->title1.c_str());
					painter.drawText(1,1,w-1,h/2,Qt::AlignCenter |Qt::TextWrapAnywhere,m_info.title1);

					painter.drawLine(1, h/2, w-1, h/2);
				
					newsize= fitSize(m_info.title2,titleFont, QRect(2,h/2+2,w-2,h-2));
					titleFont.setPointSize(newsize);
					painter.setFont(titleFont);

					//bound = fm.boundingRect(1,h/2+1, w-1, h/2, Qt::TextWrapAnywhere | Qt::AlignLeft, m_info->title2.c_str());
					painter.drawText(1,h/2+1,w-1,h/2,Qt::AlignCenter |Qt::TextWrapAnywhere ,m_info.title2);
				}
			}
			else
			{
				if (!m_info.title2.isEmpty()) painter.drawText(0,0,w-1,h-1,Qt::AlignCenter ,m_info.title2);
			}
		}
	

}
void QSquare::selected(int sel)
{
	m_selected=sel;
}
void QSquare::mousePressEvent ( QMouseEvent * event )
{
	//printf("presionado %d %d\n",m_row,m_col);
	int w = this->width();
	int h = this->height();

	
	int where_clicked;
	if (event->x()>event->y()) where_clicked = 1;
	else where_clicked= 0;

	emit press(m_row,m_col,m_info,where_clicked);
}
void QSquare::mouseMoveEvent ( QMouseEvent * event )
{
	if (m_info.type != DEFINITION_SQUARE) return;
	//printf("presionado %d %d\n",m_row,m_col);
	int w = this->width();
	int h = this->height();

	emit over(m_row,m_col,m_info);
}

void QSquare::updateBackground()
{
	
	if (m_info.type == DEFINITION_SQUARE || m_info.type == NULL_SQUARE)
	{
		m_background.load(":/black");
		//printf("   Square %d,%d : %s\n",m_row,m_col,"../extras/rc/background_black.png");
	}
	else  if (m_info.type == CHARACTER_SQUARE)
	{
		QString background_file_name;

		background_file_name=m_info.toStringDir();
		if (background_file_name.isEmpty())
			background_file_name = ":/white";
		else background_file_name = ":/white_"+background_file_name.toLower();
		
		if (m_selected==2)
			background_file_name=background_file_name+"_sel";
		
		//background_file_name += ".png";
		m_background.load(background_file_name);
	//	QByteArray ba = background_file_name.toLocal8Bit();
	//	const char *t =ba.data();
	//	printf("   Square %d,%d : %s\n",m_row,m_col,t);
	}
	else
		{
			m_background.load(":/white");
		//		printf("   Square %d,%d : %s\n",m_row,m_col,"../extras/rc/background_black.png");
	}

}
void QSquare::copy(TSquareInfo &info)
{
	m_info.char_edit=info.char_edit;
	m_info.char_play=info.char_play;
	m_info.childs.clear();
	m_info.childs=info.childs;
	m_info.parents.clear();
	m_info.parents=info.parents;
	m_info.direction1=info.direction1;
	m_info.direction2=info.direction2;
	m_info.position1=info.position2;
	m_info.title1=info.title1;
	m_info.title2=info.title2;
	m_info.type=info.type;

	updateBackground();
}
void QSquare::modify(TSquareInfo &info)
{
	m_info.char_edit=info.char_edit;
	m_info.char_play=info.char_play;
	m_info.direction1=info.direction1;
	m_info.direction2=info.direction2;
	m_info.position1=info.position2;
	m_info.title1=info.title1;
	m_info.title2=info.title2;
	m_info.type=info.type;


	updateBackground();
}
void QSquare::rescale(int size)
{
	this->setFixedSize(size,size);
}