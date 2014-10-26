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

#ifndef NETCASTMANAGER_H_
#define NETCASTMANAGER_H_

#include <QtCore>

class GstUdpAudioPlayer;

class NetcastManager
{
public:
	struct Params
	{
		QString ipAddress;
		int udpBufferSize;
	};
public:
	NetcastManager();
	virtual ~NetcastManager();

public:
	void init();
	void close();
	// pParams values may change
	void setConnection(const QString& connection, Params* pParams);
	QStringList connectionList();

	bool hasError();
	QString errorMessage();

private:
	void addNetcast(const QString& name, quint16 port);
	QStringList m_connectionList;
	QString m_activeConnection;
	GstUdpAudioPlayer* m_udpAudioPlayer;
	QMap<QString, quint16> m_udpPortsMap;
};

#endif /* NETCASTMANAGER_H_ */
