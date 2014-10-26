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

#include "SettingsManager.h"
#include "SettingsDialog.h"
#include "AdvancedSettingsWnd.h"
#include "../Utils.h"


SettingsManager::SettingsManager()
{
	m_advancedSettings = false;
}

SettingsManager::~SettingsManager()
{
}

QString SettingsManager::validateIp(const QString& ip)
{
	if (ip == BROADCAST)
		return QString();
	return ip;
}

void SettingsManager::init(QString schemeHomeDir, QString schemeAltDir, QIcon defaultSchemeIcon48,
		MidiObjectUdp* midiConnection, NetcastManager* netcastManager)
{
#ifdef Q_WS_MAEMO_5
	QSettings settings;
	m_currentSchemeId = settings.value(SETTINGS_CURRENTSCHEMEID).toString();
	if (m_currentSchemeId.isEmpty())
	{
		m_currentSchemeId = QFileInfo(schemeHomeDir + "midij_Blue_Gen.xml").absoluteFilePath();
	}
	m_currentMidiConn = settings.value(SETTINGS_MIDICONNECTION, "UDP/IP [5004/5]").toString();
	m_currentMidiConnIp = settings.value(SETTINGS_MIDICONN_IPADDR, "").toString();
	m_currentMidiConnParams.targetIp =  validateIp(m_currentMidiConnIp);
	m_currentNetcastConn = settings.value(SETTINGS_NETCASTCONNECTION, "Disabled").toString();
	m_currentNetcastParams.udpBufferSize = settings.value(SETTINGS_NETCAST_BUFFER, "0").toString().toInt();
	m_currentNetcastIp = settings.value(SETTINGS_NETCAST_IPADDR, "").toString();
	if (m_currentNetcastIp == USE_MIDI_CONN)
	{
		m_currentNetcastParams.ipAddress = m_currentMidiConnParams.targetIp;
	}
	else
	{
		m_currentNetcastParams.ipAddress = validateIp(m_currentNetcastIp);
	}
#else
	m_currentSchemeId = QFileInfo(schemeHomeDir + "midij_Stanton_Gen.xml").absoluteFilePath();
	m_currentMidiConn = "UDP/IP [5004/5]";
	m_currentNetcastConn = "Disabled";
	m_currentNetcastParams.udpBufferSize = 0;
#endif

	m_defaultSchemeIcon = defaultSchemeIcon48;
	m_midiConnList << *midiConnection->getDeviceList();
	m_midiConnHostList.append(BROADCAST);
	m_midiConnHostList.append(allBroadcastAddresses());

	m_netcastConnList = netcastManager->connectionList();
	m_netcastBufferList.append("0");
	m_netcastBufferList.append("64");
	m_netcastBufferList.append("128");
	m_netcastBufferList.append("512");
	m_netcastBufferList.append("1024");
	m_netcastBufferList.append("2048");
	m_netcastHostList.append(USE_MIDI_CONN);
	m_netcastHostList.append(BROADCAST);
	m_netcastHostList.append(allBroadcastAddresses());

	// find scheme header files
	QStringList fileExtList;
	fileExtList.append(MIDIJ_FILESCHEMEFILTER);

	DirContext dc1(schemeHomeDir);
	QDirIterator xmlHeadersIt(QDir::currentPath(), fileExtList, QDir::Files);
	while (xmlHeadersIt.hasNext())
	{
		xmlHeadersIt.next();
		m_schemeList.append(new SchemeGuiHeader(xmlHeadersIt.fileName()));
	}
	dc1.clear();
	if (QFileInfo(schemeAltDir).isDir())
	{
		DirContext dc2(schemeAltDir);
		QDirIterator xmlHeadersIt(QDir::currentPath(), fileExtList, QDir::Files);
		while (xmlHeadersIt.hasNext())
		{
			xmlHeadersIt.next();
			m_schemeList.append(new SchemeGuiHeader(xmlHeadersIt.fileName()));
		}
	}
	// remove invalid entries
	QMutableListIterator<SchemeGuiHeader*> schemesIt(m_schemeList);
	while (schemesIt.hasNext())
	{
		SchemeGuiHeader* guiHeader = schemesIt.next();
		if (guiHeader == NULL || !guiHeader->isValid())
		{
			schemesIt.remove();
			_delete(guiHeader);
		}
	}
}

