/***************************************************************************
                          midiscriptengine.h  -  description
                          -------------------
    begin                : Fri Dec 12 2008
    copyright            : (C) 2008 by Sean M. Pappalardo
    email                : pegasus@renegadetech.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef MIDISCRIPTENGINE_H
#define MIDISCRIPTENGINE_H

#include <QEvent>
#include <QtScript>
//SE_DEL//#include "configobject.h"
#include "../MidiObject.h"

//Forward declaration(s)
//SE_DEL//class ControlObjectThread;

class MidiScriptEngine : public QThread {

    Q_OBJECT

public:
    MidiScriptEngine(MidiObject* midi_object);
    ~MidiScriptEngine();

    bool isReady();
    bool hasErrors(QString filename);
    const QStringList getErrors(QString filename);

    // Evaluate a script file
    bool evaluate(QString filepath);
    // Execute a particular function
    bool execute(QString function);
    // Execute a particular function with a data string (e.g. a device ID)
    bool execute(QString function, QString data);
    // Execute a particular function with all the data
    bool execute(QString function, char channel,
                 char control, char value, MidiStatusByte status);

    // Lookup registered script functions
    QStringList getScriptFunctions();

    QScriptEngine* getEngine()
    {
    	return m_pEngine;
    };

    /* DO NOT CALL DIRECTLY,
       SHOULD ONLY BE CALLED FROM WITHIN SCRIPTS */
    Q_INVOKABLE double getValue(QString group, QString name);
    Q_INVOKABLE void setValue(QString group, QString name, double newValue);
    Q_INVOKABLE bool connectControl(QString group, QString name,
                                    QString function, bool disconnect = false);
    Q_INVOKABLE void trigger(QString group, QString name);

public slots:
    void slotValueChanged(double value);

signals:
    void initialized();

protected:
    void run();

//SE_DEL//private:
    // Only call these with the scriptEngineLock
    bool safeEvaluate(QString filepath);
    bool safeExecute(QString function);
    bool safeExecute(QString function, QString data);
    bool safeExecute(QString function, char channel,
                     char control, char value, MidiStatusByte status);
    void initializeScriptEngine();

    void generateScriptFunctions(QString code);
    bool checkException();

    //SE_DEL//ControlObjectThread* getControlObjectThread(QString group, QString name);


    MidiObject *m_pMidiObject;
    //SE_DEL//QHash<ConfigKey, QString> m_connectedControls;
    QScriptEngine *m_pEngine;
    QStringList m_scriptFunctions;
    QMap<QString,QStringList> m_scriptErrors;
    static QMutex m_scriptEngineLock;
    //SE_DEL//QHash<ConfigKey, ControlObjectThread*> m_controlCache;

    QMap<QString, double> m_valuesMap; //++MidiJ
};

#endif

