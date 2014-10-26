#include <QtTest>
#include "FakeMidiObject.h"
#include "MidiObjectGuiEmul.h"
#include "gui/MidiMappingGui.h"
#include "gui/GraphicsMidiControlItem.h"
#include "RouterObject.h"
#include "serato/MidiMappingSerato.h"
#include <QGraphicsSceneEvent>
#include <QScriptEngineDebugger>
#include <QGraphicsScene>
#include "Utils.h"

class TestMidiJ : public QObject
{
    Q_OBJECT

private slots:
	void initTestCase()
	{
		MidiObject* m_midiConnection = &fakeMidiToPC;
		m_deckB = false;

		QFileInfo xmlPath("data/schemes/midij_Stanton_SSL.xml");
		if (!QDir::setCurrent(xmlPath.absolutePath()))
		{
			QFAIL(QString("fail to change dir:%1").arg(xmlPath.absolutePath()).toAscii());
			return;
		}

		// load GUI mappings
		QList<QGraphicsItem*> guiItems;
		m_guiMapping.loadScheme(xmlPath.fileName(), guiItems);
		QVERIFY(guiItems.size() > 1);
	    foreach (QGraphicsItem *item, guiItems)
			m_scene.addItem(item);

	    QVERIFY(findMidiControl("Play") != NULL);	//QVERIFY(m_guiMapping.m_inputMapping.count() > 0);

//		m_guiMapping.connectMidiOutPort(&midiGuiEmul, SLOT(emulReceive(uchar,uchar,uchar)));
//		m_guiMapping.connectMidiInPort(&midiGuiEmul);
//		connect(midiGuiEmul);
//
//		m_router.setMidiObjects(&midiGuiEmul, &fakeMidiToPC);
//		connect(m_midiConnection, SIGNAL(midiSysexMsgOut(uchar*,uint)), &m_guiMapping, SLOT(midiSysexMsgIn(uchar*,uint)), Qt::DirectConnection);
//		connect(&m_guiMapping, SIGNAL(midiSysexMsgOut(uchar*,uint)), &m_router, SLOT(midiGatewaySysexMsgIn(uchar*,uint)), Qt::DirectConnection);

	    if (true)
	    {
	    	MidiObject* m_midiControler = &midiGuiEmul;
	    	// setup connections:
	    	// GUI -> MidiController/Engine
	    	m_guiMapping.connectMidiOutPort(m_midiControler, SLOT(emulReceive(uchar,uchar,uchar)));
	    	connect(m_midiControler, SIGNAL(emulSendShortMsg(uchar,uchar,uchar)), &m_guiMapping, SLOT(midiShortMsgIn(uchar,uchar,uchar)));
	    	connect(m_midiControler, SIGNAL(emulSendSysexMsg(uchar*,uint)), &m_guiMapping, SLOT(midiSysexMsgIn(uchar*,uint)), Qt::DirectConnection);


	    	// MidiController/Engine -> Router -> Gateway
	    	m_router.setMidiObjects(m_midiControler, m_midiConnection);
	    	connect(m_midiControler, SIGNAL(midiThru(MidiMessage,char)), &m_router, SLOT(midiSourceReceive(MidiMessage,char)));
	    	connect(m_midiControler->getMidiScriptEngine(), SIGNAL(initialized()), &m_router, SLOT(sourceReload()));

	    	// Gateway -> GUI mapping (SysEx messages from gateway)
	    	connect(m_midiConnection, SIGNAL(midiSysexMsgOut(uchar*,uint)), &m_guiMapping, SLOT(midiSysexMsgIn(uchar*,uint)), Qt::DirectConnection);

	    	// GUI mapping -> Router (SysEx messages reply)
	    	connect(&m_guiMapping, SIGNAL(midiSysexMsgOut(uchar*,uint)), &m_router, SLOT(midiGatewaySysexMsgOut(uchar*,uint)), Qt::DirectConnection);
	    }


		// load Engine mappings
	    QDomElement engineRoot = m_guiMapping.sourcePreset("engine");
	    {
	    	DirContext dc(QFileInfo(MidiMappingGui::sourceFile(engineRoot)));
			midiGuiEmul.getMidiMapping()->loadPreset(engineRoot, false);
			midiGuiEmul.getMidiMapping()->callInit(); // same has MidiJ class
			//QDir::setCurrent(xmlPath.absolutePath());
	    }
		qDebug() << "current dir (out):" << QDir::currentPath();
		// load Router mappings
		qDebug("router mappings....");
		m_router.loadMappings(m_guiMapping.sourcePreset("router"), m_guiMapping.sourcePreset("router.gateway"));
		QCOMPARE(m_router.lastError(), 0);

		QVERIFY(midiGuiEmul.getMidiMapping()->numInputMidiMessages() > 1);
		QCOMPARE(midiGuiEmul.getMidiMapping()->numOutputMixxxControls(), 0);

		QScriptEngine* engine = m_router.m_pEngine->getEngine();
	    //debugger.attachTo(engine);
	    engine->globalObject().setProperty("print", engine->newFunction(myprint, 1));

		qDebug() << "___________________________________________________________________________________";
		qDebug() << "___________________________________________________________________________________";
		qDebug() << "___________________________________________________________________________________";
//		QTest::qWait(1);
	};


//	void testLoadMapping();
//	void testPressButtonFx();
	void testGwSendSysexIdentityRequest();
	void testPressButtonLoop();
	void testPressSurfaceButtonB1();




