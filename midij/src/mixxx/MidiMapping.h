/***************************************************************************
                             midimapping.h
                           MIDI Mapping Class
                           -------------------
    begin                : Sat Jan 17 2009
    copyright            : (C) 2009 Sean M. Pappalardo
    email                : pegasus@c64.org

***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef MIDIMAPPING_H
#define MIDIMAPPING_H

#include "MidiObject.h"
#include "MidiMessage.h"
#include "MixxxControl.h"
//SE_DEL//#include "midiinputmapping.h"
//SE_DEL//#include "midioutputmapping.h"
//SE_DEL//#include <QTableWidget>

#ifdef __MIDISCRIPT__
#include "script/MidiScriptEngine.h"
#endif

//Forward declarations
//SE_DEL//class MidiInputMappingTableModel;
//SE_DEL//class MidiOutputMappingTableModel;

// MEU
typedef QHash<MidiMessage, MixxxControl>	MidiInputMapping;
typedef QHash<MixxxControl, MidiMessage>	MidiOutputMapping;
// ~MEU

#define BINDINGS_PATH QDir::homePath().append("/").append(SETTINGS_PATH).append("MixxxMIDIBindings.xml")
#define MIDI_MAPPING_EXTENSION ".midi.xml"

class MidiMapping : public QObject
{
    Q_OBJECT

    public:
    /** Constructor also loads & applies the default XML MIDI mapping file */
    MidiMapping(MidiObject &midi_object);
    ~MidiMapping();

    void loadInitialPreset();
    void loadPreset(QString path);
    void loadPreset(QDomElement root, bool callInit);

    void savePreset(QString path = "");//BINDINGS_PATH);
    void applyPreset();
    void callInit();
    void clearPreset();




    //SE_DEL//    MidiInputMappingTableModel* getMidiInputMappingTableModel();
    //SE_DEL//    MidiOutputMappingTableModel* getMidiOutputMappingTableModel();
    //MixxxControl* getInputMixxxControl(MidiMessage command);

    //SE_DEL//    static double ComputeValue(MidiOption midioption, double _prevmidivalue, double _newmidivalue);

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

    // MIDI Output Mapping Modifiers
    int numOutputMixxxControls();
    bool isOutputIndexValid(int index);
    bool isMixxxControlMapped(const MixxxControl&  control);
    MixxxControl getOutputMixxxControl(int index);
    MidiMessage getOutputMidiMessage(int index);
    MidiMessage getOutputMidiMessage(MixxxControl control);
    void setOutputMidiMapping(MixxxControl control, MidiMessage command);
    void clearOutputMidiMapping(int index);
    void clearOutputMidiMapping(MixxxControl control);
    void clearOutputMidiMapping(int index, int count);

public slots:
    void finishMidiLearn(MidiMessage message);
    void beginMidiLearn(MixxxControl control);
    void cancelMidiLearn();

signals:
    void inputMappingChanged();
    void inputMappingChanged(int startIndex, int endIndex);
    void outputMappingChanged();
    void outputMappingChanged(int startIndex, int endIndex);
    void midiLearningStarted();
    void midiLearningFinished(MidiMessage);
    void midiLearningFinished();

private:
    int internalNumInputMidiMessages();
    bool internalIsInputIndexValid(int index);
    void internalSetInputMidiMapping(MidiMessage command,
                                     MixxxControl control,
                                     bool shouldEmit);
    int internalNumOutputMidiMessages();
    int internalNumOutputMixxxControls();
    bool internalIsOutputIndexValid(int index);
    void internalSetOutputMidiMapping(MixxxControl control,
                                      MidiMessage command,
                                      bool shouldEmit);
    void buildDomElement();
    void addControl(QDomElement& control, QString device);
    void addOutput(QDomElement& output, QString device);
    void addMidiScriptInfo(QDomElement &scriptFile, QString device); //Sucks

    bool addInputControl(MidiStatusByte midiStatus, int midiNo, int midiChannel,
                         QString controlObjectGroup, QString controlObjectKey,
                         MidiOption midiOption);
    bool addInputControl(MidiMessage message, MixxxControl control);
    void removeInputMapping(MidiStatusByte midiStatus, int midiNo, int midiChannel);

#ifdef __MIDISCRIPT__
    /** Adds a script file name and function prefix to the list to be loaded */
    void addScriptFile(QString filename, QString functionprefix);

    QList<QString> m_pScriptFileNames;
    QList<QString> m_pScriptFunctionPrefixes;
    MidiScriptEngine *m_pScriptEngine;
#endif
    QMutex m_mappingLock;
    QDomElement m_Bindings;
    MidiObject &m_rMidiObject;
    MidiInputMapping m_inputMapping;
    MidiOutputMapping m_outputMapping;
    //SE_DEL//    MidiInputMappingTableModel* m_pMidiInputMappingTableModel;
    //SE_DEL//    MidiOutputMappingTableModel* m_pMidiOutputMappingTableModel;
    MixxxControl m_controlToLearn;
};

#endif
