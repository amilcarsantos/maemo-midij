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

#include "GraphicsMidiInputItem.h"
#include <QDebug>
#include <QPainter>
#include "GuiUtils.h"

// XML tags
#define INPUT_STATE					"state"
#define INPUT_STATE_RANGEMARKER		"-"


GraphicsMidiInputItem::GraphicsMidiInputItem(QDomElement controlNode, const PixmapMap& pixmapMap)
{
	// map a midi message to a
	m_inputMidiMessages.append(MidiMessage(controlNode));

	m_name = controlNode.attribute(CONTROL_NAME, "");
	m_groupName = controlNode.attribute(CONTROL_GROUP, "");
	QString debugColor = controlNode.attribute("debug");

	GraphicsMidiArea area = parseAreaElement(controlNode);
	if (area.isNull())
		return;

	QString mask = parsePixmapMask(controlNode);

	setSceneRect(area.rect);
	bool hasRanged = false;

	// get states
	QDomElement stateNode = controlNode.firstChildElement("state");
	int rangeMarker;
	QPixmap backgroundPixmap = pixmapFromMap(GuiUtils::background, m_sceneRect, pixmapMap, mask);
	ushort minRangeId, maxRangeId;
	while (!stateNode.isNull())
	{
		QString state = stateNode.text();
		QString stateValue = stateNode.attribute("value").trimmed();

		// map range to m_state2Pixmap
		rangeMarker = stateValue.indexOf(INPUT_STATE_RANGEMARKER);
		if (rangeMarker > 0)
		{
			hasRanged = true;
			bool ok = false;
			//qDebug() << state << stateValue  << rangeMarker << stateValue.left(rangeMarker) << stateValue.mid(rangeMarker + 1);
			minRangeId = stateValue.left(rangeMarker).toUShort(&ok, 0);
			if (ok)
				maxRangeId = stateValue.mid(rangeMarker + 1).toUShort(&ok, 0);
			if (!ok)
			{
				qCritical() << "error parsing:" << stateValue << "; line:" << stateNode.lineNumber();
				return;
			}

			QPixmap pixmap = pixmapFromState(stateNode, backgroundPixmap, m_sceneRect, pixmapMap, mask);
			if (!pixmap.isNull())
			{
				checkDebug(debugColor, pixmap, area);
				// map range to m_state2Pixmap
				m_state2Pixmap.insert(maxRangeId, pixmap);
				m_range2State.insert(minRangeId, maxRangeId); // add ranged value
			}
		}
		else
		{
			bool ok = false;
			ushort stateId = stateValue.toUShort(&ok, 0);
			if (pixmapMap.contains(state) && ok)
			{
//				QPixmap pixmap = pixmapMap.value(state).copy(rect);
				QPixmap pixmap = pixmapFromState(stateNode, backgroundPixmap, m_sceneRect, pixmapMap, mask);
				checkDebug(debugColor, pixmap, area);

				m_state2Pixmap.insert(stateId, pixmap);
				m_range2State.insert(stateId, stateId); // support for ranged values
				if (GuiUtils::isBackground(state))
				{
					m_offState = stateId;
					setPixmap(pixmap);
				}
			}
		}
		stateNode = stateNode.nextSiblingElement("state");
	}

	if (!hasRanged)
		m_range2State.clear(); // no ranged values after all...

/*	QPixmap pixmap = pixmapMap[GuiUtils::background];

if (0) {
	setPixmap(pixmap.copy(rect));
} else {
QPixmap tmp = pixmap.copy(rect);
QPainter dc(&tmp);
dc.setPen(Qt::green);
dc.drawRect(0,0,rect.width()-1, rect.height()-1);
setPixmap(tmp);
}
*/
	setPos(m_sceneRect.topLeft());
	setZValue(2);

	m_hasInput = true;
}

GraphicsMidiInputItem::~GraphicsMidiInputItem()
{
}

void GraphicsMidiInputItem::mergeInto(GraphicsMidiControlItem* target)
{
	target->m_inputMidiMessages.append(m_inputMidiMessages);
	target->m_state2Pixmap.unite(m_state2Pixmap);
	target->m_hasInput = m_hasInput;

	if (target->m_sceneRect.isEmpty())
	{
		target->m_sceneRect = m_sceneRect;
	}
	else
	{
		target->m_sceneRect.unite(m_sceneRect);
	}
}

/**
 * Extracts an pixmap from the map of pixmaps
 */
QPixmap GraphicsMidiInputItem::pixmapFromState(const QDomElement& stateNode, QPixmap bgPixmap, QRect sourceRect, const PixmapMap& pixmapMap, const QString& mask)
{
	QString state = stateNode.text();
	// get the new block
	GraphicsMidiArea rangedArea = parseAreaAttribute(stateNode);
	// TODO mask from attribute

	if (rangedArea.isNull())
	{
		return pixmapFromMap(state, sourceRect, pixmapMap, mask);
	}
	QRect rangedSrcRect = rangedArea.rect;
	rangedSrcRect.translate(sourceRect.topLeft());

	//qDebug() << rangedArea << rangedSrcRect;
	// merge the "background" with the new block
	if (pixmapMap.contains(state) && !bgPixmap.isNull())
	{
		QPixmap pixmap = bgPixmap;
		QPainter painter(&pixmap);
		painter.drawPixmap(rangedArea.rect, pixmapMap.value(state), rangedSrcRect);
		return pixmap;
	}
	return QPixmap();
}