void SettingsManager::close()
{
#ifdef Q_WS_MAEMO_5
	QSettings settings;
	settings.setValue(SETTINGS_CURRENTSCHEMEID, m_currentSchemeId);
	settings.setValue(SETTINGS_MIDICONNECTION, m_currentMidiConn);
	settings.setValue(SETTINGS_MIDICONN_IPADDR, m_currentMidiConnIp);
	settings.setValue(SETTINGS_NETCASTCONNECTION, m_currentNetcastConn);
	settings.setValue(SETTINGS_NETCAST_BUFFER, m_currentNetcastParams.udpBufferSize);
	settings.setValue(SETTINGS_NETCAST_IPADDR, m_currentNetcastIp);
#endif
}

void SettingsManager::execDialog(QWidget* parent)
{
	m_result = QDialog::Rejected;
	while (true)
	{
		if (m_advancedSettings)
		{
			AdvancedSettingsWnd dlg(parent);
			initSettingsWnd(&dlg);
			dlg.exec();
			if (dlg.result() == QDialog::Accepted)
			{
				acceptSettingsWnd(&dlg);
				return;
			}
			else if (!dlg.swapSettings())
			{
				// cancel settings
				return;
			}
		}
		else
		{
			SettingsDialog dlg;
			initSettingsWnd(&dlg);
			dlg.exec();
			if (dlg.result() == QDialog::Accepted)
			{
				acceptSettingsWnd(&dlg);
				return;
			}
			else if (!dlg.swapSettings())
			{
				// cancel settings
				return;
			}
		}
		// swap settings window
		m_advancedSettings = !m_advancedSettings;
	}
}

void SettingsManager::initSettingsWnd(SettingsWnd* wnd)
{
	wnd->initScheme(m_defaultSchemeIcon, m_schemeList, m_currentSchemeId);
	wnd->initMidiConnection(m_midiConnList, m_currentMidiConn);
	wnd->initMidiConnHost(m_midiConnHostList, m_currentMidiConnIp);
	wnd->initNetcast(m_netcastConnList, m_currentNetcastConn);
	wnd->initNetcastBuffer(m_netcastBufferList, QString("%1").arg(m_currentNetcastParams.udpBufferSize));
	wnd->initNetcastHost(m_netcastHostList, m_currentNetcastIp);
}

void DDX_String(QString source, QString& target, bool& flag)
{
	if (!source.isEmpty() && target != source)
	{
		target = source;
		flag = true;
	}
}

void DDX_String(QString source, QString& target, bool& flag, QString defaultTarget)
{
	if (!source.isEmpty() && target != source)
	{
		target = source;
		flag = true;
	}
	else if (target.isEmpty())
	{
		target = defaultTarget;
	}
}

void SettingsManager::acceptSettingsWnd(SettingsWnd* wnd)
{
	m_result = QDialog::Accepted;
	QString netcastBuffer = QString("%1").arg(m_currentNetcastParams.udpBufferSize);
	bool midiConnHostChanged = false;
	m_guiSchemeChanged = false;
	m_midiConnChanged = false;
	m_netcastConnChanged = false;

	DDX_String(wnd->guiScheme(), m_currentSchemeId, m_guiSchemeChanged);
	DDX_String(wnd->midiConnection(), m_currentMidiConn, m_midiConnChanged);
	DDX_String(wnd->midiConnHost(), m_currentMidiConnIp, midiConnHostChanged);
	DDX_String(wnd->netcastConnection(), m_currentNetcastConn, m_netcastConnChanged);
	DDX_String(wnd->netcastBuffer(), netcastBuffer, m_netcastConnChanged, "0");
	DDX_String(wnd->netcastHost(), m_currentNetcastIp, m_netcastConnChanged);
	if (midiConnHostChanged)
	{
		m_currentMidiConnParams.targetIp = validateIp(m_currentMidiConnIp);
		m_midiConnChanged = true;
	}
	if (m_currentNetcastIp == USE_MIDI_CONN)
	{
		m_currentNetcastParams.ipAddress = m_currentMidiConnParams.targetIp;
		m_netcastConnChanged |= midiConnHostChanged; // change also the netcast
	}
	else
	{
		m_currentNetcastParams.ipAddress = validateIp(m_currentNetcastIp);
	}
	m_currentNetcastParams.udpBufferSize = netcastBuffer.toInt();
}

int SettingsManager::result()
{
	return m_result;
}

QList<QString> SettingsManager::allBroadcastAddresses()
{
	QList<QString> addressList;
	QList<QNetworkInterface> hostInterfaces = QNetworkInterface::allInterfaces();
	foreach(QNetworkInterface interface, hostInterfaces)
	{
		foreach(QNetworkAddressEntry entry, interface.addressEntries())
		{
			QString address = entry.broadcast().toString().trimmed();
			if (!address.isEmpty())
				addressList.append(address);
		}
	}

	return addressList;
}

