#include <QtTest>
#include "mixxx/MidiObject.h"
#include "MidiObjectUdp.h"
#include "FakeMidiReceiver.h"


class TestMidiUdp : public QObject
{
	Q_OBJECT

private slots:
	void initTestCase()
	{
		pcMidiObject = new MidiObjectUdp(MidiObjectUdp::BidirServer);
		n900MidiObject = new MidiObjectUdp(MidiObjectUdp::BidirClient);

		pcMidiReceiver.setTraceName("PC");
		n900MidiReceiver.setTraceName("N900");

		connect(pcMidiObject, SIGNAL(midiThru(MidiMessage,char)), &pcMidiReceiver, SLOT(receiveMidiMessage(MidiMessage,char)), Qt::DirectConnection);

		// connect SysexMsgs
		connect(pcMidiObject, SIGNAL(midiSysexMsg(uchar*,uint)), &pcMidiReceiver, SLOT(receiveSysexMsg(uchar*,uint)), Qt::DirectConnection);
		connect(n900MidiObject, SIGNAL(midiSysexMsg(uchar*,uint)), &n900MidiReceiver, SLOT(receiveSysexMsg(uchar*,uint)), Qt::DirectConnection);

	};

	void testFailOpenDevice();
	void testOpenDevices();
	void testn900SendNoteOnOff();
	void testPcSendSysexIdentityRequest();

	void cleanupTestCase()
	{
		delete pcMidiObject;
		delete n900MidiObject;
	};

public:
	MidiObjectUdp* pcMidiObject;
	MidiObject* n900MidiObject;

	FakeMidiReceiver pcMidiReceiver;
	FakeMidiReceiver n900MidiReceiver;
};

void TestMidiUdp::testFailOpenDevice()
{
	QString device = "Any";
	QStringList* pcDevices = pcMidiObject->getDeviceList();
	QVERIFY2(!pcDevices->contains(device), "fail test device was found");
	QVERIFY(pcMidiObject->getOpenDevice().isEmpty());
	pcMidiObject->devOpen(device);
	QVERIFY2(pcMidiObject->getOpenDevice().isEmpty(), "pc midi object has open an invalid device");
}


void TestMidiUdp::testOpenDevices()
{
	QStringList* pcDevices = pcMidiObject->getDeviceList();
	QStringList* n900Devices = n900MidiObject->getDeviceList();
	QVERIFY2(pcDevices, "device list is null");
	QVERIFY2(n900Devices, "device list is null");

	QString device = "UDP [5004]";
	QVERIFY2(pcDevices->contains(device), "test device not found");
	QVERIFY2(n900Devices->contains(device), "test device not found");

	pcMidiObject->devOpen(device);
	QVERIFY(!pcMidiObject->getOpenDevice().isEmpty());
	n900MidiObject->devOpen(device);
	QVERIFY(!n900MidiObject->getOpenDevice().isEmpty());
}

void TestMidiUdp::testn900SendNoteOnOff()
{
	QVERIFY(!n900MidiObject->getOpenDevice().isEmpty() && !pcMidiObject->getOpenDevice().isEmpty());

	pcMidiReceiver.clearPendingMsgs();
	n900MidiReceiver.clearPendingMsgs();

	n900MidiObject->sendShortMsg(NOTE_ON, 0x69, 0x7F);
	QTest::qWait(250); // process network events
	QCOMPARE(pcMidiReceiver.receivedShortMsgCount(), 1);
	QVERIFY2(pcMidiReceiver.receivedShortMsg(NOTE_ON, 0x69, 0x7F), "fail to receive Note ON");

	pcMidiReceiver.clearPendingMsgs();
	n900MidiObject->sendShortMsg(NOTE_OFF, 0x69, 0x7F);
	QTest::qWait(250); // process network events
	QCOMPARE(pcMidiReceiver.receivedShortMsgCount(), 1);
	QVERIFY2(pcMidiReceiver.receivedShortMsg(NOTE_OFF, 0x69, 0x7F), "fail to receive Note OFF");
}

void TestMidiUdp::testPcSendSysexIdentityRequest()
{
	pcMidiReceiver.clearPendingMsgs();
	n900MidiReceiver.clearPendingMsgs();

	QByteArray identReqSysexMsg;
	identReqSysexMsg.append(0xF0); // SysEx
	identReqSysexMsg.append(0x7e); // Non-Realtime
	identReqSysexMsg.append(0x7f); // The SysEx channel.
	identReqSysexMsg.append(0x06); // Sub-ID -- General Information
	identReqSysexMsg.append(0x01); // Sub-ID2 -- Identity Request
	identReqSysexMsg.append(0xF7); // End of SysEx
	// IdentityRequest
	pcMidiObject->sendSysexMsg((uchar*)identReqSysexMsg.data(), identReqSysexMsg.size());
	QTest::qWait(250); // process network events
	QVERIFY2(n900MidiReceiver.pendingSysexMsg(identReqSysexMsg), "fail to send/receive SysexIdentityRequest msg");

	// IdentityReply
	QByteArray identReplySysexMsg;
	identReplySysexMsg.append(0xF0); // SysEx
	identReplySysexMsg.append(0x7e); // Non-Realtime
	identReplySysexMsg.append(0x7f); // The SysEx channel.
	identReplySysexMsg.append(0x06); // Sub-ID -- General Information
	identReplySysexMsg.append(0x02); // Sub-ID2 -- Identity Reply...
	identReplySysexMsg.append(0x01); // dummy data
	identReplySysexMsg.append(0xF7); // End of SysEx
	n900MidiReceiver.clearPendingMsgs();
	n900MidiObject->sendSysexMsg((uchar*)identReplySysexMsg.data(), identReplySysexMsg.size());
	QTest::qWait(250); // process network events
	QVERIFY2(pcMidiReceiver.pendingSysexMsg(identReplySysexMsg), "fail to send/receive SysexIdentityReply msg");

}




QTEST_MAIN(TestMidiUdp)
#include "TestMidiUdp.moc"