	void  cleanupTestCase()
	{
		m_scene.clear();
	};
	static void setMouseEventPos(QGraphicsSceneMouseEvent& event, GraphicsMidiControlItem* control, qreal x, qreal y)
	{
		event.setPos(QPointF(x, y));
		event.setScenePos(control->sceneRect().topLeft() + QPointF(x, y));
	}

public:
	// ...DONT TEST....
	void testPressButtonTrig();
	void testPressButtonVinyl();
	void testPressButtonDeck();

	void testPressSurfaceButtonB5();

	void testPressButtonPlay();
	void testPressButtonCue();
	void testPressButtonGain();

	void testPressVinylSliderS4();
	void testTouchVinylCircleC1_abs();
	// ---DONT TEST----




protected:
	MidiObjectGuiEmul midiGuiEmul;
	FakeMidiObject fakeMidiToPC;
	MidiMappingGui m_guiMapping;
	QGraphicsScene m_scene;
	RouterObject   m_router;
	QScriptEngineDebugger debugger;
	bool m_deckB;

	// utils
	GraphicsMidiControlItem* findMidiControl(QString name);
	void resetSchemeObjs();
	static QScriptValue myprint(QScriptContext* sc, QScriptEngine* se);
};

/*QDomElement openXMLFile(QString path, QString name)
{
    QDomDocument doc(name);
    QFile file(path);
    if (!file.open(QFile::ReadOnly))
    {
        qDebug() << "Could not open xml file:" << file.fileName();
        return QDomElement();
    }
    if (!doc.setContent(&file))
    {
        qWarning() << "Error parsing xml file:" << file.fileName();
        file.close();
        return QDomElement();
    }

    file.close();
    return doc.documentElement();
}*/

GraphicsMidiControlItem* TestMidiJ::findMidiControl(QString name)
{
	return m_guiMapping.midiControl(name);
}

void TestMidiJ::resetSchemeObjs()
{
	fakeMidiToPC.clearPendingMsg();
}

QScriptValue TestMidiJ::myprint(QScriptContext* sc, QScriptEngine* se)
{
	QString result;
    for (int i = 0; i < sc->argumentCount(); ++i) {
        if (i != 0)
            result.append(QLatin1Char(' '));
        result.append(sc->argument(i).toString());
    }
    qDebug(result.toAscii());
    return QScriptValue(se, 0);
}


