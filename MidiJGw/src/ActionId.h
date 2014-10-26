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

#ifndef ACTIONID_H_
#define ACTIONID_H_

#include <QAction>

class ActionId : public QAction
{
	Q_OBJECT

public:
	ActionId(QObject* parent);
	ActionId(const QString &text, int id, QObject* parent);
	ActionId(const QIcon &icon, const QString &text, int id, QObject* parent);
	virtual ~ActionId();

signals:
	void triggered(QString name);
	void triggered(int id);

private slots:
	void _triggerId();

private:
	int m_id;
};

#endif /* ACTIONID_H_ */
