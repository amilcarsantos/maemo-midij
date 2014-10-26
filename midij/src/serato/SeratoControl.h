/*
 * MidiJ - Midi controller for DJs
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

#ifndef SERATOCONTROL_H_
#define SERATOCONTROL_H_

#include <qdom.h>
#include "../mixxx/MixxxControl.h"

class SeratoControl
{
public:
	SeratoControl(const QString& name = QString());
	SeratoControl(QDomElement& controlNode, bool isOutputNode=false);
	virtual ~SeratoControl();

    bool operator==(const SeratoControl& other) const
    {
    	return m_midiName == other.m_midiName;
    }

public:
	QString getMidiName() const;
    MidiOption getMidiOption() const { return m_midiOption; };
	QString getKey() const;

	QString toString() const
	{
		return QString("SeratoControl[name:%1]").arg(m_midiName);
	}

private:
	QString        m_midiName;
    MidiOption     m_midiOption;
    QString        m_key;
};

inline bool operator<(const SeratoControl &first, const SeratoControl &second)
{
   return (first.getMidiName() < second.getMidiName());
}

/** Hash function needed so we can use SeratoControl in a QHash table */
uint qHash(const SeratoControl& key);

#endif /* SERATOCONTROL_H_ */