void TestMidiJ::testPressButtonDeck()
{
	resetSchemeObjs();
	GraphicsMidiControlItem* deckControl = findMidiControl("Deck");
	QVERIFY2(deckControl, "invalid control");
	QVERIFY(deckControl->hasOutput());

	// notes:
	// current mode - vinyl
	//   StantonSCS3d.mode_store = { "[Channel1]":"vinyl", "[Channel2]":"vinyl" };   // Set vinyl mode on both decks
	// Ids dos botoes:
	//   StantonSCS3d.buttons = { "fx":0x20, "eq":0x26, "loop":0x22, "trig":0x28, "vinyl":0x24, "deck":0x2A };

	// press event
	QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMousePress);
	m_scene.sendEvent(deckControl, &event);
	QCOMPARE(fakeMidiToPC.pendingSendShortMsgCount(), 0); // no msg is sent to PC
	QCOMPARE((int)deckControl->currentState(), 1); // DeckMode is in state 1 - button red


	// release event
	QGraphicsSceneMouseEvent event2(QEvent::GraphicsSceneMouseRelease);
	m_scene.sendEvent(deckControl, &event2);
	QCOMPARE(fakeMidiToPC.pendingSendShortMsgCount(), 0); // no msg is sent to PC
	QCOMPARE((int)deckControl->currentState(), 3); // DeckMode is in state 3 - button purple

	m_deckB = !m_deckB;

}


void TestMidiJ::testPressButtonPlay()
{
	//simular o botao
	resetSchemeObjs();
//    qDebug() << QString("current Thread ID=%1").arg((ulong)QThread::currentThreadId(),0,16);
	GraphicsMidiControlItem* playControl = findMidiControl("Play");
	QVERIFY(playControl);
	QVERIFY(playControl->hasOutput());

	// press event (DECK 2)
	QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMousePress);
	m_scene.sendEvent(playControl, &event);
	if (m_deckB)
		QVERIFY(fakeMidiToPC.pendingSendShortMsg(MIDI_STATUS_NOTE_ON, 8, 22, 127));
	else
		QVERIFY(fakeMidiToPC.pendingSendShortMsg(MIDI_STATUS_NOTE_ON, 7, 22, 127));


	// release event
	fakeMidiToPC.clearPendingMsg();
	QGraphicsSceneMouseEvent event2(QEvent::GraphicsSceneMouseRelease);
	m_scene.sendEvent(playControl, &event2);
	QVERIFY(fakeMidiToPC.pendingSendShortMsgCount() == 1);
	if (m_deckB)
		QVERIFY(fakeMidiToPC.pendingSendShortMsg(MIDI_STATUS_NOTE_OFF, 8, 22, 0));
	else
		QVERIFY(fakeMidiToPC.pendingSendShortMsg(MIDI_STATUS_NOTE_OFF, 7, 22, 0));
}




void TestMidiJ::testPressButtonCue()
{
	//simular o botao
	resetSchemeObjs();
//    qDebug() << QString("current Thread ID=%1").arg((ulong)QThread::currentThreadId(),0,16);
	GraphicsMidiControlItem* cueControl = findMidiControl("Cue");
	QVERIFY(cueControl);
	QVERIFY(cueControl->hasOutput());

	// press event (DECK 2)
	QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMousePress);
	m_scene.sendEvent(cueControl, &event);
	if (m_deckB)
		QVERIFY(fakeMidiToPC.pendingSendShortMsg(MIDI_STATUS_NOTE_ON, 8, 109, 127));
	else
		QVERIFY(fakeMidiToPC.pendingSendShortMsg(MIDI_STATUS_NOTE_ON, 7, 109, 127));


	// release event
	fakeMidiToPC.clearPendingMsg();
	QGraphicsSceneMouseEvent event2(QEvent::GraphicsSceneMouseRelease);
	m_scene.sendEvent(cueControl, &event2);
	QVERIFY(fakeMidiToPC.pendingSendShortMsgCount() == 0);
}


// LOOP tests

