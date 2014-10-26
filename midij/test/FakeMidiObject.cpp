/*
 * FakeMidiObject.cpp
 *
 *  Created on: 8 de Ago de 2010
 *      Author: Amilcar
 */

#include "FakeMidiObject.h"

FakeMidiObject::FakeMidiObject()
	: MidiObject(MIDI_OBJECT_NO_SCRIPT + MIDI_OBJECT_NO_MAPPING)
{
}

FakeMidiObject::~FakeMidiObject()
{
}


void FakeMidiObject::devOpen(QString device)
{
	qDebug() << "FakeMidiObject::devOpen(): " << device;
	m_deviceName = device;
}

void FakeMidiObject::devClose()
{
	qDebug() << "FakeMidiObject::devClose(): " << m_deviceName;
	m_deviceName.clear();
}

void FakeMidiObject::sendShortMsg(uint word)
{
	qDebug() << m_deviceName << "FakeMidiObject::sendShortMsg() " << (Qt::HANDLE)word;
	m_pendingSendSortMsg.append(word);
}

/**
 *
 */
void FakeMidiObject::fakeReceiveShortMsg(MidiStatusByte status, char channel, char midicontrol, char midivalue)
{
	qDebug() << "FakeMidiObject::fakeReceiveShortMsg() tr(" << QThread::currentThreadId()
			<< ") miditype: " << (int)status
			<< ", ch: " << (int)channel
			<< ", ctrl: " << (int)midicontrol
			<< ", val: " << (int)midivalue;

	// void receive(MidiStatusByte status, char channel, char control, char value
	receive(status, channel, midicontrol, midivalue);
}

/**
 *
 */
bool FakeMidiObject::pendingSendShortMsg(MidiStatusByte status, char channel, char midicontrol, int midivalue)
{
    unsigned int word = (((unsigned int)midivalue) << 16) |
                        (((unsigned int)midicontrol) << 8) | status + channel;

	return m_pendingSendSortMsg.contains(word);
}


void FakeMidiObject::emulSysexMsgReceive(const QByteArray& msg)
{
	emit(midiSysexMsgOut((uchar*)msg.data(), (uint)msg.size()));
}

int FakeMidiObject::pendingSysexMsgCount()
{
	return m_pendingSysexMsg.size();
}

void FakeMidiObject::sendSysexMsg(uchar* data, uint len)
{
	qDebug() <<  m_deviceName << "FakeMidiObject::sendSysMsg";
	m_pendingSysexMsg.append(QByteArray((const char*)data, len));
}

bool FakeMidiObject::pendingSysexMsg(const QByteArray& startMsg)
{
	foreach(const QByteArray& msg, m_pendingSysexMsg)
	{
		if (msg.startsWith(startMsg))
			return true;
	}
	return false;
}
