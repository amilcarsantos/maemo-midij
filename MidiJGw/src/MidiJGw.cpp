/*
 * MidiJGw - MIDI gateway for MidiJ application
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

#include "MidiJGw.h"
#include <QtGui>
#include "MidiObjectUdp.h"
#include "mixxx/MidiObjectWin.h"
#include "ActionId.h"
#include "Utils.h"



void InputMidiController::init(int ctrlId, QPushButton* button)
{
	InputMidiController* pThis = this;
	pThis->ctrlId = ctrlId;
	pThis->button = button;

	pThis->reqSysexIdent = false;
	pThis->midiObject = NULL;
	pThis->eventsCounter = 0;
}



#define OFFLINE_DEVICE	"Offline"

// Base devices IDs
#define MG_GW_DEVICE	0x10000
#define MG_WIN_DEVICE	0x20000
#define MG_UDP_DEVICE	0x30000

// Gateway IDs
#define	MG_GW_DIRECTMIDI		(MG_GW_DEVICE + 0)
#define	MG_GW_DUALDIRECTMIDI	(MG_GW_DEVICE + 1)
#define MG_GW_ENGINE(_idx)		(MG_GW_DEVICE + 2 + _idx)

// Settings
#define SETTINGS_GATEWAYCTRL	"GatewayId"
#define SETTINGS_MIDIOUTPUT		"MidiOut"
#define SETTINGS_MIDICTRL1		"Midi1"
#define SETTINGS_MIDICTRL2		"Midi2"


MidiJGw::MidiJGw(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	if (false)
	{
		// Hide debug controls
		resize(width(), ui.midiController1LineOLD->y());
		_delete(ui.midiController1LineOLD);
		_delete(ui.MIDI_DEBUG);
	}

	// System tray icon
	createTrayActions();
	createTrayIcon();
	QIcon icon(":/midijgw/midijgw_icon.png");
	trayIcon->setIcon(icon);
	trayIcon->show();
	// strip BUILD number
	QString version = QString(" %1").arg(MIDIJGW_VERSION);
	setWindowTitle(windowTitle() + version.left(version.lastIndexOf('.')));

	int id;
	//m_midiCtrl1 = NULL;
	m_inputMidi1.init(0, ui.midiController1);
	//m_midiCtrl2 = NULL;
	m_inputMidi2.init(1, ui.midiController2);

	//id = MG_GW_DEVICE;
	m_gatewayItems.insert(MG_GW_DIRECTMIDI, "Direct MIDI");
	m_gatewayItems.insert(MG_GW_DUALDIRECTMIDI , "Dual Direct MIDI");
	m_gatewayItems.insert(MG_GW_ENGINE(0), "Engine [Sound tester]"); // TODO

	ui.gatewayController->setMenu(createDeviceMenu(m_gatewayItems, SLOT(setGatewayMidi(int))));

	// MIDI Output
	QMap<int, QString> midiOutputsMap;
//	midiOutputs.insert(0, "LoopBe 2");
//	midiOutputs.insert(1, "Microsoft GS Wavetable SW Synth");
	QListIterator<QString> outListIt(m_devices.outputDeviceList());
	id = MG_WIN_DEVICE;
	while (outListIt.hasNext())
		midiOutputsMap.insert(id++, outListIt.next());

	ui.midiOutput->setMenu(createDeviceMenu(midiOutputsMap, SLOT(setMidiOutput(QString)), true, SLOT(midiOutputOffline())));
	m_midiOutput = m_devices.outputDevice();

	// MIDI Controllers
	QMap<int, QString> midiInputsMap;

	// TODO auto update (apos varios clicks ou por timeout)....
	QListIterator<QString> inListIt(m_devices.inputDeviceList());
	id = MG_UDP_DEVICE;
	int outDevId = 0;

	while (inListIt.hasNext())
	{
		QString device = inListIt.next();
		outDevId = midiOutputsMap.key(device, -1);
		if (outDevId > 0)
			midiInputsMap.insert(outDevId, device);
		else
			midiInputsMap.insert(id++, device);
	}

	ui.midiController1->setMenu(createDeviceMenu(midiInputsMap, SLOT(setMidiController1(QString)), true, SLOT(midiController1Offline())));
	ui.midiController2->setMenu(createDeviceMenu(midiInputsMap, SLOT(setMidiController2(QString)), true, SLOT(midiController2Offline())));


	// apply stored settings
	m_currentGatewayCtrlId = 0;
	id = m_settings.value(SETTINGS_MIDIOUTPUT).toString().toInt();
	if (m_gatewayItems.contains(id))
		setGatewayMidi(id);
	else
		setGatewayMidi(MG_GW_DIRECTMIDI);

	QString defaultMidiOutput = m_settings.value(SETTINGS_MIDIOUTPUT).toString();
	qDebug() << "defaultMidiOutput" << defaultMidiOutput;
	if (m_devices.outputDeviceList().contains(defaultMidiOutput))
		setMidiOutput(defaultMidiOutput);
	else
		midiOutputOffline();

	QString defaultMidiCtrl1 = m_settings.value(SETTINGS_MIDICTRL1).toString();
	qDebug() << "defaultMidiCtrl1" << defaultMidiCtrl1;
	if (m_devices.inputDeviceList().contains(defaultMidiCtrl1))
		setMidiController1(defaultMidiCtrl1);
	else
		setMidiController1(OFFLINE_DEVICE);

	if (m_currentGatewayCtrlId == MG_GW_DUALDIRECTMIDI)
	{
		QString defaultMidiCtrl2 = m_settings.value(SETTINGS_MIDICTRL2).toString();
		qDebug() << "defaultMidiCtrl2" << defaultMidiCtrl2;

		// FIXME dev2 nao liga o Port..
		if (m_devices.inputDeviceList().contains(defaultMidiCtrl2))
			setMidiController2(defaultMidiCtrl2);
		else
			setMidiController2(OFFLINE_DEVICE);
	}
	else
		setMidiController2(OFFLINE_DEVICE);

	connect(&m_midiUiTimer, SIGNAL(timeout()), this, SLOT(midiUiEvents()));
//    m_midiCrtl1Events = 0;
//    m_midiCrtl2Events = 0;
	m_midiUiTimer.start(333);
}

MidiJGw::~MidiJGw()
{
}

void MidiJGw::closeEvent(QCloseEvent *event)
{
	event->accept();
	m_settings.setValue(SETTINGS_MIDICTRL1, m_inputMidi1.device);
	m_settings.setValue(SETTINGS_MIDICTRL2, m_inputMidi2.device);
	m_settings.setValue(SETTINGS_MIDIOUTPUT, ui.midiOutput->text());
	m_settings.setValue(SETTINGS_GATEWAYCTRL, m_currentGatewayCtrlId);

	m_devices.clear();
	QWidget::closeEvent(event);
}

QMenu* MidiJGw::createDeviceMenu(const QMap<int, QString>& items, const char* slot, bool isIdString, const char* offlineSlot)
{
	QMenu* menu = new QMenu();
	QMapIterator<int, QString> i(items);
	while (i.hasNext())
	{
		i.next();
		QAction* deviceAction = new ActionId(i.value(), i.key(), this);
//		deviceAction->setIcon(m_devices.deviceIcon(i.value()));
		menu->addAction(deviceAction);
		m_deviceActionMap.insert(deviceActionMapKey(i.key()), deviceAction);
		m_deviceActionMap.insert(i.value(), deviceAction);
		foreach(QString conflictDevice, m_devices.conflictDevices(i.value()))
			m_deviceActionMap.insert(conflictDevice, deviceAction);

		if (isIdString)
			connect(deviceAction, SIGNAL(triggered(QString)), slot);
		else
			connect(deviceAction, SIGNAL(triggered(int)), slot);
	}
	if (offlineSlot != NULL)
	{
		// add offline option
		menu->addSeparator();
		connect(menu->addAction(OFFLINE_DEVICE), SIGNAL(triggered()), offlineSlot);
	}
	return menu;
}


void MidiJGw::switchDeviceActions(int newDeviceId, int oldDeviceId)
{
	QList<QAction*> deviceActions = getDeviceActions(deviceActionMapKey(oldDeviceId));
	foreach(QAction* action, deviceActions)
		action->setEnabled(true);
	deviceActions = getDeviceActions(deviceActionMapKey(newDeviceId));
	foreach(QAction* action, deviceActions)
		action->setEnabled(false);
}

void MidiJGw::switchDeviceActions(QString newDevice, QString oldDevice)
{
	QList<QAction*> deviceActions = getDeviceActions(oldDevice);
	foreach(QAction* action, deviceActions)
		action->setEnabled(true);
	deviceActions = getDeviceActions(newDevice);
	foreach(QAction* action, deviceActions)
		action->setEnabled(false);
}

void MidiJGw::createTrayActions()
{
    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
	connect(restoreAction, SIGNAL(triggered()), this, SLOT(raise()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MidiJGw::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
//    trayIconMenu->addAction(minimizeAction);
//    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}


void MidiJGw::setGatewayMidi(int id)
{
	if (m_currentGatewayCtrlId != id)
	{
		switchDeviceActions(id, m_currentGatewayCtrlId);
		m_currentGatewayCtrlId = id;

		bool midiCtrl2Visible = (id == MG_GW_DUALDIRECTMIDI);
		if (midiCtrl2Visible)
			ui.gatewayController->setIcon(QIcon(":/midijgw/right_doublearrow.png"));
		else
			ui.gatewayController->setIcon(QIcon(":/midijgw/right_arrow.png"));

		ui.midiController2->setVisible(midiCtrl2Visible);
		ui.midiController2Line->setVisible(midiCtrl2Visible);
		if (!midiCtrl2Visible)
			setMidiController2(OFFLINE_DEVICE);

/*		bool midiEngine = (id >= MG_GW_ENGINE(0));
		if (midiEngine)
		{
			outputType = m_schemesHandler->settings(id - MG_GW_ENGINE(0))->outputType();
			if (outputType & MIDI)
			{
				// enable output to MIDI
			}
			else
			{
				// hide output MIDI
			}

			if (outputType & AUDIO)
			{
				// enable output to AUDIO
			}
			else
			{
				// hide AUDIO output
			}
		}


*/
		ui.gatewayController->setText(m_gatewayItems.value(id));
	}
}


