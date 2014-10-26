/***************************************************************************
                           midideviceudp.h
                    Network UDP Broadcast MIDI Device Class
                    --------------------------------
    begin                : Fri Aug 26 2011
    copyright            : (C) 2011 Amilcar Santos
    email                : amilcar.santos@gmail.com

    This class handles network MIDI devices that use simple UDP broadcast messages
    has the comunication protocol.

***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "MidiDeviceUdp.h"

MidiDeviceUdpInfo::MidiDeviceUdpInfo() {
	deviceName = "UDP []";
	bindPort = 0;
	targetPort = 0;
}

MidiDeviceUdpInfo::MidiDeviceUdpInfo(QString name, quint16 bind, quint16 target) {
	deviceName = name;
	bindPort = bind;
	targetPort = target;
};

MidiDeviceUdpInfo::MidiDeviceUdpInfo(const MidiDeviceUdpInfo& np) {
	deviceName = np.deviceName;
	bindPort = np.bindPort;
	targetPort = np.targetPort;
};

MidiDeviceUdp::MidiDeviceUdp(MidiMapping* mapping, ChannelType type, const MidiDeviceUdpInfo& info)
    : MidiDevice(mapping) {
    m_strDeviceName = info.deviceName;
	m_bIsOutputDevice = !InputOnly;
	m_bIsInputDevice = !OutputOnly;

    m_socket = new QUdpSocket();
    m_targetPort = info.targetPort;
    m_bindPort = info.bindPort;
    m_type = type;
    m_clientPort = 0; // BidirServer only
    m_isServer = (type == BidirServer) || (type & InputOnly) == InputOnly;
}

MidiDeviceUdp::~MidiDeviceUdp()
{
    close();
    if (m_socket)
        delete m_socket;	
}

int MidiDeviceUdp::open()
{
    QMutexLocker Locker(&m_mutex); //Make this function thread safe.
    // already open?
    if (m_bIsOpen) {
        qDebug() << "MIDI UDP device" << m_strDeviceName << "already open";
        return -1;
    }
    m_bReceiveInhibit = false;
    Locker.unlock();
    startup();
    Locker.relock();
	qDebug() << "MidiDeviceUdp: bind port" << m_bindPort << "; target port" << m_targetPort;
	if ((m_type & OutputOnly) == 0) // only input or bidirectional
	{
		if (m_bindPort < 1024) { // well known service ports are invalid...
			qDebug() << "MIDI UDP device" << m_strDeviceName << "invalid bind port";
			return -2;
		}

#ifdef __MIDIUDP_LOCALHOST__
		qDebug() << "MidiDeviceUdp::bind() to localhost:" << m_bindPort;
		m_socket->bind(QHostAddress::LocalHost, m_bindPort, QUdpSocket::ShareAddress);
#else
		if (!m_socket->bind(m_bindPort, QUdpSocket::ShareAddress)) {
			qDebug() << "MIDI UDP device" << m_strDeviceName << "fail to bind";
			return -2;
		}
#endif
//		connect(m_socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
	}
    m_bIsOpen = true;

	moveToThread(this);
	start();

	return 0;
}

int MidiDeviceUdp::close()
{
    setReceiveInhibit(true);    // Prevent deadlock

    if (!m_bIsOpen) {
        qDebug() << "MIDI UDP device" << m_strDeviceName << "already closed";
        return -1;
    }
    shutdown();
	exit(0);

    //shutdown() locks so we must lock after it.
    QMutexLocker Locker(&m_mutex);
	m_clientPort = 0;
	m_socket->disconnect();
	m_socket->close();
    m_bIsOpen = false;
	return 0;
}

void MidiDeviceUdp::run() {
    QThread::currentThread()->setObjectName(QString("PM %1").arg(m_strDeviceName));
	connect(m_socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
	exec();
}

void MidiDeviceUdp::sendShortMsg(uint word) {
//    QMutexLocker Locker(&m_mutex);
//	qDebug() << "MidiDeviceUdp::sendShortMsg() " << (Qt::HANDLE)word;
	quint16 port = m_targetPort;
	if (m_isServer) {
		// no client?
		if (m_clientPort == 0)
			return;
		port = m_clientPort;
	}

    QByteArray datagram;
	datagram.append(word & 0xff).append((word>>8) & 0xff).append((word>>16) & 0xff) ;
#ifdef __MIDIUDP_LOCALHOST__
	qDebug() << "MidiDeviceUdp::sendShortMsg() to localhost:" << m_targetPort;
	m_socket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::LocalHost, port);
#else
	m_socket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, port);
#endif
}

void MidiDeviceUdp::sendSysexMsg(unsigned char data[], unsigned int length) {
//    QMutexLocker Locker(&m_mutex); // already locked???
//	qDebug() << "MidiDeviceUdp::sendSysexMsg() " << length;
	quint16 port = m_targetPort;
	if (m_isServer) {
		// no client?
		if (m_clientPort == 0)
			return;
		port = m_clientPort;
	}

#ifdef __MIDIUDP_LOCALHOST__
	qDebug() << "MidiDeviceUdp::sendShortMsg() to localhost:" << port;
	m_socket->writeDatagram((const char*)data, length, QHostAddress::LocalHost, port);
#else
	m_socket->writeDatagram((const char*)data, length, QHostAddress::Broadcast, port);
#endif
}

void MidiDeviceUdp::processPendingDatagrams() {
//	qDebug() << "MidiDeviceUdp::processPendingDatagrams()";
	if (m_isServer && m_clientPort == 0) {
		// try to read client info
		QByteArray datagram;
		datagram.resize(m_socket->pendingDatagramSize());
		m_socket->readDatagram(datagram.data(), datagram.size(), &m_client, &m_clientPort);
		processDatagram(datagram);
	}
	// more datagrams...
	while (m_socket->hasPendingDatagrams()) {
		QByteArray datagram;
		datagram.resize(m_socket->pendingDatagramSize());
		m_socket->readDatagram(datagram.data(), datagram.size());
		processDatagram(datagram);
	}
}

void MidiDeviceUdp::processDatagram(const QByteArray& datagram) {
	if (datagram.size() >= 3) {
		if ((uchar)datagram.at(0) == MIDI_STATUS_SYSEX) {
#ifdef __MIDISCRIPT__
//			qDebug("processing SYSEX datagram");
			receive((uchar*)datagram.data(), datagram.size());
#endif
		} else {
//			qDebug("processing short msg datagram");
			receive((MidiStatusByte)(datagram.at(0) & 0xf0), datagram.at(0) & 0xf, datagram.at(1), datagram.at(2));
		}
	}
}
