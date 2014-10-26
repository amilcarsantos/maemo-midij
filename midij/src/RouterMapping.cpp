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

#include "RouterMapping.h"
#include "RouterScriptEngine.h"

// XML tags
#define SOURCE			"Source"
#define SOURCE_CONTROL	"control"


RouterMapping::RouterMapping()
{
	m_pScriptEngine = NULL;
}

RouterMapping::~RouterMapping()
{
}

void RouterMapping::setScriptEngine(RouterScriptEngine* engine)
{
	m_pScriptEngine = engine;
}

bool RouterMapping::loadPreset(QDomElement root)
{
    // For each controller in the DOM
    m_Bindings = root;

    QDomElement srcControl = m_Bindings.firstChildElement(SOURCE).firstChildElement(SOURCE_CONTROL);

    //Iterate through each <control> block in the XML
    while (!srcControl.isNull())
    {
        //Unserialize these objects from the XML
        MidiMessage midiMessage(srcControl);
        SeratoControl seratoControl(srcControl, true);

        //Add to the output mapping.
        m_routingTable.insert(midiMessage, seratoControl);

        srcControl = srcControl.nextSiblingElement(SOURCE_CONTROL);
    }

    if (m_routingTable.isEmpty())
    {
    	qWarning() << "RouterMapping: mapping is invalid!";
    	return false;
    }

    if (m_pScriptEngine == NULL)
    {
    	qWarning("Missing Script Engine");
    	return false;
    }
    // Get a list of MIDI script files to load
    QDomElement scriptFile = m_Bindings.firstChildElement("scriptfiles").firstChildElement("file");

    // Look for additional ones
    while (!scriptFile.isNull()) {
        QString functionPrefix = scriptFile.attribute("functionprefix","");
        QString filename = scriptFile.attribute("filename","");
        addScriptFile(filename, functionPrefix);

        scriptFile = scriptFile.nextSiblingElement("file");
    }

    // Load Script files
    qDebug("MidiMapping: Loading & evaluating all MIDI script code");
    QListIterator<QString> it(m_pScriptFileNames);
    while (it.hasNext()) {
        QString curScriptFileName = it.next();
        m_pScriptEngine->evaluate(curScriptFileName);

        if(m_pScriptEngine->hasErrors(curScriptFileName)) {
            qDebug() << "Errors occured while loading " << curScriptFileName;
        }
    }
    qDebug() << "RouterMapping: mapping parsed! size:" << m_routingTable.size();
    return true;
}

void RouterMapping::addScriptFile(QString filename, QString functionprefix)
{
    m_pScriptFileNames.append(filename);
    m_pScriptFunctionPrefixes.append(functionprefix);
}


bool RouterMapping::isMidiMessageMapped(const MidiMessage& inputCommand)
{
	return m_routingTable.contains(inputCommand);
}

SeratoControl RouterMapping::getOutputSeratoControl(const MidiMessage& inputCommand)
{
	return m_routingTable.value(inputCommand);
}

void RouterMapping::clear()
{
	m_Bindings.clear();
	m_routingTable.clear();
    m_pScriptFileNames.clear();
    m_pScriptFunctionPrefixes.clear();
    //m_pScriptEngine;
}

void RouterMapping::callInit()
{
	// Call each script's init function if it exists
	QListIterator<QString> prefixIt(m_pScriptFunctionPrefixes);
	while (prefixIt.hasNext()) {
		QString initName = prefixIt.next();
		if (initName!="") {
			initName.append(".init");
			qDebug() << "MidiMapping: Executing" << initName;
			if (!m_pScriptEngine->execute(initName, "TODO:device id"))
				qWarning() << "RouterMapping: No" << initName << "function in script";
		}
	}
}

