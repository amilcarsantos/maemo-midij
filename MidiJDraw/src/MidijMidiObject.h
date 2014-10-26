/*
 * MidijMidiObject.h
 *
 *  Created on: 26 de Jun de 2011
 *      Author: Amilcar
 */

#ifndef MIDIJMIDIOBJECT_H_
#define MIDIJMIDIOBJECT_H_

#include "midij/mixxx/MidiObject.h"

class MidiMappingDraw;
class MidiMapping;

struct MidijQueuedMessage
{
	uchar status;
	uchar midicontrol;
	uchar midivalue;

	MidijQueuedMessage(uchar s, uchar mc, uchar mv)
	{
		status = s;
		midicontrol = mc;
		midivalue = mv;
	}
};

class MidijMidiObject : public MidiObject
{
	Q_OBJECT

public:
	enum QueueReceives
	{
		NoQueue,
		QueueAfterEvent,
		QueueAlways
	};
	MidijMidiObject(MidiMappingDraw* midiMappingGui, QObject *parent=0);
	virtual ~MidijMidiObject();

	void setMidiMapping(MidiMapping* midiMapping);

	void queueReceives(QueueReceives b);

public slots:
	void emulReceive(uchar status, uchar midicontrol, uchar midivalue);
	void processPendingReceives();

signals:
	void receiveFinished();

protected:
    virtual void sendShortMsg(unsigned int word);
    virtual void sendSysexMsg(unsigned char data[], unsigned int length);


    bool execute(const MidiMessage& inputCommand, char midivalue);

	MidiMappingDraw* m_midiMappingGui;
	MidiMapping* m_midiMapping;

	QQueue<MidijQueuedMessage> m_pendingReceives;
	QueueReceives m_queueReceives;
};

inline void MidijMidiObject::setMidiMapping(MidiMapping* midiMapping)
{
	m_midiMapping = midiMapping;
}

// enqueue MIDI receives (prevents deadlocks of QtScript debugger)
inline void MidijMidiObject::queueReceives(QueueReceives b)
{
	m_queueReceives = b;
}


#endif /* MIDIJMIDIOBJECT_H_ */
