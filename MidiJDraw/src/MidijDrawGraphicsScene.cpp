/*
 * MidijDrawGraphicsScene.cpp
 *
 *  Created on: 13 de Abr de 2011
 *      Author: pt101821
 */

#include "MidijDrawGraphicsScene.h"
#include "qgraphicssceneevent.h"
#include "midij/gui/GraphicsMidiControlItem.h"
#include <QPainter>
#include <QCursor>


GraphicsSelectionItem::GraphicsSelectionItem()
{
	setZValue(1000);
	setVisible(false);
	setFlag(QGraphicsItem::ItemIsSelectable, false);
//    setFlag(QGraphicsItem::ItemIsMovable, true);
	setAcceptHoverEvents(true);
	m_isResizing = false;
	m_readOnly = true;
	setCacheMode(ItemCoordinateCache);
}

GraphicsSelectionItem::~GraphicsSelectionItem()
{
}

int GraphicsSelectionItem::type() const
{
	return GraphicsSelectionItem::Type;
}

void GraphicsSelectionItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	if (m_isResizing || isInResizeArea(event->pos()))
		setCursor(Qt::SizeFDiagCursor);
	else
		setCursor(Qt::ArrowCursor);
	QGraphicsRectItem::hoverMoveEvent(event);
}

bool GraphicsSelectionItem::isInResizeArea(const QPointF & pos)
{
	return (rect().left() < pos.x() - rect().width() + 9)
		&& (rect().top() < pos.y() - rect().height() + 9) && !m_readOnly;
}

void GraphicsSelectionItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (m_isResizing)
	{
		qreal dx = event->pos().x() - rect().x();
		qreal dy = event->pos().y() - rect().y();
		prepareGeometryChange();

		dx = qBound(MIN_ITEM_SIZE, dx, MAX_ITEM_SIZE);
		dy = qBound(MIN_ITEM_SIZE, dy, MAX_ITEM_SIZE);

		setRect(QRectF(rect().topLeft(), QSizeF(dx, dy)));
		emit rectChanged(rect());
	}
	else if ((event->buttons() & Qt::LeftButton) == Qt::LeftButton)
	{
		prepareGeometryChange();
		setRect(QRectF(rect().topLeft() + event->scenePos() - event->lastScenePos(), rect().size()));
		emit rectChanged(rect());

	}
	else
	{
		QGraphicsItem::mouseMoveEvent(event);
	}
}

void GraphicsSelectionItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isInResizeArea(event->pos()))
        m_isResizing = true;
    else if (m_readOnly)
        QGraphicsItem::mousePressEvent(event);
}

void GraphicsSelectionItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_isResizing)
        m_isResizing = false;
    else
        QGraphicsItem::mouseReleaseEvent(event);
}

void GraphicsSelectionItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	if (!m_readOnly)
		emit contextMenu(event->screenPos());
}


void GraphicsSelectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w)
{
	qDebug(__PRETTY_FUNCTION__);

	if (m_readOnly)
	{
		setPen(QPen(Qt::white));
		QGraphicsRectItem::paint(painter, option, w);
		return;
	}

	setPen(QPen(Qt::yellow));
	QGraphicsRectItem::paint(painter, option, w);

	QRectF rect = QGraphicsRectItem::rect();
	int bottom = rect.bottom();
	int right = rect.right();
    painter->drawLine(right - 9, bottom, right, bottom - 9);
    painter->drawLine(right - 6, bottom, right, bottom - 6);
    painter->drawLine(right - 3, bottom, right, bottom - 3);
}


/////////////////////////////////////////////////
//
MidijDrawGraphicsScene::MidijDrawGraphicsScene(QObject *parent)
	: QGraphicsScene(parent)
{
	// TODO Auto-generated constructor stub

}

MidijDrawGraphicsScene::~MidijDrawGraphicsScene()
{
	// TODO Auto-generated destructor stub
}


bool MidijDrawGraphicsScene::event(QEvent *event)
{
	if (event->type() == QEvent::GraphicsSceneMousePress)
	{
		QGraphicsItem* item = itemAt(((QGraphicsSceneMouseEvent*)event)->scenePos());
		if (item && item->type() == GraphicsMidiControlItem::Type)
		{
			emit itemClicked(item);
		}
	}

	return QGraphicsScene::event(event);
}


/*
 *
 *
ItemBase::ItemBase(int size, int x, int y) : m_size(size), m_isResizing(false)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setAcceptHoverEvents(true);
    setPos(x, y);
    m_startTime = QTime::currentTime();
}
*/


/*
 * Alterar o tamanho do Item com o rato
 *
void ItemBase::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_isResizing) {
        int dx = int(2.0 * event->pos().x());
        int dy = int(2.0 * event->pos().y());
        prepareGeometryChange();
        m_size = (dx > dy ? dx : dy);
        if (m_size < MIN_ITEM_SIZE)
            m_size = MIN_ITEM_SIZE;
        else if (m_size > MAX_ITEM_SIZE)
            m_size = MAX_ITEM_SIZE;
    } else {
        QGraphicsItem::mouseMoveEvent(event);
    }
}
*/

/*
 * Alterar o rato cursor para Resize "<->"
 *
void ItemBase::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if (m_isResizing || (isInResizeArea(event->pos()) && isSelected()))
        setCursor(Qt::SizeFDiagCursor);
    else
        setCursor(Qt::ArrowCursor);
    QGraphicsItem::hoverMoveEvent(event);
}
*/




/*
 *  Desenhar 1 Rect Amarelo quando o Item é seleccionado e com 1grip (canto inferior direito) para activar o Resize
 *   (tem cor branca quando a janela perde o Focus)
 *
void ItemBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    if (option->state & QStyle::State_Selected) {
        painter->setRenderHint(QPainter::Antialiasing, true);
        if (option->state & QStyle::State_HasFocus)
            painter->setPen(Qt::yellow);
        else
            painter->setPen(Qt::white);
        painter->drawRect(boundingRect());

        painter->drawLine(m_size / 2 - 9, m_size / 2, m_size / 2, m_size / 2 - 9);
        painter->drawLine(m_size / 2 - 6, m_size / 2, m_size / 2, m_size / 2 - 6);
        painter->drawLine(m_size / 2 - 3, m_size / 2, m_size / 2, m_size / 2 - 3);

        painter->setRenderHint(QPainter::Antialiasing, false);
    }
}
*/
