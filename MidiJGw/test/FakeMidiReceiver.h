/*
 * FakeMidiReceiver.h
 *
 *  Created on: 4 de Mar de 2011
 *      Author: pt101821
 */

#ifndef FAKEMIDIRECEIVER_H_
#define FAKEMIDIRECEIVER_H_

#include <QObject>
#include "mixxx/MidiMessage.h"

class FakeMidiReceiver : public QObject
{
	Q_OBJECT

public:
	FakeMidiReceiver();
	virtual ~FakeMidiReceiver();
	void setTraceName(QString name);

public slots:
	void receiveMidiMessage(MidiMessage message, char value);
	void receiveSysexMsg(uchar* data, uint size);

public:
	void clearPendingMsgs();

	bool receivedShortMsgCount();
	bool receivedShortMsg(uchar status, uchar byte1, uchar byte2);

	bool pendingSysexMsg(const QByteArray& startMsg);

protected:
	QList<QByteArray> m_pendingReceiveSysexMsg;
	QList<quint32> m_pendingReceiveShortMsg;
	QString m_traceName;

};

inline void FakeMidiReceiver::setTraceName(QString name)
{
	m_traceName = name;
}

inline void FakeMidiReceiver::clearPendingMsgs()
{
	m_pendingReceiveSysexMsg.clear();
	m_pendingReceiveShortMsg.clear();
}

inline bool FakeMidiReceiver::receivedShortMsgCount()
{
	return m_pendingReceiveShortMsg.size();
}

#endif /* FAKEMIDIRECEIVER_H_ */
