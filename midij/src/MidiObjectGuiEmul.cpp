/*
 * MidiJ - Remote MIDI controller for DJs
 * Copyright (C) 2010  Amilcar Santos <amilcar.santos@gmail.com>
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

#include "MidiObjectGuiEmul.h"

MidiObjectGuiEmul::MidiObjectGuiEmul()
{
}

MidiObjectGuiEmul::~MidiObjectGuiEmul()
{
}

void MidiObjectGuiEmul::devOpen(QString device)
{
/*	if (m_deviceName.isEmpty() && m_midiObject)
	{
		m_deviceName = device;
		// delegate
		m_midiObject->devOpen(device);
	}*/
}

void MidiObjectGuiEmul::devClose()
{
/*	QString device = m_deviceName;
	m_deviceName.clear();
	if (!device.isEmpty() && m_midiObject)
	{
		// delegate
		m_midiObject->devClose();
	}*/
}

//
// receives a MIDI message from the GUI
void MidiObjectGuiEmul::emulReceive(uchar status, uchar midicontrol, uchar midivalue)
{
/*    if (midiDebug != -1)*/ qDebug() << QString("MIDI status: %1, ctrl: %2, val: %3")
        .arg(QString::number(status & 255, 16).toUpper())
        .arg(QString::number(midicontrol, 16).toUpper())
        .arg(QString::number(midivalue, 16).toUpper());

    receive((MidiStatusByte)(status & 255), status & 15, midicontrol, midivalue); // void receive(MidiStatusByte status, char channel, char control, char value);
}


//
// receives a MIDI message from the Script engine
void MidiObjectGuiEmul::sendShortMsg(uint word)
{
//	qDebug() << "MidiObjectGuiEmul::sendShortMsg() " << (Qt::HANDLE)word;
	emit(emulSendShortMsg(word & 0x000000ff, (word & 0x0000ff00) >> 8, (word & 0x00ff0000) >> 16));
}


void MidiObjectGuiEmul::sendSysexMsg(unsigned char data[], unsigned int length)
{
	QString dump;
	for (uint i = 0; i < length; i++)
	{
		if (i > 0)
			dump+=" ";
		dump+=QString::number(data[i], 16);
	}

    qWarning() << "MidiObjectGuiEmul::SysexMsg MSG...." << dump;
	emit(emulSendSysexMsg(data, length));
}

