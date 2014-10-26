/*
 * MidiJ - Midi controller for DJs
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

#ifndef MIDIMESSAGESERATO_H_
#define MIDIMESSAGESERATO_H_

#include <qdom.h>


class MidiMessageSerato
{
public:
	MidiMessageSerato();
	MidiMessageSerato(const QDomElement& node);
	virtual ~MidiMessageSerato();

	static int eventType2MidiStatusByte(QString midiEventType, bool* ok = NULL);
	static QString name(const QDomElement& node);
public:
   unsigned short  m_midiStatusByte; /** Complete 8-bit status byte (including category and channel). */
   unsigned short  m_midiNo;         /** The second MIDI byte. */
   ushort          m_midiChannel;

};

#endif /* MIDIMESSAGESERATO_H_ */
