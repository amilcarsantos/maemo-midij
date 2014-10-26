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

#ifndef MIDIOBJECTGUIEMUL_H_
#define MIDIOBJECTGUIEMUL_H_

#include "mixxx/MidiObject.h"

class MidiObjectGuiEmul : public MidiObject
{
	Q_OBJECT
public:
	MidiObjectGuiEmul();
	virtual ~MidiObjectGuiEmul();

public slots:
	void emulReceive(uchar status, uchar midicontrol, uchar midivalue);

signals:
	void emulSendShortMsg(uchar status, uchar midicontrol, uchar midivalue);
	void emulSendSysexMsg(uchar *data, uint length);

// Overrides
public:
	virtual void devOpen(QString device);
	virtual void devClose();
	virtual void sendShortMsg(uint word);
	virtual void sendSysexMsg(unsigned char data[], unsigned int length);

};

#endif /* MIDIOBJECTGUIEMUL_H_ */
