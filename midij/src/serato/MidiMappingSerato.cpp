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

#include "MidiMappingSerato.h"
#include <QDebug>


MidiMappingSerato::MidiMappingSerato()
{
	m_Bindings.clear();
	m_cachedSelect.clear();
	m_node.clear();
}

MidiMappingSerato::~MidiMappingSerato()
{
}


/* loadPreset(QDomElement)
 * Loads a set of MIDI bindings from a QDomElement structure.
 */
bool MidiMappingSerato::loadPreset(QDomElement root) {
    //qDebug() << QString("MidiMapping: loadPreset() called in thread ID=%1").arg(this->thread()->currentThreadId(),0,16);

    if (root.isNull())
    	return false;

    //SE_DEL//    m_pMidiInputMappingTableModel->removeRows(0, m_pMidiInputMappingTableModel->rowCount());
    //SE_DEL//    m_pMidiOutputMappingTableModel->removeRows(0, m_pMidiOutputMappingTableModel->rowCount());

    // Note, the lock comes after these two lines. We mustn't touch the
    // *MappingTableModel's after we are locked because they have pointers to us
    // so when we make a call to them they might in turn call us, causing a
    // deadlock.
    m_mappingLock.lock();

//#ifdef __MIDISCRIPT__
//    m_rMidiObject.restartScriptEngine();
//    m_pScriptEngine = m_rMidiObject.getMidiScriptEngine();
//    m_pScriptFileNames.clear();
//    m_pScriptFunctionPrefixes.clear();
//#endif

    // For each controller in the DOM
    m_Bindings = root;
/*SE_DEL//    QDomElement controller = m_Bindings.firstChildElement("Control");
    while (!controller.isNull()) {
        // For each controller
        // Get deviceid
        QString device = controller.attribute("id","");
        qDebug() << device << " settings found" << endl;

#ifdef __MIDISCRIPT__

        // Get a list of MIDI script files to load
        QDomElement scriptFile = controller.firstChildElement("scriptfiles").firstChildElement("file");

        // Default currently required file
        addScriptFile(REQUIRED_MAPPING_FILE,"");

        // Look for additional ones
        while (!scriptFile.isNull()) {
            QString functionPrefix = scriptFile.attribute("functionprefix","");
            QString filename = scriptFile.attribute("filename","");
            addScriptFile(filename, functionPrefix);

            scriptFile = scriptFile.nextSiblingElement("file");
        }

        // Load Script files
        ConfigObject<ConfigValue> *config = new ConfigObject<ConfigValue>(QDir::homePath().append("/").append(SETTINGS_PATH).append(SETTINGS_FILE));

        qDebug() << "MidiMapping: Loading & evaluating all MIDI script code";

        QListIterator<QString> it(m_pScriptFileNames);
        while (it.hasNext()) {
            QString curScriptFileName = it.next();
            m_pScriptEngine->evaluate(config->getConfigPath().append("midi/").append(curScriptFileName));

            if(m_pScriptEngine->hasErrors(curScriptFileName)) {
                qDebug() << "Errors occured while loading " << curScriptFileName;
            }

        }

        // Call each script's init function if it exists
        QListIterator<QString> prefixIt(m_pScriptFunctionPrefixes);
        while (prefixIt.hasNext()) {
            QString initName = prefixIt.next();
            if (initName!="") {
                initName.append(".init");
                qDebug() << "MidiMapping: Executing" << initName;
                if (!m_pScriptEngine->execute(initName, device))
                    qWarning() << "MidiMapping: No" << initName << "function in script";
            }
        }

        QStringList scriptFunctions = m_pScriptEngine->getScriptFunctions();

#endif

        QDomElement control = controller.firstChildElement("controls").firstChildElement("control");

        //Iterate through each <control> block in the XML
        while (!control.isNull()) {

            //Unserialize these objects from the XML
            MidiMessageSerato midiMessage(control);
            SeratoControl mixxxControl(control);
#ifdef __MIDISCRIPT__
            // Verify script functions are loaded
            if (mixxxControl.getMidiOption()==MIDI_OPT_SCRIPT &&
                scriptFunctions.indexOf(mixxxControl.getControlObjectValue())==-1) {

                QString statusText = QString(midiMessage.getMidiStatusByte());
                qWarning() << "Error: Function" << mixxxControl.getControlObjectValue()
                           << "was not found in loaded scripts."
                           << "The MIDI Message with status byte"
                           << statusText << midiMessage.getMidiNo()
                           << "will not be bound. Please check the"
                           << "mapping and script files.";
            } else {
#endif
                //Add to the input mapping.
                internalSetInputMidiMapping(midiMessage, mixxxControl, true);
#ifdef __MIDISCRIPT__
            }
#endif
            control = control.nextSiblingElement("control");
        }

        qDebug() << "MidiMapping: Input parsed!";
*/
        QDomElement output = m_Bindings.firstChildElement("Control");
        if (output.isNull())
        {
        	qWarning() << "MidiMappingSerato: mapping is invalid!";
        	return false;
        }

        //Iterate through each <control> block in the XML
        while (!output.isNull()) {
            //Unserialize these objects from the XML
            MidiMessageSerato midiMessage(output);
            //SeratoControl seratoControl(output, true);

            //Add to the output mapping.
            m_outputMapping.insert(MidiMessageSerato::name(output), midiMessage);

            output = output.nextSiblingElement("Control");
        }
        qDebug() << "MidiMappingSerato: mapping parsed! (count:" << m_outputMapping.size() << ")";
//SE_DEL//        controller = controller.nextSiblingElement("controller");
//SE_DEL//    }

    m_mappingLock.unlock();

    return true;
}   // END loadPreset(QDomElement)

