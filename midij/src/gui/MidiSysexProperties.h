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

#ifndef MIDISYSEXPROPERTIES_H_
#define MIDISYSEXPROPERTIES_H_

#include <QtCore>

class MidiSysexProperties
{
public:
	MidiSysexProperties(const QString& str);
	virtual ~MidiSysexProperties();

	bool isValid() const
	{
		if (this == NULL)
			return false;
		return m_validProps;
	}
	void apply(QObject* target);

protected:
	bool           m_validProps;
	QString        m_name;
	QVariant       m_value;
};

#endif /* MIDISYSEXPROPERTIES_H_ */
