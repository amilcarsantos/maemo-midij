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

#ifndef MIDINETJ_H
#define MIDINETJ_H

#include <QtGui/QWidget>
#include "ui_MidiJGw.h"
#include <QSystemTrayIcon>

#include "MidiDeviceHandler.h"

struct InputMidiController
{
	QString device;
	int ctrlId;
	QPushButton* button;
	bool reqSysexIdent;
	MidiObject* midiObject;
	int eventsCounter;

	void init(int ctrlId, QPushButton* button);
};

class MidiJGw : public QWidget
{
	Q_OBJECT

public:
	MidiJGw(QWidget *parent = 0);
	virtual ~MidiJGw();

	// System tray icon
	void createTrayActions();
	void createTrayIcon();
	virtual void closeEvent(QCloseEvent *);

	QMenu* createDeviceMenu(const QMap<int, QString>& items, const char* slot, bool isIdString = false, const char* offlineSlot = NULL);
	QList<QAction*> getDeviceActions(int id);
	QList<QAction*> getDeviceActions(const QString& stringId);
	void switchDeviceActions(int newDeviceId, int oldDeviceId);
	void switchDeviceActions(QString newDevice, QString oldDevice);

private slots:
	void instanceMessage(const QString& s);
	void midiReceiveCtrl1(MidiMessage message, char value);
	void midiReceiveCtrl2(MidiMessage message, char value);

	void setGatewayMidi(int id);
	void setMidiOutput(QString device);
	void midiOutputOffline();
	void setMidiController1(QString device);
	void midiController1Offline();
	void setMidiController2(QString device);
	void midiController2Offline();
	void midiUiEvents();


private:
	void manageInputMidiController(InputMidiController* imc, const QString& device, const char* slotMidiReceive);
	void gatewaySendShortMsg(const MidiMessage& message, char value);
	static QString deviceActionMapKey(int key);

	Ui::midiJGwClass ui;

	QSettings m_settings;
	MidiDeviceHandler m_devices;
	int m_currentGatewayCtrlId;

	QAction* minimizeAction;
	QAction* maximizeAction;
	QAction* restoreAction;
	QAction* quitAction;

	QSystemTrayIcon* trayIcon;
	QMenu* trayIconMenu;

//    MidiObject* m_midiCtrl1;
	InputMidiController m_inputMidi1;
//    MidiObject* m_midiCtrl2;
	InputMidiController m_inputMidi2;
	MidiObject* m_midiOutput;
	QTimer      m_midiUiTimer;
	int         m_midiOutputEvents;

	QMap<int, QString> m_gatewayItems;

	QMultiHash<QString, QAction*> m_deviceActionMap;
};

inline QString MidiJGw::deviceActionMapKey(int key)
{
	return QString("deviceId=%1").arg(key);
}

inline QList<QAction*> MidiJGw::getDeviceActions(int id)
{
	return m_deviceActionMap.values(deviceActionMapKey(id));
}
inline QList<QAction*> MidiJGw::getDeviceActions(const QString& stringId)
{
	return m_deviceActionMap.values(stringId);
}


#endif // MIDINETJ_H
