#include "QCrosswordView.h"

QCrosswordView::QCrosswordView(QWidget* parent, bool useOpenGL) :
	QGraphicsView(parent)
{
	// We want the images to be scaled with linear interpolation and nice antialiasing
	//this->setRenderHints( QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


	m_Scene = new QCrosswordScene(this);
	m_Scene->setItemIndexMethod(QGraphicsScene::NoIndex);

	int w = this->width();
	int h = this->height();
	m_Scene->setSceneRect(0, 0, w, h);

	
	this->setScene(m_Scene);

	//m_Scene->createCrossword(20, 20, 40, 40);
	setViewportUpdateMode(FullViewportUpdate);

	// para poder arrastrar una vez hecho zoom //
	setTransformationAnchor(AnchorUnderMouse);
	setDragMode(QGraphicsView::ScrollHandDrag);
	setRenderHint(QPainter::Antialiasing);
	scale(qreal(1.0), qreal(1.0));
	//this->setInteractive(true);
	//scale(qreal(0.2), qreal(0.2));
	//scaleView(0.5);
	setMinimumSize(400, 400);



}

void QCrosswordView::SetSceneSize(const QSize &size)
{
	if (m_Scene) m_Scene->setSceneRect(0, 0, size.width(), size.height());
}
QSize QCrosswordView::GetSceneSize()
{
	if (!m_Scene) return QSize(0, 0);

	return QSize(this->width(), this->height());

}

void QCrosswordView::wheelEvent(QWheelEvent *event)
{
	if (QApplication::keyboardModifiers() & Qt::ControlModifier)
		scaleView(pow((double)2, event->delta() / 240.0));  // forward in backward out
															//scaleView(pow((double)2,  - event->delta() / 240.0)); // forward out backward in
	else
	{
		//emit siAdvanceFrames(event->delta() / 120);
	}
}

void QCrosswordView::scaleView(qreal scaleFactor)
{
	qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (factor < 0.07 || factor > 100)
		return;

	scale(scaleFactor, scaleFactor);
}

void QCrosswordView::zoomIn()
{
	scaleView(qreal(1.2));
}

void QCrosswordView::zoomOut()
{
	scaleView(1 / qreal(1.2));
}

void QCrosswordView::resizeEvent(QResizeEvent * event)
{
	int scene_width, scene_height;
	scene_width = m_Scene->width();
	scene_height = m_Scene->height();


	// tamaño de la escena
	//printf("%d,%d\n\n",scene_width,scene_height);
	//printf("%d,%d\n\n",this->viewport()->width(),this->viewport()->height());
	//printf("%d,%d\n\n",width(),height());

	// mapFromScene : como se está viendo el scenerect en la ventana
	QPolygonF polygon = mapFromScene(scene()->sceneRect());
	if (polygon.boundingRect().width() < frameSize().width())
	{
		QGraphicsView::fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
	}

	QGraphicsView::resizeEvent(event);

}
void QCrosswordView::init()
{

	QGraphicsView::fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);

}

void QCrosswordView::drawForeground(QPainter *painter, const QRectF &rect)
{
	//-- Draw over the canvas of the QGraphicsView --//
	//-- If you set a background brush in the scene then drawForeground in the view is called instead of drawForeground in the scenes --//

	//emit siPaintAfterItems(painter);

}

void QCrosswordView::paintEvent(QPaintEvent * event)
{
	QGraphicsView::paintEvent(event);

	QPainter painter(this->viewport());

	// Emit in case you want to draw in the view, over the scene 
	//emit siPaintView(painter);

}

void QCrosswordView::createGrid(int rows, int cols, int size) {

}
void QCrosswordView::createCrosswordinScene(int idx_data, int size) {

	m_Scene->createGraphicalCrossword(idx_data, size,size);
}

void QCrosswordView::keyPressEvent(QKeyEvent *event)
{
	if (!(QApplication::keyboardModifiers() & Qt::ControlModifier))
	{
		//if control is not pressed arrows will be used to move in the word
		switch (event->key())
		{
		case Qt::Key_Space:case Qt::Key_Left: case Qt::Key_Right: case Qt::Key_Up: case Qt::Key_Down:
		case Qt::Key::Key_A: case Qt::Key::Key_Aacute:
		case Qt::Key::Key_E: case Qt::Key::Key_Eacute:
		case Qt::Key::Key_I: case Qt::Key::Key_Iacute:
		case Qt::Key::Key_O: case Qt::Key::Key_Oacute:
		case Qt::Key::Key_U: case Qt::Key::Key_Uacute:
		case Qt::Key::Key_B: case Qt::Key::Key_C: case Qt::Key::Key_D: case Qt::Key::Key_F: case Qt::Key::Key_G: case Qt::Key::Key_H:
		case Qt::Key::Key_J: case Qt::Key::Key_K: case Qt::Key::Key_L: case Qt::Key::Key_M: case Qt::Key::Key_N: case Qt::Key::Key_Ntilde:
		case Qt::Key::Key_P: case Qt::Key::Key_Q: case Qt::Key::Key_R: case Qt::Key::Key_S: case Qt::Key::Key_T: case Qt::Key::Key_V:
		case Qt::Key::Key_W: case Qt::Key::Key_X: case Qt::Key::Key_Y: case Qt::Key::Key_Z:
			{
				event->ignore();
				return;
			}
		}
	}
		
	QGraphicsView::keyPressEvent(event);
}