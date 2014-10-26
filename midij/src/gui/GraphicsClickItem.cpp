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

#include "GraphicsClickItem.h"
#include <QGraphicsSceneEvent>
#include <QDebug>

GraphicsClickItem::GraphicsClickItem(QPixmap background, QPixmap hilight, QRect sceneRect)
	: QGraphicsPixmapItem(background),
	  m_sceneRect(sceneRect)
{
	setCacheMode(DeviceCoordinateCache);
	setPos(m_sceneRect.topLeft());
	setZValue(1);
	m_background = background;
	m_hilight = hilight;
}

GraphicsClickItem::~GraphicsClickItem()
{
}
void GraphicsClickItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	Q_UNUSED(event);
//	qDebug() << " GraphicsClickItem::mousePressEvent ";
	if (!m_hilight.isNull())
		setPixmap(m_hilight);
}

void GraphicsClickItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (m_hilight.isNull())
		return;
	if (m_sceneRect.contains(event->scenePos()))
	{
		if (pixmap().cacheKey() != m_hilight.cacheKey())
			setPixmap(m_hilight);
	}
	else
	{
		if (pixmap().cacheKey() != m_background.cacheKey())
			setPixmap(m_background);
	}
}

void GraphicsClickItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
//	qDebug() << " GraphicsClickItem::mouseReleaseEvent ";
	if (m_sceneRect.contains(event->scenePos()))
	{
		if (!m_hilight.isNull())
			setPixmap(m_background);
		emit(clicked());
	}
}
