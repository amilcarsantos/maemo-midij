/*
 * MidiJ - Midi controller for DJs
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

#ifndef MIDISYSEXACTION_H_
#define MIDISYSEXACTION_H_

#include <qdom.h>
#include <QHash>
#include "MidiSysexProperties.h"
#include "GraphicsMidiControlItem.h"
class MidiMappingGui;


class MidiSysexAction
{
public:
	MidiSysexAction();
	MidiSysexAction(QDomElement headerNode, QDomElement messageNode, MidiMappingGui* mapping, const PixmapMap& pixmapMap);
	virtual ~MidiSysexAction();

	bool isValid();
	QString getMapKey();
	virtual void run(QString data);
	virtual bool isUserDefined();
	QGraphicsPixmapItem* background();

	static QString makeMapKey(const uchar data[], int Level);
	static QByteArray parseId(const QString& text);

protected:
	void addSubId(QDomElement subId);
	QByteArray     m_subIds;
	bool           m_validId;
	MidiMappingGui* m_mapping;
	QHash<QString, QString> m_groupAction;
	QHash<QString, MidiSysexProperties*> m_groupProperties;
	QGraphicsPixmapItem* m_bgItem;
};

inline bool MidiSysexAction::isUserDefined()
{
	return true;
}

inline QGraphicsPixmapItem* MidiSysexAction::background()
{
	return m_bgItem;
}


class MidiSysexActionEx : public MidiSysexAction
{
public:
	MidiSysexActionEx();
	MidiSysexActionEx(QByteArray subIds, QByteArray replyData, MidiMappingGui* mapping);
	virtual ~MidiSysexActionEx();

	virtual void run(QString data);
	virtual bool isUserDefined();
private:
	QByteArray  m_replyData;
};

inline bool MidiSysexActionEx::isUserDefined()
{
	return false;
}

#endif /* MIDISYSEXACTION_H_ */
