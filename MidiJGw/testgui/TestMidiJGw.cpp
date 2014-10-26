#include <QtTest>
#include "MidiJGw.h"
#include "MidiObjectUdp.h"
#include "mixxx/MidiObjectWin.h"
#include "../test/FakeMidiReceiver.h"

class TestMidiJGw : public QObject
{
	Q_OBJECT

private slots:
	void initTestCase()
	{
		QCoreApplication::setOrganizationName("MySoft");
		QCoreApplication::setOrganizationDomain("mysoft.com");
		QCoreApplication::setApplicationName("TestMidiJGw");
		QSettings st;
		st.setValue("MIDI1", "Out To MIDI Yoke:  1");
		st.setValue("MidiCtrl1", "UDP [5005]");
		st.sync();

		qDebug() << "---------------------------";
		w = new MidiJGw();
		qDebug() << "---------------------------";

		midiNet = new MidiObjectUdp(MidiObjectUdp::BidirClient);
		midiNet->updateDeviceList();
		midiNet->devOpen("UDP [5005]");

		midiLoopbe = new MidiObjectWin(MIDI_OBJECT_NO_SCRIPT + MIDI_OBJECT_NO_MAPPING + MIDI_OBJECT_ONLY_INPUT);
		midiLoopbe->devOpen("In From MIDI Yoke:  1");
		connect(midiLoopbe, SIGNAL(midiThru(MidiMessage,char)), &loopbeReceiver, SLOT(receiveMidiMessage(MidiMessage,char)), Qt::DirectConnection);
	}

	void testSend1stNote();

	void cleanupTestCase()
	{
		midiLoopbe->devClose();
		delete midiLoopbe;

		midiNet->devClose();
		delete midiNet;

		w->close();
		delete w;
	}

public:
	MidiJGw* w;
	MidiObject* midiNet;
	MidiObject* midiLoopbe;
	FakeMidiReceiver loopbeReceiver;
};


void TestMidiJGw::testSend1stNote()
{
	qDebug("TestMidiJGw");
	midiNet->sendShortMsg(NOTE_ON, 0x69, 0x7F);
	QTest::qWait(450);
	QVERIFY2(loopbeReceiver.receivedShortMsg(NOTE_ON, 0x69, 0x7F), "fail to receive Note ON msg");

	loopbeReceiver.clearPendingMsgs();
	midiNet->sendShortMsg(NOTE_OFF, 0x69, 0x0);
	QTest::qWait(450);
	QVERIFY2(loopbeReceiver.receivedShortMsg(NOTE_OFF, 0x69, 0x0), "fail to receive Note OFF msg");
}


QTEST_MAIN(TestMidiJGw)
#include "TestMidiJGw.moc"

