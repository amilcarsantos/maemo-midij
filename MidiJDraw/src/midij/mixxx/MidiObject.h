/*
 * MidiObject.h
 *
 *  Created on: 22 de Mai de 2011
 *      Author: Amilcar
 */

#ifndef MIDIOBJECT_H_
#define MIDIOBJECT_H_

#include <QtCore>

#include "MidiMessage.h"

class MidiScriptEngine;

class MidiObject : public QObject
{
	Q_OBJECT
public:
	MidiObject(QObject *parent=0);
	virtual ~MidiObject();


	void restartScriptEngine() {};
	MidiScriptEngine* getMidiScriptEngine() {return m_pScriptEngine;};
	void setMidiScriptEngine(MidiScriptEngine* engine) {m_pScriptEngine = engine;};

    // Stuff for sending messages to control the device
    Q_INVOKABLE void sendShortMsg(unsigned char status, unsigned char byte1, unsigned char byte2);
    virtual void sendShortMsg(unsigned int word);
    virtual void sendSysexMsg(unsigned char data[], unsigned int length);
    Q_INVOKABLE void sendSysexMsg(QList<int> data, unsigned int length);

protected:
    MidiScriptEngine* m_pScriptEngine;
};

#endif /* MIDIOBJECT_H_ */
