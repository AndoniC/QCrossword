#include "QCrosswordScene.h"

QCrosswordScene::QCrosswordScene(QGraphicsView* parent) : QGraphicsScene(parent)
{
	connect(&m_info_widget, SIGNAL(update()), this, SLOT(sltUpdate()));
}

QCrosswordScene::~QCrosswordScene()
{
	m_info_widget.close();
}

void QCrosswordScene::drawBackground(QPainter *painter, const QRectF &rect)
{
	//painter.drawTiledPixmap ( QRect(pixmap_pos.x(),pixmap_pos.y(),m_pixmap.width(),this->height()-pixmap_pos.y()),m_pixmap,QPoint(0,0));
	//painter.drawPixmap ( QRect(pixmap_pos.x(),pixmap_pos.y(),m_pixmap.width(),this->height()-pixmap_pos.y()),m_pixmap,QRect(0,0,m_pixmap.width(),m_pixmap.height()));
	Q_UNUSED(rect);

//	painter->drawPixmap(0, 0, this->backgroundBrush().texture());

	QPainterPath path;
	path.addRect(0, 0, width(), height());
	painter->fillPath(path, Qt::gray);

	QPainterPath path1;
	path1.addRect(0, 0, 10, 10);
	QPen pen(Qt::black, 10);

	painter->fillPath(path1, Qt::red);
	painter->drawPath(path1);
	

	//emit siPaintAfterBackground(painter);


}
void QCrosswordScene::drawForeground(QPainter * painter, const QRectF & rect)
{
	//QGraphicsScene::drawForeground(painter, rect);

//	emit siPaintAfterItems(painter);
}

void QCrosswordScene::createGraphicalCrossword(int idx_data, int h, int w)
{

	DataManager::crossword_data_t *cd = DataManager::getCrossword(idx_data);

	if (cd)
	{
		CrossWord::crossword_description_t  desc = cd->data.getDescription();
		squaregrid.resize(desc.rows);
		cd->graphical_data.resize(desc.rows);
		for (int i = 0; i < desc.rows; i++)
		{
			squaregrid[i].resize(desc.cols);
			cd->graphical_data[i].resize(desc.cols);
			for (int j = 0; j < desc.cols; j++)
			{
				KeyTile * square = cd->data.getKeySquare(i, j);
				if (square)
					squaregrid[i][j] = new QCrosswordSquare(j, i, w, h,0);
				else 
					squaregrid[i][j] = new QCrosswordSquare(j, i, w, h,1);

				this->addItem(squaregrid[i][j]);
				squaregrid[i][j]->setSize(w, h);
				squaregrid[i][j]->setPos(j*w, i*h);
				cd->graphical_data[i][j] = squaregrid[i][j];
			}
		}
		this->setSceneRect(0, 0, desc.cols*w, desc.rows*h);
	}

}

void QCrosswordScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

	//Scene receives mouse events before items, so if Alt is pressed event is not processed and Viewer will manage it
	//if (QGuiApplication::keyboardModifiers().testFlag(Qt::AltModifier))
	//{
		
	//	return;
	//}

	//QGraphicsScene::mousePressEvent(event);
}

void QCrosswordScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	//Scene receives mouse events before items, so if Alt is pressed event is not processed and Viewer will manage it
	if (QGuiApplication::keyboardModifiers().testFlag(Qt::AltModifier))
	{

		return;
	}

	// set all tiles belonging to the word in the colum or row as semiselected
	QGraphicsItem *item = itemAt(mouseEvent->scenePos(), QTransform());// it is your clicked item, you can do everything what you want. for example send it somewhere
	QCrosswordSquare *ell = qgraphicsitem_cast<QCrosswordSquare *>(item);
	cv::Point map_pos = cv::Point(-1, -1);
	if (ell)
	{

		 map_pos = ell->getIds();



		changeTo(map_pos.x, map_pos.y);



		//ell->setBrush(QBrush(Qt::black));
	}

	if (mouseEvent->button() == Qt::LeftButton)
	{
	
		QGraphicsScene::mouseReleaseEvent(mouseEvent);
	}
	else if (mouseEvent->button() == Qt::RightButton)
	{
		if (StatusManager::is(StatusManager::ST_EDITING))
		{
			KeyTile* kt = DataManager::getCrossword()->data.getKeySquare(map_pos.y, map_pos.x);
			if (kt)
				m_info_widget.fill(kt);
			m_info_widget.show();
		}

	}
}


void QCrosswordScene::changeTo(int pos_x, int pos_y)
{
	if (!DataManager::getCrossword()) return;

	if (pos_x == -1 || pos_y == -1)
	{
		pos_x = DataManager::getCurrentTile().x;
		pos_y = DataManager::getCurrentTile().y;
	}
	QCrosswordSquare *ell = DataManager::getCrossword()->graphical_data[pos_y][pos_x];
	if (!ell) return;

	this->clearSelection();
	clearSemiSelected(); 
	
	ell->setSelected(true);

	// set current tile in datamanager
	DataManager::setCurrentTile(pos_x, pos_y);
	std::cout << "Current Tile: " << pos_x << " , " << pos_y << std::endl;

	if (DataManager::getCrossword()->data.empty()) return;

	NormalTile* nt = dynamic_cast<NormalTile*>(DataManager::getCrossword()->data.getTile(pos_y,pos_x));
	

	

	if (nt && nt->isValid)
	{


		if (DataManager::getCurrentDirection() == GAME_DIRECTION::VERTICAL)
		{
			setSemiSelected(nt->first_vertical_tile, nt->last_vertical_tile);
		}
		else
		{
			setSemiSelected(nt->first_horizontal_tile, nt->last_horizontal_tile);
		}

	}

}
void QCrosswordScene::clearSemiSelected()
{
	QList<QGraphicsItem *> items = this->items();
	QList<QGraphicsItem *>::iterator i;
	for (i = items.begin(); i != items.end(); ++i)
	{
		QCrosswordSquare *tile = dynamic_cast<QCrosswordSquare*>(*i);
		if (tile) tile->setSemiSelected(false);
	}
}

void QCrosswordScene::setSemiSelected(cv::Point from, cv::Point to)
{
	if (from.x == to.x)
	{
		for (int i = from.y; i < to.y; i++)
		{
				QCrosswordSquare* cs = DataManager::getCrossword()->graphical_data[i][from.x];
				if (cs)
				{
					cs->setSemiSelected(true);
				}
		}
	}
	else
	{
		for (int j = from.x; j < to.x; j++)
		{
			QCrosswordSquare* cs = DataManager::getCrossword()->graphical_data[from.y][j];
			if (cs)
			{
				cs->setSemiSelected(true);
			}
		}
	}
	this->update();
}

void QCrosswordScene::sltUpdate()
{
	this->update();

}