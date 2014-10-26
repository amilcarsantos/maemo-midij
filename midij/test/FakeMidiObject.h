/*
 * FakeMidiObject.h
 *
 *  Created on: 8 de Ago de 2010
 *      Author: Amilcar
 */

#ifndef FAKEMIDIOBJECT_H_
#define FAKEMIDIOBJECT_H_

#include "../src/mixxx/MidiObject.h"

class FakeMidiObject : public MidiObject
{
	Q_OBJECT

public:
	FakeMidiObject();
	virtual ~FakeMidiObject();

signals:
	void midiSysexMsgOut(uchar*,uint);

// Overrides
public:
	virtual void devOpen(QString);
	virtual void devClose();
	virtual void sendShortMsg(uint word);

	void fakeReceiveShortMsg(MidiStatusByte status, char channel, char midicontrol, char midivalue);
	/**
	 *
	 * midivalue = -1 any
	 */
	bool pendingSendShortMsg(MidiStatusByte status, char channel, char midicontrol, int midivalue);
	int pendingSendShortMsgCount()
	{
		return m_pendingSendSortMsg.size();
	}

	void clearPendingMsg()
	{
		m_pendingSendSortMsg.clear();
		m_pendingSysexMsg.clear();
	}

	void emulSysexMsgReceive(const QByteArray& msg);
	int pendingSysexMsgCount();
	virtual void sendSysexMsg(uchar* data, uint len);
	bool pendingSysexMsg(const QByteArray& startMsg);

public:

	QList<uint> m_pendingSendSortMsg;
	QList<QByteArray> m_pendingSysexMsg;

};

#endif /* FAKEMIDIOBJECT_H_ */
