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

#ifndef SETTINGSWND_H_
#define SETTINGSWND_H_

#include "../gui/MidiMappingGui.h"

class QMaemo5ListPickSelector;
class QMaemo5ValueButton;

class SettingsWnd
{
public:
	SettingsWnd();
	virtual ~SettingsWnd();

protected:
	virtual QMaemo5ValueButton* buttonFromName(QString name) = 0;
public:
	void initScheme(QIcon icon, const QList<SchemeGuiHeader*>& schemeList, const QString& currentScheme);
	QString guiScheme();
	void initMidiConnection(const QStringList& connections, const QString& currentConnection);
	QString midiConnection();
	void initMidiConnHost(const QStringList& ipList, const QString& currentIp);
	QString midiConnHost();
	void initNetcast(const QStringList& connections, const QString& currentConnection);
	QString netcastConnection();
	void initNetcastHost(const QStringList& ipList, const QString& currentIp);
	QString netcastHost();
	void initNetcastBuffer(const QStringList& connections, const QString& currentConnection);
	QString netcastBuffer();


	void setSwapSettings(bool flag);
	bool swapSettings();

private:
	QMaemo5ListPickSelector* m_schemeSelector;
	bool m_swapSettings;
};

#endif /* SETTINGSDIALOG_H_ */
