/*
 * MidijDrawGraphicsScene.h
 *
 *  Created on: 13 de Abr de 2011
 *      Author: pt101821
 */

#ifndef MIDIJDRAWGRAPHICSSCENE_H_
#define MIDIJDRAWGRAPHICSSCENE_H_

#include <QGraphicsScene>
#include <QtGui/QGraphicsRectItem>


#define MIN_ITEM_SIZE	2.0
#define MAX_ITEM_SIZE	480.0


class GraphicsSelectionItem : public QObject, public QGraphicsRectItem
{
	Q_OBJECT

public:
	GraphicsSelectionItem();
	~GraphicsSelectionItem();

signals:
	void rectChanged(const QRectF &rect);
	void contextMenu(const QPointF &scenePos);

public:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w);

	enum { Type = QGraphicsItem::UserType + 1000 };
	int type() const;

	bool m_isResizing;
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	bool isInResizeArea(const QPointF & pos);

	bool m_readOnly;
};


class MidijDrawGraphicsScene : public QGraphicsScene
{
	Q_OBJECT

public:
	MidijDrawGraphicsScene(QObject *parent = 0);
	virtual ~MidijDrawGraphicsScene();

signals:
	void itemClicked(QGraphicsItem *);

protected:
    bool event(QEvent *event);


};

#endif /* MIDIJDRAWGRAPHICSSCENE_H_ */
