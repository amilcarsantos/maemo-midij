/*
 * MidiJ - Remote MIDI controller for DJs
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

#ifndef GSTUDPAUDIOPLAYER_H_
#define GSTUDPAUDIOPLAYER_H_

#include "GstEngine.h"

class GstUdpAudioPlayer : public GstEngine
{
public:
	GstUdpAudioPlayer(QObject* parent=0);
	virtual ~GstUdpAudioPlayer();

public:
	virtual void setVolume(int percent);
	virtual int volume();
	void setUdpParams(const QString& ipAddress, int port, int bufferSize);

protected:
	virtual const char* createElements();

protected:
	GstElement* m_playbin;
};

#endif /* GSTUDPAUDIOPLAYER_H_ */
