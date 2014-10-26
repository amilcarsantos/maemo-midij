/*
 * MidijMidiObject.cpp
 *
 *  Created on: 26 de Jun de 2011
 *      Author: Amilcar
 */

#include "MidijMidiObject.h"
#include "MidiMappingDraw.h"
#include "midij/mixxx/MidiMapping.h"
#include "midij/mixxx/script/MidiScriptEngine.h"

MidijMidiObject::MidijMidiObject(MidiMappingDraw* midiMappingGui, QObject *parent)
	: MidiObject(parent)
{
	m_midiMappingGui = midiMappingGui;
	m_midiMapping = NULL;
	m_queueReceives = NoQueue;
}

MidijMidiObject::~MidijMidiObject()
{
	qDebug("MidijMidiObject::~MidijMidiObject()");
}

void MidijMidiObject::emulReceive(uchar status, uchar midicontrol, uchar midivalue)
{
	qDebug("MidijMidiObject::emulReceive()");

	// isto gera 1dead lock (por causa do release) --> criar 1queue de pedidos

	if (m_queueReceives == QueueAlways)
	{
		qDebug("Queue emulReceive()...");
		m_pendingReceives.enqueue(MidijQueuedMessage(status, midicontrol, midivalue));
	}
	else
	{
		MidiMessage inputCommand((MidiStatusByte)status, midicontrol, 0); // <status> already has the channel
		if (!m_midiMapping->isMidiMessageMapped(inputCommand))
			return;

		qDebug("... INICIO execute .......");
		if (m_queueReceives == QueueAfterEvent)
		{
			qDebug("QueueAfterEvent");
			m_queueReceives = QueueAlways;
		}

		execute(inputCommand, midivalue);
		qDebug("---- FIM execute -------");
		emit(receiveFinished());
	}
}

bool MidijMidiObject::execute(const MidiMessage& inputCommand, char midivalue)
{
	MixxxControl mixxxControl = m_midiMapping->getInputMixxxControl(inputCommand);
	if (mixxxControl.isNull())
		return false;

	if (!m_pScriptEngine->execute(mixxxControl.getControlObjectValue(), (char)inputCommand.getMidiChannel(), (char)inputCommand.getMidiNo(), (char)midivalue, (MidiStatusByte)(inputCommand.getMidiStatusByte() & 0xF0)))
	{
		qDebug() << "MidiObject: Invalid script function" << mixxxControl.getControlObjectValue();
	}
	return true;
}

void MidijMidiObject::processPendingReceives()
{
	qDebug("... processPendingReceives ....");
	while (!m_pendingReceives.isEmpty())
	{
		MidijQueuedMessage event = m_pendingReceives.dequeue();
		MidiMessage inputCommand((MidiStatusByte)event.status, event.midicontrol, 0); // <status> already has the channel
		execute(inputCommand, event.midivalue);
	}
	qDebug("---- processPendingReceives exit -------");
}

void MidijMidiObject::sendShortMsg(unsigned int word)
{
	m_midiMappingGui->midiShortMsgIn(word & 0x000000ff, (word & 0x0000ff00) >> 8, (word & 0x00ff0000) >> 16);
}

void MidijMidiObject::sendSysexMsg(unsigned char data[], unsigned int length)
{
	m_midiMappingGui->midiSysexMsgIn(data, length);
}
