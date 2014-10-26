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

#ifndef MIDIMAPPINGGUI_H_
#define MIDIMAPPINGGUI_H_

#include <qdom.h>
#include <QtCore>
#include <qgraphicsitem.h>
#include "../mixxx/MidiMessage.h"
#include <QIcon>

class MidiSysexAction;
class GraphicsMidiControlItem;

class SchemeGuiHeader
{
public:
	SchemeGuiHeader(QString schemeFile);
//	SchemeGuiHeader(QString id, QString description);
	~SchemeGuiHeader();

	bool isValid();
	QString id();
	QString description();
	QIcon icon48(const QIcon& defaultIcon);

protected:
	QString m_id;
	QString m_description;
	QString m_iconFilePath;
	QIcon   m_icon;
};



class MidiMappingGui : public QObject
{
	Q_OBJECT
public:
	MidiMappingGui();
	virtual ~MidiMappingGui();

public:
	void connectMidiOutPort(const QObject *receiver, const char* receiverSlot, Qt::ConnectionType conType = Qt::AutoConnection);

	bool loadScheme(const QString& schemeFile, QList<QGraphicsItem*>& guiItems);
	bool loadGraphicItems(QDomElement root, QList<QGraphicsItem*>& guiItems);
	bool hasSourcePreset(const QString& name) const;
	QDomElement sourcePreset(QString name);

	GraphicsMidiControlItem* midiControl(const QString& name);
	QPixmap background();
	QRect toggleButtonRect(int defaultHeight);

	void init();
	void clear();

	// XML util methods
	static QDomElement openXMLFile(QString path, QString domName);
	static QString sourceFile(const QDomElement& root);

public slots:
	void midiShortMsgIn(MidiMessage inputCommand, char midivalue);
	void midiShortMsgIn(uchar statusByte, uchar midino, uchar midivalue);
	void midiSysexMsgIn(uchar *data, uint length);

signals:
	void midiSysexMsgOut(uchar *data, uint length);

public:
	Q_INVOKABLE void ctrlAllNotesOff();

protected:
	void emitMidiSysexMsgOut(const QByteArray& data);

	QDomElement    m_Bindings;
	QMultiHash<MidiMessage, GraphicsMidiControlItem*> m_inputMapping;
	QHash<QString, GraphicsMidiControlItem*> m_midiControlsMap;
	QMap<QString, MidiSysexAction*> m_midiSysexActionMap; // Key is ordered
	QList<QByteArray> m_sysexInitMsgList;
	QGraphicsPixmapItem* m_midiSysexBg;
	int            m_minSubIdLevel;
	int            m_maxSubIdLevel;
	QHash<QString, QString> m_sources;
	QHash<MidiMessage, const char*> m_controlMessages;
	QPixmap        m_background;
	QRect          m_toggleButtonRect;
	QList<GraphicsMidiControlItem*> m_midiControlsList;

	// connection object
	const QObject* m_receiver;
	const char*    m_receiverMember;

	friend class MidiSysexAction;
	friend class MidiSysexActionEx;
};


inline bool SchemeGuiHeader::isValid()
{
	return !m_id.isEmpty();
}

inline QString SchemeGuiHeader::id()
{
	return m_id;
}

inline QString SchemeGuiHeader::description()
{
	return m_description;
}

inline bool MidiMappingGui::hasSourcePreset(const QString& name) const
{
	return m_sources.contains(name);
}

inline GraphicsMidiControlItem* MidiMappingGui::midiControl(const QString& name)
{
	return m_midiControlsMap.value(name);
}

inline QPixmap MidiMappingGui::background()
{
	return m_background;
}

inline QRect MidiMappingGui::toggleButtonRect(int defaultHeight)
{
	if (!m_toggleButtonRect.isValid())
		return QRect(0, 0, defaultHeight, defaultHeight);
	return m_toggleButtonRect;
}


#endif /* MIDIMAPPINGGUI_H_ */
