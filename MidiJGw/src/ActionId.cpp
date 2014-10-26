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

#include "ActionId.h"

ActionId::ActionId(QObject* parent) : QAction(parent)
{
}

ActionId::ActionId(const QString &text, int id, QObject* parent)
	: QAction(text, parent)
{
	m_id = id;
	connect(this, SIGNAL(triggered()), this, SLOT(_triggerId()), Qt::DirectConnection);
}

ActionId::ActionId(const QIcon &icon, const QString &text, int id, QObject* parent)
	: QAction(icon, text, parent)
{
	m_id = id;
	connect(this, SIGNAL(triggered()), this, SLOT(_triggerId()), Qt::DirectConnection);
}


ActionId::~ActionId()
{
}


void ActionId::_triggerId()
{
	emit triggered(m_id);
	emit triggered(text());
}
