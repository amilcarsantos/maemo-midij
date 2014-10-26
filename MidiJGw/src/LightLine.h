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

#ifndef LIGHTLINE_H_
#define LIGHTLINE_H_

#include <QFrame>

class LightLine : public QFrame
{
public:
	LightLine(QWidget* parent = 0, Qt::WindowFlags f = 0);
	virtual ~LightLine();

	void lightOn(Qt::GlobalColor color = Qt::green);
	void lightOff();

protected:
    virtual void paintEvent(QPaintEvent *);

    Qt::GlobalColor m_light;
};


inline void LightLine::lightOn(Qt::GlobalColor color)
{
	m_light = color;
	update();
}

inline void LightLine::lightOff()
{
	m_light = Qt::color0;
	update();
}

#endif /* HLIGHTLINE_H_ */
