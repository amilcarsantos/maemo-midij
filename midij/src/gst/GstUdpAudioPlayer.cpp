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

#include "GstUdpAudioPlayer.h"
#include "gstplay-enum.h"

GstUdpAudioPlayer::GstUdpAudioPlayer(QObject *parent)
	: GstEngine(parent)
{
	m_playbin = NULL;
}

GstUdpAudioPlayer::~GstUdpAudioPlayer()
{
	stop();
//	removeElement(m_playbin);
}

const char* GstUdpAudioPlayer::createElements()
{
	m_playbin = gst_element_factory_make("playbin2", "player");
	if (m_playbin == NULL)
		return "Fail to load player";
/*	int e;
	g_object_get(G_OBJECT(m_playbin), "flags", &e, NULL);
	qDebug(QString("m_playbin - flags %1").arg(e, 0, 16).toAscii());*/
	g_object_set(G_OBJECT(m_playbin), "flags", GST_PLAY_FLAG_AUDIO | GST_PLAY_FLAG_SOFT_VOLUME | GST_PLAY_FLAG_BUFFERING, NULL);
	addElement(m_playbin);

	// All ok
	return NULL;
}

void GstUdpAudioPlayer::setVolume(int percent)
{
	g_object_set(G_OBJECT(m_playbin), "volume", ((double)percent)*0.01, NULL);
}

int GstUdpAudioPlayer::volume()
{
	double volume = 0.0;
	g_object_get(G_OBJECT(m_playbin), "volume", &volume, NULL);
	return volume * 100;
}

void GstUdpAudioPlayer::setUdpParams(const QString& ipAddress, int port, int bufferSize)
{
//	qDebug(QString("GstUdpAudioPlayer::setUdpParams-ip:%1,port:%2,buf:%3").arg(ipAddress).arg(port).arg(bufferSize).toAscii());
	if (port >= 0 && port <=65535)
	{
		QString address = "255.255.255.255"; // Broadcast by default
		if (!ipAddress.isEmpty())
		{
			address = ipAddress;
		}
		QString uri = QString("udp://%1:%2").arg(address).arg(port);
		g_object_set(G_OBJECT(m_playbin), "uri", uri.toAscii().data(), NULL);
	}
	if (bufferSize >= 0)
		g_object_set(G_OBJECT(m_playbin), "buffer-size", bufferSize, NULL);
}
