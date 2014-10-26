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

#ifndef MIDIDEVICEHANDLER_H_
#define MIDIDEVICEHANDLER_H_

#include "mixxx/MidiObject.h"
#include <QIcon>

class MidiDeviceHandler
{
public:
	MidiDeviceHandler();
	virtual ~MidiDeviceHandler();

public:
	MidiObject* outputDevice();
	QStringList outputDeviceList();

	MidiObject* inputDevice(int ctrlId, QString deviceName);
	QStringList inputDeviceList();
	QStringList conflictDevices(QString deviceName);

	QIcon deviceIcon(QString deviceName);
	bool suportsSysexIdentMsg(QString deviceName);

	void clear(bool callInit = true);

private:
	void init();
	void createInputDeviceList(int type, const QStringList& deviceList);
	MidiObject* m_outputObject;
	QStringList m_outputDeviceList;

	QHash<QString, int> m_inputDevice2Type;
	QStringList         m_inputDeviceList;
	QHash<int, MidiObject*> m_inputObjectMap;
	QMultiHash<QString, QString> m_conflictDevices;
};

#endif /* MIDIDEVICEHANDLER_H_ */