void TestMidiJ::testPressButtonLoop()
{
	resetSchemeObjs();
	GraphicsMidiControlItem* loopControl = findMidiControl("Loop");
	QVERIFY2(loopControl, "invalid control");
	QVERIFY(loopControl->hasOutput());
	QVERIFY(!findMidiControl("SurfaceButtonB1")->isVisible()); // This button should not be visible at this time

	// notes:
	// current mode - vinyl
	//   StantonSCS3d.mode_store = { "[Channel1]":"vinyl", "[Channel2]":"vinyl" };   // Set vinyl mode on both decks
	// Ids dos botoes:
	//   StantonSCS3d.buttons = { "fx":0x20, "eq":0x26, "loop":0x22, "trig":0x28, "vinyl":0x24, "deck":0x2A };

	// press event
	QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMousePress);
	m_scene.sendEvent(loopControl, &event);
	QCOMPARE(fakeMidiToPC.pendingSendShortMsgCount(), 0); // no msg is sent to PC
	QCOMPARE((int)loopControl->currentState(), 3); // LoopMode is in state 3 - button purple


	// release event
	QGraphicsSceneMouseEvent event2(QEvent::GraphicsSceneMouseRelease);
	m_scene.sendEvent(loopControl, &event2);
	QCOMPARE(fakeMidiToPC.pendingSendShortMsgCount(), 0); // no msg is sent to PC
	QCOMPARE((int)loopControl->currentState(), 1); // LoopMode is in state 1 - button red
	QVERIFY(findMidiControl("SurfaceButtonB1")->isVisible()); // This button should be visible
//	QVERIFY(!findMidiControl("C1")->isVisible()); // This button should not be visible

	// +notes:
	// SysExMsg - muda o tipo de botoes q estao na superficie:
	//   StantonSCS3d.surface = { "C1":0x00, "S5":0x01, "S3":0x02, "S3+S5":0x03, "Buttons":0x04 };
	//   StantonSCS3d.sysex = [0xF0, 0x00, 0x01, 0x60];  // Preamble for all SysEx messages for this device
    // ??   StantonSCS3d.modeSurface = { "fx":"S3+S5", "eq":"S3+S5", "loop":"Buttons", "loop2":"Buttons", "loop3":"Buttons", "trig":"Buttons", "trig2":"Buttons", "trig3":"Buttons", "vinyl":"C1", "vinyl2":"C1", "vinyl3":"C1"};
}

void TestMidiJ::testPressButtonTrig()
{
	resetSchemeObjs();
	GraphicsMidiControlItem* trigControl = findMidiControl("Trig");
	QVERIFY2(trigControl, "invalid control");
	QVERIFY(trigControl->hasOutput());
	QVERIFY(!findMidiControl("SurfaceButtonB1")->isVisible()); // This button should not be visible at this time

	// notes:
	// current mode - vinyl
	//   StantonSCS3d.mode_store = { "[Channel1]":"vinyl", "[Channel2]":"vinyl" };   // Set vinyl mode on both decks
	// Ids dos botoes:
	//   StantonSCS3d.buttons = { "fx":0x20, "eq":0x26, "loop":0x22, "trig":0x28, "vinyl":0x24, "deck":0x2A };

	// press event
	QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMousePress);
	m_scene.sendEvent(trigControl, &event);
	QTest::qWait(250);
	QCOMPARE(fakeMidiToPC.pendingSendShortMsgCount(), 0); // no msg is sent to PC
	QCOMPARE((int)trigControl->currentState(), 3); // TrigMode is in state 3 - button purple


	// release event
	QGraphicsSceneMouseEvent event2(QEvent::GraphicsSceneMouseRelease);
	m_scene.sendEvent(trigControl, &event2);
	QCOMPARE(fakeMidiToPC.pendingSendShortMsgCount(), 0); // no msg is sent to PC
	QCOMPARE((int)trigControl->currentState(), 1); // LoopMode is in state 1 - button red
	QVERIFY(findMidiControl("SurfaceButtonB1")->isVisible()); // This button should be visible
//	QVERIFY(!findMidiControl("C1")->isVisible()); // This button should not be visible

}

