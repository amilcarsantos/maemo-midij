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

#ifndef MIDIOBJECTUDP_H_
#define MIDIOBJECTUDP_H_

#include "mixxx/MidiObject.h"
#include <QtNetwork>

struct UdpPorts
{
	UdpPorts();
	UdpPorts(quint16 bind, quint16 target);
	UdpPorts(const UdpPorts& np);

	quint16 bindPort;
	quint16 targetPort;
};

class MidiObjectUdp : public MidiObject
{
	Q_OBJECT
public:
    enum ChannelType {
        BidirServer = 0x00,
        BidirClient = 0x01,
        InputOnly   = 0x10,
        OutputOnly  = 0x20,
    };
	MidiObjectUdp(ChannelType type);
	virtual ~MidiObjectUdp();

private slots:
	void processPendingDatagrams();

signals:
	void midiSysexMsg(uchar* data, uint size);

public:
	virtual void devOpen(QString device);
	virtual void devClose();
	virtual void updateDeviceList();

	virtual void sendShortMsg(uint word);
    virtual void sendSysexMsg(unsigned char data[], unsigned int length);

protected:
	void addDevice(QString name, quint16 bindPort, quint16 targetPort);
	void processDatagram(const QByteArray& datagram);

	QUdpSocket* m_socket;
	bool m_isClient;
	int m_targetPort;
	QMap<QString, UdpPorts> m_deviceUdpPortsMap;

	// BidirServer mode
	QHostAddress m_client;
	quint16 m_clientPort;
};

#endif /* MIDIOBJECTUDP_H_ */
