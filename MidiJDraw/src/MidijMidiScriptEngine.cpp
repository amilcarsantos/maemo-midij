/*
 * MidijMidiSciptEngine.cpp
 *
 *  Created on: 26 de Jun de 2011
 *      Author: Amilcar
 */

#include "MidijMidiScriptEngine.h"
#include "midij/Utils.h"
#include "MidijMidiObject.h"

MidijMidiScriptEngine::MidijMidiScriptEngine(MidiObject* midi_object)
	: MidiScriptEngine(midi_object)
{
	m_debugging = false;
}

MidijMidiScriptEngine::~MidijMidiScriptEngine()
{
}

void MidijMidiScriptEngine::initEngine(MidiObject* midi_object)
{
	if (midi_object)
		m_pMidiObject = midi_object;

	initializeScriptEngine();

	if (m_pMidiObject->parent() == NULL)
		m_pMidiObject->setParent(m_pEngine);
}

void MidijMidiScriptEngine::stopEngine()
{
	_delete(m_pEngine);
}

void MidijMidiScriptEngine::debugging(bool b)
{
	if (m_debugging == b)
		return;
	MidijMidiObject* midiObject = qobject_cast<MidijMidiObject*>(m_pMidiObject);
	if (midiObject)
	{
		if (b)
			midiObject->queueReceives(MidijMidiObject::QueueAfterEvent);
		else
		{
			midiObject->queueReceives(MidijMidiObject::NoQueue);
			midiObject->processPendingReceives();
		}
	}
	m_debugging = b;
}