bool MidiMappingSerato::isOutputSeratoMessageMapped(const QString& name)
{
	return m_outputMapping.contains(name);
}

MidiMessageSerato MidiMappingSerato::getOutputSeratoMessage(const QString& name)
{
	return m_outputMapping.value(name);
}

QString MidiMappingSerato::getValueFromBindings(QString select, QString attrSelect)
{
	// first check cache
	if (m_cachedSelect == select)
	{
		qDebug() << "CACHE HIT --" << m_cachedSelect;
		return m_node.attribute(attrSelect);
	}

	m_cachedSelect = select;
	m_node = m_Bindings;

	QStringList selectList = select.split(QChar('/'));
	int i = 0;

	while (selectList.size() > i)
	{
		QString nodeName = selectNodeName(selectList.at(i));
		QString attrName = selectAttrName(selectList.at(i));
		qDebug() << "node:" << nodeName << "; attr:" << attrName;
		while (!m_node.isNull())
		{
			if (attrName.isEmpty())
			{
				if (m_node.nodeName() == nodeName)
				{
					// next child
					i++;
					if (selectList.size() <= i)
						return m_node.attribute(attrSelect);
					m_node = m_node.firstChildElement();
					break;
				}
				// next sibling
				m_node = m_node.nextSiblingElement();
			}
			else
			{
				// check name attr
				if (attrName == m_node.attribute("name", "") && (m_node.nodeName() == nodeName))
				{
					// next child
					i++;
					if (selectList.size() <= i)
						return m_node.attribute(attrSelect);
					m_node = m_node.firstChildElement();
					break;
				}
				// next sibling
				m_node = m_node.nextSiblingElement();
			}
		}
		// not found!!!
		if (m_node.isNull())
		{
			qDebug() << "not found...";
			m_cachedSelect.clear();
			return QString();
		}
	}

	m_cachedSelect.clear();
	return QString();
}

void MidiMappingSerato::clear()
{
    m_node.clear();
	m_Bindings.clear();
    m_outputMapping.clear();
    m_cachedSelect.clear();
}

QString MidiMappingSerato::selectNodeName(QString select)
{
	int pos = select.indexOf('[');
	if (pos > 0)
	{
		return select.left(pos);
	}
	return select;
}

QString MidiMappingSerato::selectAttrName(QString select)
{
	int pos = select.indexOf('[');
	if (pos > 0)
	{
		return select.mid(pos + 1, select.indexOf(']') - pos - 1);
	}
	return QString();
}
