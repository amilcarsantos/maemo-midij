/*
 * MidiMapping.h
 *
 *  Created on: 27 de Jun de 2011
 *      Author: pt101821
 */

#ifndef MIDIMAPPING_H_
#define MIDIMAPPING_H_

#include "MidiObject.h"
#include "MidiMessage.h"
#include "MixxxControl.h"

typedef QHash<MidiMessage, MixxxControl>	MidiInputMapping;


class MidiMapping
{
public:
	MidiMapping(MidiObject &midi_object);
	virtual ~MidiMapping();

	void loadPreset(QDomElement root, bool callInit);

    // MIDI Input Mapping Modifiers
    int numInputMidiMessages();
    bool isInputIndexValid(int index);
    bool isMidiMessageMapped(MidiMessage command);
    MidiMessage getInputMidiMessage(int index);
    MixxxControl getInputMixxxControl(int index);
    MixxxControl getInputMixxxControl(MidiMessage command);
    void setInputMidiMapping(MidiMessage command, MixxxControl control);
    void clearInputMidiMapping(int index);
    void clearInputMidiMapping(MidiMessage command);
    void clearInputMidiMapping(int index, int count);

#ifdef __MIDISCRIPT__
    /** Adds a script file name and function prefix to the list to be loaded */
    void addScriptFile(QString filename, QString functionprefix);

    QList<QString> m_pScriptFileNames;
    QList<QString> m_pScriptFunctionPrefixes;
    MidiScriptEngine *m_pScriptEngine;
#endif

private:
    void internalSetInputMidiMapping(MidiMessage command,
                                     MixxxControl control,
                                     bool shouldEmit);
    void internalSetOutputMidiMapping(MixxxControl control,
                                      MidiMessage command,
                                      bool shouldEmit);


    QMutex m_mappingLock;
    QDomElement m_Bindings;
    MidiObject &m_rMidiObject;
    MidiInputMapping m_inputMapping;

};

#endif /* MIDIMAPPING_H_ */
