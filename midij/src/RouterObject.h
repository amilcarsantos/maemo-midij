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

#ifndef ENGINE_H_
#define ENGINE_H_

#include "mixxx/MidiObject.h"
#include "mixxx/MidiMapping.h"
#include "serato/MidiMappingSerato.h"
#include "RouterMapping.h"
#include "RouterScriptEngine.h"
#include <qdom.h>

class RouterObject : public QObject
{
	Q_OBJECT

public:
	RouterObject();
	virtual ~RouterObject();

public:
	bool loadMappings(QDomElement routerXml, QDomElement seratoXml);
	void setMidiObjects(MidiObject* source, MidiObject* gateway);
	RouterMapping* getRouterMapping();

	void clear();
	int lastError();

	// "mappings" Script Object
    Q_INVOKABLE QString getValue(QString group, QString name);

public slots:
	// GUI/Engine -> Router.Source
	void midiSourceReceive(MidiMessage inputCommand, char value);
	void sourceReload();

	// Gateway -> Network
	void midiGatewayShortMsgOut(uchar statusByte, uchar midino, uchar midivalue);
	void midiGatewaySysexMsgOut(uchar* data,uint size);

public:
	RouterMapping  m_routerMapping;
	MidiMappingSerato m_seratoMapping;
	MidiObject*    m_sourceMidiObject;
	MidiObject*    m_gatewayMidiObject;
	RouterScriptEngine* m_pEngine;
	int            m_lastError;
};

inline int RouterObject::lastError()
{
	return m_lastError;
}

inline RouterMapping* RouterObject::getRouterMapping()
{
	return &m_routerMapping;
}


#endif /* ENGINE_H_ */
