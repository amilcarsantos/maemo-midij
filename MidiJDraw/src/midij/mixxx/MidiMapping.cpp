/*
 * MidiMapping.cpp
 *
 *  Created on: 27 de Jun de 2011
 *      Author: pt101821
 */

#include "MidiMapping.h"
#include "script/MidiScriptEngine.h"


#define REQUIRED_MAPPING_FILE "midi-mappings-scripts.js"
#define XML_SCHEMA_VERSION "1"


MidiMapping::MidiMapping(MidiObject &midi_object) : m_rMidiObject(midi_object)
{
}

MidiMapping::~MidiMapping()
{
}



/* loadPreset(QDomElement)
 * Loads a set of MIDI bindings from a QDomElement structure.
 */
void MidiMapping::loadPreset(QDomElement root, bool callInit) {
    //qDebug() << QString("MidiMapping: loadPreset() called in thread ID=%1").arg(this->thread()->currentThreadId(),0,16);

    if (root.isNull()) return;

    //SE_DEL//    m_pMidiInputMappingTableModel->removeRows(0, m_pMidiInputMappingTableModel->rowCount());
    //SE_DEL//    m_pMidiOutputMappingTableModel->removeRows(0, m_pMidiOutputMappingTableModel->rowCount());

    // Note, the lock comes after these two lines. We mustn't touch the
    // *MappingTableModel's after we are locked because they have pointers to us
    // so when we make a call to them they might in turn call us, causing a
    // deadlock.
    m_mappingLock.lock();

#ifdef __MIDISCRIPT__
    m_rMidiObject.restartScriptEngine();
    m_pScriptEngine = m_rMidiObject.getMidiScriptEngine();
    m_pScriptFileNames.clear();
    m_pScriptFunctionPrefixes.clear();
#endif

    // For each controller in the DOM
    m_Bindings = root;
    QDomElement controller = m_Bindings.firstChildElement("controller");
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
//SE_DEL        ConfigObject<ConfigValue> *config = new ConfigObject<ConfigValue>(QDir::homePath().append("/").append(SETTINGS_PATH).append(SETTINGS_FILE));

        qDebug() << "MidiMapping: Loading & evaluating all MIDI script code";

        QListIterator<QString> it(m_pScriptFileNames);
        while (it.hasNext()) {
            QString curScriptFileName = it.next();
            //SE_DEL            m_pScriptEngine->evaluate(config->getConfigPath().append("midi/").append(curScriptFileName));
            m_pScriptEngine->evaluate(curScriptFileName);

            if(m_pScriptEngine->hasErrors(curScriptFileName)) {
                qDebug() << "Errors occured while loading " << curScriptFileName;
            }

        }

        if (callInit) //++ MidiJ
        {
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
        }
        QStringList scriptFunctions = m_pScriptEngine->getScriptFunctions();

#endif

        QDomElement control = controller.firstChildElement("controls").firstChildElement("control");

        //Iterate through each <control> block in the XML
        while (!control.isNull()) {

            //Unserialize these objects from the XML
            MidiMessage midiMessage(control);
            MixxxControl mixxxControl(control);
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
                /*Old code: m_inputMapping.insert(midiMessage, mixxxControl);
                  Reason why this is bad: Don't want to access this directly because the
                  model doesn't get notified about the update */
#ifdef __MIDISCRIPT__
            }
#endif
            control = control.nextSiblingElement("control");
        }

        qDebug() << "MidiMapping: Input parsed!";

        QDomElement output = controller.firstChildElement("outputs").firstChildElement("output");

        //Iterate through each <control> block in the XML
        while (!output.isNull()) {
            //Unserialize these objects from the XML
            MidiMessage midiMessage(output);
            MixxxControl mixxxControl(output, true);

            //Add to the output mapping.
            internalSetOutputMidiMapping(mixxxControl, midiMessage, true);
            /*Old code: m_outputMapping.insert(mixxxControl, midiMessage);
              Reason why this is bad: Don't want to access this directly because the
                                      model doesn't get notified about the update */

            output = output.nextSiblingElement("output");
        }

        qDebug() << "MidiMapping: Output parsed!";
        controller = controller.nextSiblingElement("controller");
    }

    m_mappingLock.unlock();

}   // END loadPreset(QDomElement)


bool MidiMapping::isMidiMessageMapped(MidiMessage command) {
    m_mappingLock.lock();
    bool value = m_inputMapping.contains(command);
    m_mappingLock.unlock();
    return value;
}


/*
 * Lookup the MixxxControl mapped to a given MidiMessage.
 */
MixxxControl MidiMapping::getInputMixxxControl(MidiMessage command) {
    m_mappingLock.lock();
    MixxxControl control;
    if (m_inputMapping.contains(command)) {
        control = m_inputMapping.value(command);
    }
    m_mappingLock.unlock();
    return control;
}


void MidiMapping::internalSetInputMidiMapping(MidiMessage command, MixxxControl control, bool shouldEmit) {
    // If the command is already in the mapping, it will be replaced
    m_inputMapping.insert(command,control);
//    m_mappingLock.unlock();
//    if (shouldEmit)
//        emit(inputMappingChanged());
}




void MidiMapping::internalSetOutputMidiMapping(MixxxControl control,
                                               MidiMessage command,
                                               bool shouldEmit) {
    // If the command is already in the mapping, it will be replaced
//    m_outputMapping.insert(control, command);
//
//    if (shouldEmit)
//        emit(outputMappingChanged());
}


#ifdef __MIDISCRIPT__
/* -------- ------------------------------------------------------
   Purpose: Adds an entry to the list of script file names
            & associated list of function prefixes
   Input:   QString file name, QString function prefix
   Output:  -
   -------- ------------------------------------------------------ */
void MidiMapping::addScriptFile(QString filename, QString functionprefix) {
    // No lock necessary since this is private. Assume the lock is held.
    Q_ASSERT(!m_mappingLock.tryLock());

    m_pScriptFileNames.append(filename);
    m_pScriptFunctionPrefixes.append(functionprefix);
}
#endif