void MidiJGw::setMidiOutput(QString device)
{
	if (ui.midiOutput->text() != device)
	{
		switchDeviceActions(device, ui.midiOutput->text());

		ui.midiOutput->setText(device);
		ui.midiOutput->setIcon(m_devices.deviceIcon(device));

		if (!m_midiOutput->getOpenDevice().isEmpty())
			m_midiOutput->devClose();

		if (device != OFFLINE_DEVICE)
		{
			m_midiOutput->devOpen(device);
			if (m_midiOutput->getOpenDevice().isEmpty())
			{
				// TODO gestao de erro
				qCritical() << "fail to open output device" << device;
				ui.midiOutput->setText("Error");
				ui.midiOutput->setIcon(QMessageBox::standardIcon(QMessageBox::Critical));
			}
		}
	}
}

void MidiJGw::midiOutputOffline()
{
	setMidiOutput(OFFLINE_DEVICE);
}

void MidiJGw::manageInputMidiController(InputMidiController* imc, const QString& device, const char* slotMidiReceive)
{
	if (imc->device != device)
	{
		switchDeviceActions(device, imc->device);

		imc->device = device;
		imc->button->setText(device);
		imc->button->setIcon(m_devices.deviceIcon(device));
		imc->reqSysexIdent = m_devices.suportsSysexIdentMsg(device);

		if (device == OFFLINE_DEVICE)
		{
			if (imc->midiObject != NULL && !imc->midiObject->getOpenDevice().isEmpty())
				imc->midiObject->devClose();
			return;
		}

		MidiObject* midiDevice = m_devices.inputDevice(imc->ctrlId, device);
		if (midiDevice != imc->midiObject)
		{
			if (imc->midiObject)
				disconnect(imc->midiObject, 0, this, 0);
			connect(midiDevice, SIGNAL(midiThru(MidiMessage,char)), slotMidiReceive);
		}
		if (imc->midiObject)
			imc->midiObject->devClose();
		imc->midiObject = midiDevice;
		imc->midiObject->devOpen(device);
		if (imc->midiObject->getOpenDevice().isEmpty())
		{
			// FIXME gestao de erro
			qCritical() << "fail to open output device" << device;
			ui.errorLabel->setText(QString("Fail to open device: %1").arg(device));
			ui.errorLabel->setStyleSheet("background-color: rgb(192, 0, 0);color: rgb(255, 255, 0);");
			imc->button->setText("Error");
			imc->button->setIcon(QMessageBox::standardIcon(QMessageBox::Critical));

		}
	}
}


