#include "QCrosswordSquare.h"

QCrosswordSquare::QCrosswordSquare(int _idx,int _idy, int w, int h,int key, QMenu* _menu) :m_menu(_menu)
{
	//setFlag(ItemIsMovable);
	//setFlag(ItemSendsGeometryChanges);
	//setCacheMode(DeviceCoordinateCache);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setZValue(-1.1);

	iskey = key;

	m_width = w;
	m_height = h;
	this->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
	setAcceptHoverEvents(true);
	idx = _idx;
	idy = _idy;
	semiselected = false;
	//m_text = new QLineEdit();
	//QGraphicsProxyWidget* pMyProxy = new QGraphicsProxyWidget(this); // the proxy's parent is pMyItem
	//pMyProxy->setWidget(m_text); // adding the QWidget based object to the proxy
	//m_text->setText("A");
	//m_text->setGeometry(0, 0, m_width, m_height);
}

QPainterPath QCrosswordSquare::shape() const
{
	QPainterPath path;
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_MAEMO_5)
	// Add some extra space around the circle for easier touching with finger
	path.addRect(pos().x(), pos().y(), m_width, m_height);
#else
	path.addRect(0,0, m_width, m_height);
#endif
	return path;
}


//! [11]
QVariant QCrosswordSquare::itemChange(GraphicsItemChange change, const QVariant &value)
{
	switch (change) {
	case ItemPositionHasChanged:
		// foreach (Edge *edge, edgeList)
		//    edge->adjust();

		//graph->itemMoved();
		//cada vez que movemos el pol�gono hay que reajustar la posici�n de los puntos.

		//adjust();

		break;

		// case ItemSelectedChange:
		//	 adjust();
		//	 break;
	default:

		break;
	};

	return QGraphicsItem::itemChange(change, value);
}
//! [12]
void QCrosswordSquare::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (QGuiApplication::keyboardModifiers().testFlag(Qt::AltModifier))  return;
	//update();
	//printf("pressed %d %d\n", idx, idy);
	
	QGraphicsItem::mousePressEvent(event);
	
}


void QCrosswordSquare::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (QGuiApplication::keyboardModifiers().testFlag(Qt::AltModifier))  return;

	//update();
	//printf("released %d %d\n", idx, idy);

	

	QGraphicsItem::mouseReleaseEvent(event);
	

}

void QCrosswordSquare::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	if (QGuiApplication::keyboardModifiers().testFlag(Qt::AltModifier))  return;
	//emit siOver(this);
	//update();
	QGraphicsItem::hoverEnterEvent(event);
}

void QCrosswordSquare::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	if (QGuiApplication::keyboardModifiers().testFlag(Qt::AltModifier))  return;
	//emit siOut(this);
	//update();
	QGraphicsItem::hoverLeaveEvent(event);

}

void QCrosswordSquare::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
	//printf ("\n\nPainting point \n\n");
	painter->setPen(Qt::NoPen);
	painter->setBrush(Qt::NoBrush);
	QPen text_color;
	//if (def_list.empty())
	iskey = DataManager::isKey(idx,idy);
	if (!iskey)
	{
		text_color = QColor(0, 0, 0);
		if (this->isSelected())
		{
			painter->setBrush(QColor(255, 150, 130, 255));
		}
		else if (semiselected)
		{
			painter->setBrush(QColor(255, 220, 210, 255));
		}
		else
		{
			painter->setBrush(QColor(255, 255, 255, 255));
		}
	}
	else
	{
		text_color = QColor(255, 255, 255);
		painter->setBrush(QColor(0, 0, 0, 255));
	}
	
	
	//border
	QPainterPath path;
	QPolygonF p;
	p << QPointF(0, 0) << QPointF(m_width, 0) << QPointF(m_width, m_height) << QPointF(0, m_height);
	path.addPolygon(p);
	//QPen pen(Qt::black, 2);
	painter->fillPath(path, Qt::white);

	//painter->setBrush(QColor(0, 0, 0, 255));
	
	painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter->drawPolygon(p);
	//painter->drawPath(path);
	//painter->drawRect(pos().x(),pos().y(),m_width,m_height);
	//painter->drawRect(pos().x(), pos().y(), m_width, m_height);

	
	// drawText
	///////////

	// Get text

	std::string square_text;

	if (StatusManager::is(StatusManager::ST_DISPLAY_SOLUTION) || StatusManager::is(StatusManager::ST_EDITING))
		square_text = DataManager::getSquareText(idy, idx);
	else if (StatusManager::is(StatusManager::ST_PLAYING))
		square_text = DataManager::getSquarePlayingText(idy, idx);


	if (iskey) {
		std::vector<std::string> ndefs = ext::string::split(DataManager::getSquareText(idy, idx), ";;");
		/// Draw Identifier if flag is active
		if (ndefs.size() >= 1)
		{
			square_text = ext::string::toUpperCase(ndefs[0]);

			std::locale::global(std::locale(""));
			std::wcout.imbue(std::locale());
			auto& f = std::use_facet<std::ctype<wchar_t>>(std::locale());
			std::wstring str = ext::string::utf8_to_wstring(square_text);
			f.toupper(&str[0], &str[0] + str.size());
			
			square_text= ext::string::wstring_to_utf8(str);
		

			QFont font("Arial", 4, 3, false);
			painter->setFont(font);
			QFontMetrics metrics = painter->fontMetrics();
			QRect fontRect = metrics.tightBoundingRect(square_text.c_str());
			painter->setPen(text_color);
			if (ndefs.size() == 1)
				painter->drawText(QRect(0, 0, m_width, m_height), Qt::AlignCenter  | Qt::TextWordWrap, square_text.c_str());
			else 
				painter->drawText(QRect(0, 0, m_width, m_height*0.5), Qt::AlignCenter | Qt::TextWordWrap, square_text.c_str());
		}
		if (ndefs.size() == 2)
		{
			
			painter->drawLine(QPoint(m_width*0.05, m_height*0.5), QPoint(m_width*0.9, m_height*0.5));
			square_text = ext::string::toUpperCase(ndefs[1]);

			std::locale::global(std::locale(""));
			std::wcout.imbue(std::locale());
			auto& f = std::use_facet<std::ctype<wchar_t>>(std::locale());
			std::wstring str = ext::string::utf8_to_wstring(square_text);
			f.toupper(&str[0], &str[0] + str.size());
			
			square_text= ext::string::wstring_to_utf8(str);
		

			QFont font("Arial", 4, 3, false);
			painter->setFont(font);
			QFontMetrics metrics = painter->fontMetrics();
			QRect fontRect = metrics.tightBoundingRect(square_text.c_str());
			painter->setPen(text_color);
			painter->drawText(QRect(0, m_height*0.5, m_width, m_height*0.5), Qt::AlignCenter  | Qt::TextWordWrap, square_text.c_str());

		}
	}
	else
	{
		square_text = ext::string::toUpperCase(square_text);

		QFont font("Arial", 0, 12, false);
		painter->setFont(font);
		QFontMetrics metrics = painter->fontMetrics();
		QRect fontRect = metrics.tightBoundingRect(square_text.c_str());
		painter->setPen(text_color);
		painter->drawText(QPoint(m_width*0.5 - fontRect.width()*0.5, m_height*0.5 + fontRect.height()*0.5), square_text.c_str());
	}


	

}