void TestMidiJ::testPressButtonVinyl()
{
	//simular o botao
	resetSchemeObjs();
	GraphicsMidiControlItem* vinylControl = findMidiControl("Vinyl");
	QVERIFY2(vinylControl, "invalid control");
	QVERIFY(vinylControl->hasOutput());

	QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMousePress);
	vinylControl->scene()->sendEvent(vinylControl, &event);
	QCOMPARE(fakeMidiToPC.pendingSendShortMsgCount(), 0); // no msg is sent to PC
	QCOMPARE((int)vinylControl->currentState(), 3); // VinylMode is in state 3 - button purple

	// release event
	fakeMidiToPC.clearPendingMsg();
	QGraphicsSceneMouseEvent event2(QEvent::GraphicsSceneMouseRelease);
	vinylControl->scene()->sendEvent(vinylControl, &event2);
	QCOMPARE((int)vinylControl->currentState(), 3); // LoopMode is in state 3 - button purple


//	QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMousePress);
	vinylControl->scene()->sendEvent(vinylControl, &event);
	QCOMPARE(fakeMidiToPC.pendingSendShortMsgCount(), 0); // no msg is sent to PC
	QCOMPARE((int)vinylControl->currentState(), 3); // VinylMode is in state 3 - button purple

	// release event
	fakeMidiToPC.clearPendingMsg();
//	QGraphicsSceneMouseEvent event2(QEvent::GraphicsSceneMouseRelease);
	vinylControl->scene()->sendEvent(vinylControl, &event2);
	QCOMPARE((int)vinylControl->currentState(), 0); // LoopMode is in state 0 - button black
}

void TestMidiJ::testPressVinylSliderS4()
{
	resetSchemeObjs();
	GraphicsMidiControlItem* vinylControl = findMidiControl("Vinyl");
	QVERIFY2(vinylControl, "invalid control");
	QVERIFY2((int)vinylControl->currentState() != 2, "invalid vinyl mode"); // VinylMode is not in state 2 - button blue

	GraphicsMidiControlItem* vss4Control = findMidiControl("VinylSliderS4");
	QVERIFY2(vss4Control, "invalid control");
	QVERIFY(vss4Control->hasOutput());
	QVERIFY(vss4Control->isVisible());

	// press event
	QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMousePress);
//	event.setPos(sss4Control->pixmap().rect().center());
	event.setPos(QPointF(0,0));
	vss4Control->scene()->sendEvent(vss4Control, &event);
	QCOMPARE(fakeMidiToPC.pendingSendShortMsgCount(), 1);

/*	if (m_deckB)
		QVERIFY(fakeMidiToPC.pendingSendShortMsg(MIDI_STATUS_NOTE_ON, 8, 0x28, 0x7f));
	else
		QVERIFY(fakeMidiToPC.pendingSendShortMsg(MIDI_STATUS_NOTE_ON, 7, 0x28, 0x7f));*/

	// release event
	fakeMidiToPC.clearPendingMsg();
	QGraphicsSceneMouseEvent event2(QEvent::GraphicsSceneMouseRelease);
	vss4Control->scene()->sendEvent(vss4Control, &event2);
//	QCOMPARE(fakeMidiToPC.pendingSendShortMsgCount(), 0); // no msg is sent to PC

}



void TestMidiJ::testPressSurfaceButtonB1()
{
	resetSchemeObjs();
	GraphicsMidiControlItem* loopControl = findMidiControl("Loop");
	QVERIFY2(loopControl, "invalid control");
	QCOMPARE((int)loopControl->currentState(), 1); // LoopMode is in state 1 - button red (Auto mode)

	GraphicsMidiControlItem* sbb1Control = findMidiControl("SurfaceButtonB1");
	QVERIFY2(sbb1Control, "invalid control");
	QVERIFY(sbb1Control->hasOutput());
	QVERIFY2(sbb1Control->isVisible(), "button is not visible");

	// notes:
	// Ids dos botoes:

	// press event
	QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMousePress);
	m_scene.sendEvent(sbb1Control, &event);
	if (m_deckB)
		QVERIFY(fakeMidiToPC.pendingSendShortMsg(MIDI_STATUS_NOTE_ON, 8, 0x24, 0x7f));
	else
		QVERIFY(fakeMidiToPC.pendingSendShortMsg(MIDI_STATUS_NOTE_ON, 7, 0x24, 0x7f));
