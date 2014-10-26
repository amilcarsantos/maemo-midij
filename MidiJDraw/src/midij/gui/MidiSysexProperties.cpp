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

#include "MidiSysexProperties.h"

MidiSysexProperties::MidiSysexProperties(const QString& str)
{
	QStringList property = str.split(':');
	m_name = property.at(0);

	// TODO switch(g_name2Type.value(m_name))
	//{
	// case QVariant::Bool:
	m_value = QVariant(property.at(1) == "true");
	m_validProps = true;
	// default:
	//	m_validProps = true;
	//}
}

MidiSysexProperties::~MidiSysexProperties()
{
}



void MidiSysexProperties::apply(QObject* target)
{
	//qDebug("MidiSysexProperties::apply(): %s", m_name.toAscii());

	target->setProperty(m_name.toAscii(), m_value);
}
