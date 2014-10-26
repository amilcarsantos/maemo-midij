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

#include "NetcastManager.h"
#include "Utils.h"
#include "gst/GstUdpAudioPlayer.h"
#include <QtNetwork>

NetcastManager::NetcastManager()
{
	m_udpAudioPlayer = NULL;
}

NetcastManager::~NetcastManager()
{
	_delete(m_udpAudioPlayer);
}

void NetcastManager::init()
{
	m_udpAudioPlayer = new GstUdpAudioPlayer();
	m_udpAudioPlayer->init();
	m_connectionList.append("Disabled");
	addNetcast("UDP/IP [1234]", 1234);
	addNetcast("UDP/IP [4951]", 4951);
}

void NetcastManager::close()
{
	if (m_udpAudioPlayer)
		m_udpAudioPlayer->stop();
}

void NetcastManager::addNetcast(const QString& name, quint16 port)
{
	m_connectionList.append(name);
	m_udpPortsMap.insert(name, port);
}

void NetcastManager::setConnection(const QString& connection, Params* pParams)
{
	qDebug(QString("m_activeConnection: '%1' ; newConnection: '%2'").arg(m_activeConnection).arg(connection).toAscii());

	if (m_udpPortsMap.contains(m_activeConnection))
		m_udpAudioPlayer->stop();

	if (m_udpPortsMap.contains(connection))
	{
		QString targetAddress;
		if (!pParams->ipAddress.isEmpty())
		{
			QHostAddress testAddress;
			if (testAddress.setAddress(pParams->ipAddress))
			{
				targetAddress = testAddress.toString();
			}
			else
			{
				// invalid IP
				pParams->ipAddress.clear();
			}
		}
		m_udpAudioPlayer->setUdpParams(targetAddress, m_udpPortsMap.value(connection), pParams->udpBufferSize);
		m_udpAudioPlayer->play();
		m_activeConnection = connection;
		return;
	}
	// disabled
	m_activeConnection = m_connectionList.at(0);
}

QStringList NetcastManager::connectionList()
{
	return m_connectionList;
}

bool NetcastManager::hasError()
{
	if (m_udpAudioPlayer)
		return m_udpAudioPlayer->hasError();
	return false;
}

QString NetcastManager::errorMessage()
{
	if (m_udpAudioPlayer)
		return m_udpAudioPlayer->errorMessage();
	return QString();
}

