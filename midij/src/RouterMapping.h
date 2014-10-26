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

#ifndef ROUTERMAPPING_H_
#define ROUTERMAPPING_H_

#include <QtCore>
#include <qdom.h>
#include "mixxx/MidiMessage.h"
#include "serato/SeratoControl.h"
class RouterScriptEngine;



class RouterMapping
{
public:
	RouterMapping();
	virtual ~RouterMapping();

public:
	void setScriptEngine(RouterScriptEngine* engine);
	bool loadPreset(QDomElement root);
	bool isMidiMessageMapped(const MidiMessage& inputCommand);
	SeratoControl getOutputSeratoControl(const MidiMessage& inputCommand);

	void callInit();

	void clear();

protected:
	void addScriptFile(QString filename, QString functionprefix);

	QDomElement    m_Bindings;
	QHash<MidiMessage, SeratoControl> m_routingTable;

    QList<QString> m_pScriptFileNames;
    QList<QString> m_pScriptFunctionPrefixes;
    RouterScriptEngine *m_pScriptEngine;

};

#endif /* ROUTERMAPPING_H_ */