void MidiJGw::setMidiController1(QString device)
{
/*	if (ui.midiController1->text() != device)
	{
		switchDeviceActions(device, ui.midiController1->text());

		ui.midiController1->setText(device);
		ui.midiController1->setIcon(devices.deviceIcon(device));
		if (device == OFFLINE_DEVICE)
		{
			if (m_midiCtrl1 != NULL && !m_midiCtrl1->getOpenDevice().isEmpty())
				m_midiCtrl1->devClose();
			return;
		}

		MidiObject* midiDevice = devices.inputDevice(0, device);
		if (midiDevice != m_midiCtrl1)
		{
			if (m_midiCtrl1)
				disconnect(m_midiCtrl1, 0, this, 0);
			connect(midiDevice, SIGNAL(midiThru(MidiMessage,char)), SLOT(midiReceiveCtrl1(MidiMessage,char)));
		}
		if (m_midiCtrl1)
			m_midiCtrl1->devClose();
		m_midiCtrl1 = midiDevice;
		m_midiCtrl1->devOpen(device);
	}*/
	manageInputMidiController(&m_inputMidi1, device, SLOT(midiReceiveCtrl1(MidiMessage,char)));
}

void MidiJGw::midiController1Offline()
{
	//ui.midiController1->setText(MENU_OFFLINE);
	setMidiController1(OFFLINE_DEVICE);
}

