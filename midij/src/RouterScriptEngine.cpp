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

#include "RouterScriptEngine.h"
#include "Utils.h"

RouterScriptEngine::RouterScriptEngine()
	: MidiScriptEngine(NULL)
{
	//m_pEngine = new QScriptEngine(this);
	m_pEngine = NULL;
}

RouterScriptEngine::~RouterScriptEngine()
{
}

bool RouterScriptEngine::init(MidiObject* source, MidiObject* gatewayMidi, QObject* gatewayMapping)
{
    //engineGlobalObject.setProperty("midi", m_pEngine->newQObject(m_pMidiObject));

/*	if (source)
	{
		QScriptValue qScript = m_pEngine->newObject();
		QScriptValueIterator it(source->getMidiScriptEngine()->getGlobalObject());
		bool native = true;
		while (it.hasNext()) {
		    it.next();
		    if (it.flags() & QScriptValue::SkipInEnumeration)
		    	continue;
		    if (native && it.name() != "version")
		    	continue;

		    if (native)
		    {
		    	// skip "version"
		    	native = false;
		    	continue;
		    }

		    qDebug() << it.name() << "(" << it.flags() << "): " << it.value().toString();
		    qScript.setProperty(it.name(), it.value());
		}
		QScriptValue qSource = m_pEngine->newQObject(source);
		qSource.setProperty("script", qScript);//source->getMidiScriptEngine()->getGlobalObject(), QScriptValue::QObjectMember);
	    engineGlobalObject.setProperty("source", qSource);
	}*/

	m_pEngine = source->getMidiScriptEngine()->getEngine();
	//m_scriptEngineLock = source->getMidiScriptEngine()->m_scriptEngineLock;

	if (!m_pEngine)
		return false;

	QScriptValue engineGlobalObject = m_pEngine->globalObject();

	if (gatewayMidi)
	{
		QScriptValue gatewayObject = m_pEngine->newObject();
		gatewayObject.setProperty("midi", m_pEngine->newQObject(gatewayMidi));
		if (gatewayMapping)
			gatewayObject.setProperty("mapping", m_pEngine->newQObject(gatewayMapping));

		engineGlobalObject.setProperty("gateway", gatewayObject);

	    qDebug("Init MidiJ values");
	    // Init MidiJ values....
	    source->getMidiScriptEngine()->setValue("[MidiJ]", "version", MIDIJ_MAJORVERSION_HI + MIDIJ_MAJORVERSION_LO * 0.01);
	}
	return true;
}

void RouterScriptEngine::clear()
{
	if (m_pEngine)
	{
		QScriptValue engineGlobalObject = m_pEngine->globalObject();
		engineGlobalObject.setProperty("gateway", m_pEngine->nullValue());
		m_pEngine = NULL;
	}
}

