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

#include "MidiSysexAction.h"
#include "MidiMappingGui.h"

// XML tags
#define SYSEX_INPUT_SUBID			"subid"
#define SYSEX_INPUT_MANUFACTURERID	"ManufacturerID"
#define SYSEX_INPUT_ACTION			"action"
#define SYSEX_INPUT_ACTION_CONTROLS "controls"
#define SYSEX_INPUT_ACTION_GROUP 	"group"
#define SYSEX_INPUT_ACTION_BG		"background"


MidiSysexAction::MidiSysexAction()
{
	m_mapping = NULL;
	m_bgItem = NULL;
}

MidiSysexAction::MidiSysexAction(QDomElement headerNode, QDomElement messageNode, MidiMappingGui* mapping, const PixmapMap& pixmapMap)
{
	m_validId = true;
	m_bgItem = NULL;
	if (!headerNode.isNull())
	{
		addSubId(headerNode.firstChildElement(SYSEX_INPUT_MANUFACTURERID));
		addSubId(messageNode.firstChildElement(SYSEX_INPUT_SUBID));
	}
	else
		addSubId(messageNode.firstChildElement(SYSEX_INPUT_MANUFACTURERID));


	if (m_validId && m_subIds.count() == 0)
	{
		m_validId = false;
		return;
	}

	QDomElement actionNode = messageNode.firstChildElement(SYSEX_INPUT_ACTION);
	if (!actionNode.isNull())
	{
		QDomElement controlsNode = actionNode.firstChildElement(SYSEX_INPUT_ACTION_CONTROLS);
		while (!controlsNode.isNull())
		{
			m_groupAction.insert(controlsNode.attribute(SYSEX_INPUT_ACTION_GROUP), controlsNode.text());
			controlsNode = controlsNode.nextSiblingElement(SYSEX_INPUT_ACTION_CONTROLS);
		}
		//actionNode = actionNode.nextSiblingElement(SYSEX_INPUT_ACTION);
		QDomElement bgNode = actionNode.firstChildElement(SYSEX_INPUT_ACTION_BG);
		if (!bgNode.isNull())
		{
			QString pixmapName = bgNode.text();
			GraphicsMidiArea area = GraphicsMidiControlItem::parseAreaAttribute(bgNode);
			if (pixmapMap.contains(pixmapName))
			{
				m_bgItem = new QGraphicsPixmapItem(GraphicsMidiControlItem::pixmapFromMap(pixmapName, area.rect, pixmapMap, ""));
				m_bgItem->setZValue(0.5);
				m_bgItem->setPos(area.rect.topLeft());
				m_bgItem->setVisible(false);
			}
		}
	}
	m_mapping = mapping;
}

QByteArray MidiSysexAction::parseId(const QString& text)
{
	bool ok = false;
	QStringList subIdList;
	QByteArray id;

	if (text.indexOf(',') > 0)
		subIdList = text.split(',');
	else
		subIdList.append(text);

	foreach(QString subId, subIdList)
	{
		int subIdNum = subId.toInt(&ok, 0);
		if (!ok)
		{
			return QByteArray();
		}
		id.append(subIdNum);
	}
	return id;
}

void MidiSysexAction::addSubId(QDomElement subId)
{
	while (!subId.isNull())
	{
		bool ok = false;
		QString text = subId.text();
		QStringList subIdList;

		if (text.indexOf(',') > 0)
			subIdList = text.split(',');
		else
			subIdList.append(text);

		foreach(QString subId, subIdList)
		{
			int id = subId.toInt(&ok, 0);
			if (!ok)
			{
				m_validId = false;
				return;
			}
			m_subIds.append(id);
		}
		subId = subId.nextSiblingElement(SYSEX_INPUT_SUBID);
	}
}

MidiSysexAction::~MidiSysexAction()
{
	qDeleteAll(m_groupProperties.values());
}


bool MidiSysexAction::isValid()
{
	return m_validId;
}

QString MidiSysexAction::getMapKey()
{
	if (m_validId)
		return makeMapKey(reinterpret_cast<const uchar*>(m_subIds.data()), m_subIds.size() - 1);
	return QString();
}


void MidiSysexAction::run(QString data)
{
	qDebug("running SYSEX action...");
	foreach(QString key, m_groupAction.keys())
		qDebug() << key << m_groupAction[key];

	foreach(GraphicsMidiControlItem* item, m_mapping->m_midiControlsMap.values())
	{
		if (m_groupAction.contains(item->groupName()))
		{
			MidiSysexProperties* props = m_groupProperties.value(item->groupName());
			if (!props)
			{
				props = new MidiSysexProperties(m_groupAction.value(item->groupName()));
				m_groupProperties.insert(item->groupName(), props);
			}
			if (props->isValid())
				props->apply(item);
			qDebug() << "item:" << item->name() << "; visible: " << item->isVisible();
		}

/*		//TODO: zzz; action for individual items
		if (m_itemActions.contains(item->name()))
		{
			qWarning("MidiSysexAction::run()... m_itemActions.contains(item->name()))");
		}*/
	}
}

QString MidiSysexAction::makeMapKey(const uchar data[], int level)
{
	QString key;
	if (data == NULL || level < 0)
		return key;
	key = QString("%1").arg(data[0], 2, 16, QChar('0'));
	for (int i = 1; i <= level; i++)
	{
		key += QString("-%1").arg(data[i], 2, 16, QChar('0'));
	}
	return key;
}


// Internal SysEx messages
MidiSysexActionEx::MidiSysexActionEx()
{
}

MidiSysexActionEx::MidiSysexActionEx(QByteArray subIds, QByteArray replyData, MidiMappingGui* mapping)
{
	m_subIds = subIds;
	m_validId = true;
	m_mapping = mapping;
	m_replyData = replyData;
}

MidiSysexActionEx::~MidiSysexActionEx()
{
}

void MidiSysexActionEx::run(QString data)
{
	Q_UNUSED(data)
	qDebug() << "emiting MidiSysex reply";
	m_mapping->emitMidiSysexMsgOut(m_replyData);
}


