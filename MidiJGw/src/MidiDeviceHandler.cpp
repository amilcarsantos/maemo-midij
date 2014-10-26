/*
 * MidiJGw - MIDI gateway for MidiJ application
 * Copyright (C) 2011  Amilcar Santos <amilcar.santos@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "MidiDeviceHandler.h"
#include "mixxx/MidiObjectWin.h"
#include "MidiObjectUdp.h"

#define INPUTDEVICE_WIN		0x10000
#define INPUTDEVICE_UDP		0x20000

MidiDeviceHandler::MidiDeviceHandler()
{
	m_outputObject = NULL;
	init();
}

MidiDeviceHandler::~MidiDeviceHandler()
{
	clear(false);
}

void MidiDeviceHandler::init()
{
	m_inputDevice2Type.insert("INPUTDEVICE_WIN", INPUTDEVICE_WIN);
	m_inputDevice2Type.insert("INPUTDEVICE_UDP", INPUTDEVICE_UDP);

	// TODO needs improvement
	m_conflictDevices.insertMulti("UDP/IP [5004/5]", "UDP/IP [5004]");
	m_conflictDevices.insertMulti("UDP/IP [5004/5]", "UDP/IP [5005]");
	m_conflictDevices.insertMulti("UDP/IP [5004]",   "UDP/IP [5004/5]");
	m_conflictDevices.insertMulti("UDP/IP [5005]",   "UDP/IP [5004/5]");

	m_conflictDevices.insertMulti("UDP/IP [21928/9]", "UDP/IP [21928]");
	m_conflictDevices.insertMulti("UDP/IP [21928/9]", "UDP/IP [21929]");
	m_conflictDevices.insertMulti("UDP/IP [21928]",   "UDP/IP [21928/9]");
	m_conflictDevices.insertMulti("UDP/IP [21929]",   "UDP/IP [21928/9]");
}

MidiObject* MidiDeviceHandler::outputDevice()
{
	if (m_outputObject == NULL)
		m_outputObject = new MidiObjectWin(MIDI_OBJECT_NO_MAPPING + MIDI_OBJECT_NO_SCRIPT + MIDI_OBJECT_ONLY_OUTPUT);

	return m_outputObject;
}

QStringList MidiDeviceHandler::outputDeviceList()
{
	if (m_outputDeviceList.isEmpty())
		m_outputDeviceList.append(*outputDevice()->getDeviceList());

	foreach(QString device, m_outputDeviceList)
		m_inputDevice2Type.insert(device, INPUTDEVICE_WIN);


	return m_outputDeviceList;
}


MidiObject* MidiDeviceHandler::inputDevice(int ctrlId, QString deviceName)
{
	int type = m_inputDevice2Type.value(deviceName);
	MidiObject* midiObject = m_inputObjectMap.value(type + ctrlId);
	if (midiObject)
		return midiObject;
	// must create device
	switch (type)
	{
	case INPUTDEVICE_WIN:
		midiObject = new MidiObjectWin(MIDI_OBJECT_NO_MAPPING + MIDI_OBJECT_NO_SCRIPT + MIDI_OBJECT_ONLY_INPUT);
		break;
	case INPUTDEVICE_UDP:
		midiObject = new MidiObjectUdp(MidiObjectUdp::BidirServer);
		midiObject->updateDeviceList();
		break;
	default:
		// not valid
		return NULL;
	}
	m_inputObjectMap.insert(type + ctrlId, midiObject);
	return midiObject;
}

void MidiDeviceHandler::createInputDeviceList(int type, const QStringList& deviceList)
{
	foreach(QString device, deviceList)
		m_inputDevice2Type.insert(device, type);

	m_inputDeviceList.append(deviceList);
}

QStringList MidiDeviceHandler::inputDeviceList()
{
	if (m_inputDeviceList.isEmpty())
	{
		MidiObject* midiDevice = inputDevice(0, "INPUTDEVICE_UDP");
		createInputDeviceList(INPUTDEVICE_UDP, *midiDevice->getDeviceList());
		midiDevice = inputDevice(0, "INPUTDEVICE_WIN");
		createInputDeviceList(INPUTDEVICE_WIN, *midiDevice->getDeviceList());
	}
	return m_inputDeviceList;
}

QStringList MidiDeviceHandler::conflictDevices(QString deviceName)
{
	// TODO get this from MidiObjectUdp class
	QList<QString> deviceList = m_conflictDevices.values(deviceName);
	return QStringList(deviceList);
}

QIcon MidiDeviceHandler::deviceIcon(QString deviceName)
{
	int type = m_inputDevice2Type.value(deviceName);
	switch (type)
	{
	case INPUTDEVICE_WIN:
		return QIcon(":/midijgw/midi_win.png");
	case INPUTDEVICE_UDP:
		return QIcon(":/midijgw/midi_udp.png");
	}
	// unknown
	return QIcon(":/midijgw/midi_offline.png");
}


bool MidiDeviceHandler::suportsSysexIdentMsg(QString deviceName)
{
	// FIXME suportsSysexIdent(QString deviceName)
	return false;
}


// all previous retrieved devices are invalidated
void MidiDeviceHandler::clear(bool callInit)
{
	if (m_outputObject)
	{
		m_outputObject->devClose();
		delete m_outputObject;
		m_outputObject = NULL;
	}
	m_outputDeviceList.clear();


	foreach(MidiObject* device, m_inputObjectMap.values())
	{
		device->devClose();
	}
	qDeleteAll(m_inputObjectMap.values());
	m_inputObjectMap.clear();
	m_inputDeviceList.clear();
	m_inputDevice2Type.clear();

	if (callInit)
		init();
}
