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

#ifndef SETTINGSMANAGER_H_
#define SETTINGSMANAGER_H_

#include <QWidget>
#include "../gui/MidiMappingGui.h"
#include "../MidiObjectUdp.h"
#include "../NetcastManager.h"

#define SETTINGS_CURRENTSCHEMEID	"scheme"
#define SETTINGS_MIDICONNECTION		"connection"
#define SETTINGS_MIDICONN_IPADDR	"connection.ipaddress"
#define SETTINGS_NETCASTCONNECTION	"netcast"
#define SETTINGS_NETCAST_BUFFER		"netcast.udpbuffer"
#define SETTINGS_NETCAST_IPADDR		"netcast.ipaddress"

#define BROADCAST					"Broadcast"
#define USE_MIDI_CONN				"Use MIDI Host"

#define MIDIJ_FILESCHEMEFILTER		"*.xml"

class SettingsWnd;

class SettingsManager
{
public:
	SettingsManager();
	virtual ~SettingsManager();

public:
	// load from QSettings/default values
	void init(QString schemeHomeDir, QString schemeAltDir, QIcon defaultSchemeIcon48, MidiObjectUdp* midiConnection, NetcastManager* netcastManager);
	void close();

	// opens a dialog box for the user to change the settings
	void execDialog(QWidget* parent);
	int result();

	QString guiScheme();
	QString midiConnection();
	MidiObjectUdp::Params* midiConnParams();
	QString netcastConnection();
	NetcastManager::Params* netcastParams();

	bool isGuiSchemeChanged();
	bool isMidiConnectionChanged();
	bool isNetcastConnectionChanged();

protected:
	QIcon                   m_defaultSchemeIcon;
	QHash<QString, QString> m_schemeSources;
	QList<SchemeGuiHeader*> m_schemeList;
	bool                    m_guiSchemeChanged;
	QString                 m_currentSchemeId;

	QStringList             m_midiConnList;
	bool                    m_midiConnChanged;
	QString                 m_currentMidiConn;
	QString                 m_currentMidiConnIp;
	QStringList             m_midiConnHostList;
	MidiObjectUdp::Params   m_currentMidiConnParams;

	QStringList             m_netcastConnList;
	bool                    m_netcastConnChanged;
	QString                 m_currentNetcastConn;
	QStringList             m_netcastHostList;
	QString                 m_currentNetcastIp;
	QStringList             m_netcastBufferList;
	NetcastManager::Params  m_currentNetcastParams;

	bool                    m_advancedSettings;
	int                     m_result;

	void initSettingsWnd(SettingsWnd* wnd);
	void acceptSettingsWnd(SettingsWnd* wnd);

public:
	static QList<QString> allBroadcastAddresses();
	static QString validateIp(const QString& ip);
};


inline bool SettingsManager::isGuiSchemeChanged()
{
	return m_guiSchemeChanged;
}

inline QString SettingsManager::guiScheme()
{
	return m_currentSchemeId;
}

inline bool SettingsManager::isMidiConnectionChanged()
{
	return m_midiConnChanged;
}

inline QString SettingsManager::midiConnection()
{
	return m_currentMidiConn;
}

inline MidiObjectUdp::Params* SettingsManager::midiConnParams()
{
	return &m_currentMidiConnParams;
}

inline bool SettingsManager::isNetcastConnectionChanged()
{
	return m_netcastConnChanged;
}

inline QString SettingsManager::netcastConnection()
{
	return m_currentNetcastConn;
}

inline NetcastManager::Params* SettingsManager::netcastParams()
{
	return &m_currentNetcastParams;
}

#endif /* SETTINGSMANAGER_H_ */
