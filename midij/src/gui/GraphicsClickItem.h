/*
 * MidiJ - Remote MIDI controller for DJs
 * Copyright (C) 2010  Amilcar Santos <amilcar.santos@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef GRAPHICSCLICKITEM_H_
#define GRAPHICSCLICKITEM_H_

#include <QtGui/QGraphicsPixmapItem>
#include <QtCore/qobject.h>


class GraphicsClickItem : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
	Q_PROPERTY(QPointF pos READ pos WRITE setPos)
	Q_PROPERTY(qreal x READ x WRITE setX)
	Q_PROPERTY(qreal y READ y WRITE setY)

public:
	GraphicsClickItem(QPixmap background, QPixmap hilight, QRect sceneRect);
	virtual ~GraphicsClickItem();

signals:
	void clicked();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    QRectF  m_sceneRect;
    QPixmap m_background;
    QPixmap m_hilight;
};

#endif /* GRAPHICSCLICKITEM_H_ */