//	QCOMPARE((int)sbb1Control->currentState(), 1); // DeckMode is in state 1 - button red


	// release event
	fakeMidiToPC.clearPendingMsg();
	QGraphicsSceneMouseEvent event2(QEvent::GraphicsSceneMouseRelease);
	m_scene.sendEvent(sbb1Control, &event2);
//	QVERIFY(fakeMidiToPC.pendingSendShortMsg(MIDI_STATUS_NOTE_OFF, 7, 0xff, 0x0));
//	QCOMPARE((int)sbb1Control->currentState(), 3); // DeckMode is in state 3 - button purple
	QCOMPARE(fakeMidiToPC.pendingSendShortMsgCount(), 1);

}


void TestMidiJ::testPressSurfaceButtonB5()
{
	resetSchemeObjs();
	GraphicsMidiControlItem* loopControl = findMidiControl("Loop");
	QVERIFY2(loopControl, "invalid control");
	QCOMPARE((int)loopControl->currentState(), 1); // LoopMode is in state 1 - button red

	GraphicsMidiControlItem* sbb5Control = findMidiControl("SurfaceButtonB5");
	QVERIFY2(sbb5Control, "invalid control");
	QVERIFY(sbb5Control->hasOutput());
	QVERIFY(sbb5Control->isVisible());

	// press event
	QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMousePress);
	m_scene.sendEvent(sbb5Control, &event);
	QCOMPARE(fakeMidiToPC.pendingSendShortMsgCount(), 1);
/*	if (m_deckB)
		QVERIFY(fakeMidiToPC.pendingSendShortMsg(MIDI_STATUS_NOTE_ON, 8, 0x28, 0x7f));
	else
		QVERIFY(fakeMidiToPC.pendingSendShortMsg(MIDI_STATUS_NOTE_ON, 7, 0x28, 0x7f));*/

	// release event
	fakeMidiToPC.clearPendingMsg();
	QGraphicsSceneMouseEvent event2(QEvent::GraphicsSceneMouseRelease);
	m_scene.sendEvent(sbb5Control, &event2);
	QCOMPARE(fakeMidiToPC.pendingSendShortMsgCount(), 0); // no msg is sent to PC

}

void TestMidiJ::testPressButtonGain()
{
	//simular o botao
	resetSchemeObjs();
//    qDebug() << QString("current Thread ID=%1").arg((ulong)QThread::currentThreadId(),0,16);
	GraphicsMidiControlItem* gainControl = findMidiControl("S1Gain");
	QVERIFY(gainControl);
	QVERIFY(gainControl->hasOutput());

	// press event (DECK 2)
	QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMousePress);
	event.setPos(gainControl->pixmap().rect().center());
	m_scene.sendEvent(gainControl, &event);
	if (m_deckB)
		QVERIFY(fakeMidiToPC.pendingSendShortMsg(MIDI_STATUS_CC, 8, 2, 64));
	else
		QVERIFY(fakeMidiToPC.pendingSendShortMsg(MIDI_STATUS_CC, 7, 2, 64));


	// release event
	fakeMidiToPC.clearPendingMsg();
	QGraphicsSceneMouseEvent event2(QEvent::GraphicsSceneMouseRelease);
	m_scene.sendEvent(gainControl, &event2);
	QCOMPARE(fakeMidiToPC.pendingSendShortMsgCount(), 0);
}


