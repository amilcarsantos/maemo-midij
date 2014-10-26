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

#include "MidiObjectUdp.h"


// **************************** //
// ** System Common Messages ** //
// **************************** //
#ifdef _DEBUG_C
	#pragma mark -
	#pragma mark System Common Messages
#endif

// MIDI_SYSEX:						240
#define MIDI_SYSEX					0xF0

// MIDI_TIMECODE:					241
// MIDI_SONG_POSITION_POINTER		242
// MIDI_SONG_SELECT					243
// MIDI_TUNE_REQUEST				246
// MIDI_EOX (END OF SYSEX)			247
#define MIDI_TIMECODE				0xF1
#define MIDI_SONG_POSITION_POINTER	0xF2
#define MIDI_SONG_SELECT			0xF3
#define MIDI_TUNE_REQUEST			0xF6
#define MIDI_EOX					0xF7



UdpPorts::UdpPorts()
{
	bindPort = 0;
	targetPort = 0;
}

UdpPorts::UdpPorts(quint16 bind, quint16 target)
{
	bindPort = bind;
	targetPort = target;
};

UdpPorts::UdpPorts(const UdpPorts& np)
{
	bindPort = np.bindPort;
	targetPort = np.targetPort;
};



#define MIDI_OBJECT_MASK	(MIDI_OBJECT_ONLY_INPUT | MIDI_OBJECT_ONLY_OUTPUT)

MidiObjectUdp::MidiObjectUdp(ChannelType type)
	: MidiObject(MIDI_OBJECT_NO_SCRIPT + MIDI_OBJECT_NO_MAPPING + (type & MIDI_OBJECT_MASK))
{
	m_socket = new QUdpSocket(this);
	m_targetPort = 0;
	m_clientPort = 0; // BidirServer only
	m_isClient = (type & BidirClient) == BidirClient
			|| (type & OutputOnly) == OutputOnly;
}

MidiObjectUdp::~MidiObjectUdp()
{
	if (!m_deviceName.isEmpty())
		devClose();
}

void MidiObjectUdp::devOpen(QString device)
{
	// already open?
	if (!m_deviceName.isEmpty())
		return;

	if (!m_deviceUdpPortsMap.contains(device))
	{
		qCritical() << "not a valid device:" << device;
		return;
	}
	// device name like "UDP [port]"
	m_deviceName = device;
	UdpPorts udpPorts = m_deviceUdpPortsMap.value(device);
	m_targetPort = udpPorts.targetPort;

	qDebug() << "MidiObjectUdp: bind port" << udpPorts.bindPort << "; target port" << m_targetPort;
	if ((m_restrictionFlags & MIDI_OBJECT_ONLY_OUTPUT) == 0) // only input or bidirectional
	{
		bool binded;
#ifdef __MIDIUDP_LOCALHOST__
		qDebug() << "MidiObjectUdp::bind() to localhost:" << udpPorts.bindPort;
		binded = m_socket->bind(QHostAddress::LocalHost, udpPorts.bindPort, QUdpSocket::ShareAddress);
#else
		binded = m_socket->bind(udpPorts.bindPort, QUdpSocket::ShareAddress);
#endif
		if (binded)
			connect(m_socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
		else
		{
			qCritical() << "MidiObjectUdp::bind() fail to bind port:" << udpPorts.bindPort;
			m_deviceName.clear();
			m_targetPort = 0;
		}
	}
}

void MidiObjectUdp::devClose()
{
	m_deviceName.clear();
	m_targetPort = 0;
	m_client.clear();
	m_clientPort = 0;
	m_socket->close();
	m_socket->disconnect(this);
}

void MidiObjectUdp::addDevice(QString name, quint16 bindPort, quint16 targetPort)
{
	devices.append(name);
	m_deviceUdpPortsMap.insert(name, UdpPorts(bindPort, targetPort));
}

void MidiObjectUdp::updateDeviceList()
{
	if (devices.isEmpty() && m_isClient)
	{
		// place this in a config file?
		addDevice("UDP/IP [5004/5]", 5005, 5004);
		addDevice("UDP/IP [5004]", 0, 5004);
		addDevice("UDP/IP [5005]", 0, 5005);
		addDevice("UDP/IP [9000]", 0, 9000);
		addDevice("UDP/IP [21928/9]", 21929, 21928);
		addDevice("UDP/IP [21928]", 0, 21928);
		addDevice("UDP/IP [21929]", 0, 21929);
		addDevice("UDP/IP [21930]", 0, 21930);
		addDevice("UDP/IP [21931]", 0, 21931);
		addDevice("UDP/IP [21932]", 0, 21932);
		return;
	}

	if (devices.isEmpty() && !m_isClient)
	{
		addDevice("UDP/IP [5004/5]", 5004, 5005);
		addDevice("UDP/IP [5004]", 5004, 0);
		addDevice("UDP/IP [5005]", 5005, 0);
		addDevice("UDP/IP [9000]", 9000, 0);
		addDevice("UDP/IP [21928/9]", 21928, 21929);
		addDevice("UDP/IP [21928]", 21928, 0);
		addDevice("UDP/IP [21929]", 21929, 0);
		addDevice("UDP/IP [21930]", 21930, 0);
		addDevice("UDP/IP [21931]", 21931, 0);
		addDevice("UDP/IP [21932]", 21932, 0);
		return;
	}
}

void MidiObjectUdp::sendShortMsg(uint word)
{
	qDebug() << "MidiObjectUdp::sendShortMsg() " << (Qt::HANDLE)word;

    QByteArray datagram;
	datagram.append(word & 0xff).append((word>>8) & 0xff).append((word>>16) & 0xff) ;
#ifdef __MIDIUDP_LOCALHOST__
	qDebug() << "MidiObjectUdp::sendShortMsg() to localhost:" << m_targetPort;
	m_socket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::LocalHost, m_targetPort);
#else
	m_socket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, m_targetPort);