void MidiJGw::setMidiController2(QString device)
{
/*	if (ui.midiController2->text() != device)
	{
		switchDeviceActions(device, ui.midiController2->text());

		ui.midiController2->setText(device);
		ui.midiController2->setIcon(devices.deviceIcon(device));

		if (device == OFFLINE_DEVICE)
		{
			if (m_midiCtrl2 != NULL && !m_midiCtrl2->getOpenDevice().isEmpty())
				m_midiCtrl2->devClose();
			return;
		}

		MidiObject* midiDevice = devices.inputDevice(1, device);
		if (midiDevice != m_midiCtrl2)
		{
			if (m_midiCtrl2)
				disconnect(m_midiCtrl2, 0, this, 0);
			connect(midiDevice, SIGNAL(midiThru(MidiMessage,char)), SLOT(midiReceiveCtrl2(MidiMessage,char)));
		}
		if (m_midiCtrl2)
			m_midiCtrl2->devClose();
		m_midiCtrl2 = midiDevice;
		m_midiCtrl2->devOpen(device);

	}*/
	manageInputMidiController(&m_inputMidi2, device, SLOT(midiReceiveCtrl2(MidiMessage,char)));
}

void MidiJGw::midiController2Offline()
{
	setMidiController2(OFFLINE_DEVICE);
}

void MidiJGw::instanceMessage(const QString& s)
{
	showNormal();
}


void MidiJGw::midiReceiveCtrl1(MidiMessage message, char value)
{
//	if (m_midiCrtl1Events == 0)
	if (m_inputMidi1.eventsCounter == 0)
	{
		ui.midiController1Line->lightOn(Qt::green);
		if (m_inputMidi1.reqSysexIdent)
		{
			// TODO enviar SysexMsg IdentReqst
			m_inputMidi1.reqSysexIdent = false;
			qDebug() << "invokar SysexIdent no CTRL1";
		}
	}
	m_inputMidi1.eventsCounter++;

	gatewaySendShortMsg(message, value);
}

void MidiJGw::midiReceiveCtrl2(MidiMessage message, char value)
{
	if (m_inputMidi2.eventsCounter == 0)
	{
		ui.midiController2Line->lightOn(Qt::green);
		if (m_inputMidi2.reqSysexIdent)
		{
			// TODO enviar SysexMsg IdentReqst
			m_inputMidi2.reqSysexIdent = false;
			qDebug() << "invokar SysexIdent no CTRL2";
		}
	}
	m_inputMidi2.eventsCounter++;

	gatewaySendShortMsg(message, value);
}

void MidiJGw::gatewaySendShortMsg(const MidiMessage& message, char value)
{
	static int count = 0;
	count++;

	QString msg = QString("(%1) 0x%2 0x%3 0x%4")
			.arg(count)
			.arg(message.getMidiStatusByte(), 2, 16)
			.arg(message.getMidiNo(), 2, 16)
			.arg((ushort)value, 2, 16);
	qDebug() << msg;

	if (ui.MIDI_DEBUG)
		ui.MIDI_DEBUG->setText(msg);

	if (m_midiOutput)
	{
		if (m_midiOutputEvents == 0)
			ui.gatewayControllerLine->lightOn(Qt::darkGreen);
		m_midiOutputEvents++;

		m_midiOutput->sendShortMsg(message.getMidiStatusByte(), message.getMidiNo(), value);
	}
}

void MidiJGw::midiUiEvents()
{
    if (m_inputMidi1.eventsCounter > 0)
    {
		ui.midiController1Line->lightOff();
		m_inputMidi1.eventsCounter = 0;
    }
    if (m_inputMidi2.eventsCounter > 0)
    {
    	ui.midiController2Line->lightOff();
    	m_inputMidi2.eventsCounter = 0;
    }
	if (m_midiOutputEvents > 0)
	{
		ui.gatewayControllerLine->lightOff();
		m_midiOutputEvents = 0;
	}

}



