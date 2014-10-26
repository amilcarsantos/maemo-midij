/*
 * FakeMidiReceiver.cpp
 *
 *  Created on: 4 de Mar de 2011
 *      Author: pt101821
 */

#include "FakeMidiReceiver.h"
#include <QtDebug>

FakeMidiReceiver::FakeMidiReceiver()
{
}

FakeMidiReceiver::~FakeMidiReceiver()
{
}

void FakeMidiReceiver::receiveMidiMessage(MidiMessage message, char value)
{
	quint32 word = (((unsigned int)value) << 16) |
		           (((unsigned int)message.getMidiNo()) << 8) | message.getMidiStatusByte() /*+ channel*/;

	qDebug() << m_traceName << "FakeMidiReceiver::receiveMidiMessage" << (Qt::HANDLE)word;
	m_pendingReceiveShortMsg.append(word);
}


bool FakeMidiReceiver::receivedShortMsg(uchar status, uchar byte1, uchar byte2)
{
	quint32 word = (((unsigned int)byte2) << 16) |
		           (((unsigned int)byte1) << 8) | status /*+ channel*/;

	return m_pendingReceiveShortMsg.contains(word);
}

void FakeMidiReceiver::receiveSysexMsg(uchar* data, uint size)
{
	qDebug() << m_traceName << "FakeMidiReceiver::receiveSysexMsg" << size << "bytes";
	QByteArray ba((const char*)data, size);
	m_pendingReceiveSysexMsg.append(ba);
}

bool FakeMidiReceiver::pendingSysexMsg(const QByteArray& startMsg)
{
	foreach(const QByteArray& msg, m_pendingReceiveSysexMsg)
	{
		if (msg.startsWith(startMsg))
			return true;
	}
	return false;
}


