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

#include "MidiMessageSerato.h"
#include <QtXml>
//#include "../mixxx/MidiMessage.h"
typedef enum {
    MIDI_STATUS_NOTE_OFF   = 0x80,
    MIDI_STATUS_NOTE_ON    = 0x90,
    MIDI_STATUS_CC         = 0xB0,
    MIDI_STATUS_PITCH_BEND = 0xE0,
} MidiStatusByte;

MidiMessageSerato::MidiMessageSerato()
{
}

/** Constructor that unserializes a MidiMessage object from a <control> or <output>
    node block in our MIDI mapping XML file.
*/
MidiMessageSerato::MidiMessageSerato(const QDomElement& node)
{
    // For each control

    QString midiChannel = node.attribute("channel");
    QString midiEventType = node.attribute("event_type");
    QString midiControl = node.attribute("control");
//    QString midiOff = parentNode.firstChildElement("off").text();

    bool ok = false;

    //Use QString with toInt base of 0 to auto convert hex values
    m_midiNo = midiControl.toUShort(&ok);
    if (!ok)
        m_midiNo = 0x00;

    m_midiChannel = midiChannel.toUShort(&ok) - 1;
    if (!ok)
    	m_midiChannel = 0x00;

    m_midiStatusByte = eventType2MidiStatusByte(midiEventType, &ok);
    if (!ok)
        m_midiStatusByte = 0x00;

/*SE_DEL//    m_midiByte2On = midiOn.toInt(&ok, 0);
    if (!ok)
        m_midiByte2On = 0x7F;

    m_midiByte2Off = midiOff.toInt(&ok, 0);
    if (!ok)
        m_midiByte2Off = 0x00;*/
}


MidiMessageSerato::~MidiMessageSerato()
{
}

int MidiMessageSerato::eventType2MidiStatusByte(QString midiEventType, bool* ok)
{
	if (ok != NULL)
		*ok = true;

	if (midiEventType == QString("Note On"))
		return MIDI_STATUS_NOTE_ON;
	if (midiEventType == QString("Control Change"))
		return MIDI_STATUS_CC;

	if (ok != NULL)
		*ok = false;
	return 0;
}


QString MidiMessageSerato::name(const QDomElement& node)
{
	return node.attribute("name");
}