#endif
}

void MidiObjectUdp::sendSysexMsg(unsigned char data[], unsigned int length)
{
//	qDebug() << "MidiObjectUdp::sendSysexMsg() " << length;
	quint16 port = m_targetPort;
	if (!m_isClient)
	{
		// no client is active
		if (m_clientPort == 0)
			return;
		port = m_clientPort;
	}

#ifdef __MIDIUDP_LOCALHOST__
	qDebug() << "MidiObjectUdp::sendShortMsg() to localhost:" << port;
	m_socket->writeDatagram((const char*)data, length, QHostAddress::LocalHost, port);
#else
	m_socket->writeDatagram((const char*)data, length, QHostAddress::Broadcast, port);
#endif
}

void MidiObjectUdp::processPendingDatagrams()
{
//	qDebug() << "MidiObjectUdp::processPendingDatagrams()";
	if (!m_isClient && m_clientPort == 0)
	{
		// try to read client info
		QByteArray datagram;
		datagram.resize(m_socket->pendingDatagramSize());
		m_socket->readDatagram(datagram.data(), datagram.size(), &m_client, &m_clientPort);
		processDatagram(datagram);
	}
	// more datagrams...
	while (m_socket->hasPendingDatagrams())
	{
		QByteArray datagram;
		datagram.resize(m_socket->pendingDatagramSize());
		m_socket->readDatagram(datagram.data(), datagram.size());
//		statusLabel->setText(tr("Received datagram: \"%1\"")
//							 .arg(datagram.data()));
		processDatagram(datagram);
	}
}

void MidiObjectUdp::processDatagram(const QByteArray& datagram)
{
	if (datagram.size() >= 3)
	{
		if ((uchar)datagram.at(0) == MIDI_SYSEX)
		{
//			qDebug("processing SYSEX datagram");
			emit(midiSysexMsg((uchar*)datagram.data(), datagram.size()));
		}
		else
		{
//			qDebug("processing short msg datagram");
			receive((MidiStatusByte)(datagram.at(0) & 0xf0), datagram.at(0) & 0xf, datagram.at(1), datagram.at(2));
		}
	}
}
