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

#ifndef MIDIMAPPINGSerato_H_
#define MIDIMAPPINGSerato_H_

#include <qdom.h>
#include <qmutex.h>
#include "MidiMessageSerato.h"
#include <QtCore>

class MidiMappingSerato
{
public:
	MidiMappingSerato();
	virtual ~MidiMappingSerato();

public:
	bool loadPreset(QDomElement root);

	bool isOutputSeratoMessageMapped(const QString& seratoControl);
	MidiMessageSerato getOutputSeratoMessage(const QString& seratoControl);

    QMutex m_mappingLock;
    QDomElement m_Bindings;
    QHash<QString, MidiMessageSerato> m_outputMapping;

    QString getValueFromBindings(QString select, QString attrSelect);

    void clear();
protected:
    static QString selectNodeName(QString select);
    static QString selectAttrName(QString select);
    QString        m_cachedSelect;
    QDomElement    m_node;
};


#endif /* MIDIMAPPINGSerato_H_ */