/*
void TestMidiJ::testPressButtonFx()
{
	resetObjs();
	GraphicsMidiControlItem* fxControl = findMidiControl("Fx");
	QVERIFY2(fxControl, "invalid control");
	QVERIFY(fxControl->hasOutput());

	// press event
	QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMousePress);
	m_scene.sendEvent(fxControl, &event);
	QCOMPARE(fakeMidiToPC.pendingSendShortMsgCount(), 0); // no msg is sent to PC

	// release event
	QGraphicsSceneMouseEvent event2(QEvent::GraphicsSceneMouseRelease);
	m_scene.sendEvent(fxControl, &event2);
	QCOMPARE(fakeMidiToPC.pendingSendShortMsgCount(), 0); // no msg is sent to PC

}
*/

void TestMidiJ::testTouchVinylCircleC1_abs()
{
	// set to "Vinyl3" mode (black)

	testPressButtonVinyl();
	fakeMidiToPC.clearPendingMsg();

	qDebug() << "Press----------------------------------------------";
	GraphicsMidiControlItem* vinylControl = findMidiControl("VinylCircleC1_abs");
	QVERIFY2(vinylControl, "invalid control");
	QVERIFY(vinylControl->hasOutput());

	QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMousePress);
	setMouseEventPos(event, vinylControl, vinylControl->sceneRect().center().rx() , 10);
//	event.setPos(QPointF(vinylControl->boundingRect().center().rx(), 10.0));
	vinylControl->scene()->sendEvent(vinylControl, &event);
	QCOMPARE(fakeMidiToPC.pendingSendShortMsgCount(), 2); // with absolute 1st sends a NoteON the a ControlChange

	qDebug() << "Move----------------------------------------------";
	fakeMidiToPC.clearPendingMsg();
	QGraphicsSceneMouseEvent eventM(QEvent::GraphicsSceneMouseMove);
//	eventM.setPos(QPointF(218, 10));
//	eventM.setScenePos(QPointF(vinylControl->scenePos().rx() + 218, vinylControl->scenePos().ry() + 10));
	setMouseEventPos(eventM, vinylControl, vinylControl->sceneRect().center().rx() + 2 , 10);
	vinylControl->scene()->sendEvent(vinylControl, &eventM);
	QCOMPARE(fakeMidiToPC.pendingSendShortMsgCount(), 1); // ControlChange


	qDebug() << "Release----------------------------------------------";
	// release event
	fakeMidiToPC.clearPendingMsg();
	QGraphicsSceneMouseEvent event2(QEvent::GraphicsSceneMouseRelease);
	setMouseEventPos(event2, vinylControl, vinylControl->sceneRect().center().rx() + 2 , 10);
	vinylControl->scene()->sendEvent(vinylControl, &event2);
	QCOMPARE(fakeMidiToPC.pendingSendShortMsgCount(), 1); // NoteOFF
}
/*
QByteArray& operator<<(QByteArray& ba, int n)
{
	ba.append(n);
	return ba;
}*/

void TestMidiJ::testGwSendSysexIdentityRequest()
{
	QByteArray identReqSysexMsg;
	identReqSysexMsg.append(0xF0); // SysEx
	identReqSysexMsg.append(0x7e); // Non-Realtime
	identReqSysexMsg.append(0x7f); // The SysEx channel.
	identReqSysexMsg.append(0x06); // Sub-ID -- General Information
	identReqSysexMsg.append(0x01); // Sub-ID2 -- Identity Request
	identReqSysexMsg.append(0xF7); // End of SysEx
	fakeMidiToPC.emulSysexMsgReceive(identReqSysexMsg);
	QCOMPARE(fakeMidiToPC.pendingSysexMsgCount(), 1); // Sysex reply

	QByteArray identReplySysexMsg;
	identReplySysexMsg.append(0xF0); // SysEx
	identReplySysexMsg.append(0x7e); // Non-Realtime
	identReplySysexMsg.append(0x7f); // The SysEx channel.
	identReplySysexMsg.append(0x06); // Sub-ID -- General Information
	identReplySysexMsg.append(0x02); // Sub-ID2 -- Identity Reply...
	QVERIFY(fakeMidiToPC.pendingSysexMsg(identReplySysexMsg));
}



QTEST_MAIN(TestMidiJ)
#include "TestMidiJ.moc"

