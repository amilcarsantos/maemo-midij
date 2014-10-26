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

#ifndef GRAPHICSMIDIOUTPUTITEM_H_
#define GRAPHICSMIDIOUTPUTITEM_H_

#include "GraphicsMidiControlItem.h"

// Handles MIDI button controllers
class MidiButton : public MidiOutputController
{
public:
	MidiButton(const MidiMessage& midiMsg);
	virtual ~MidiButton();

	// Overrides
public:
	int midiStatusByte(EventType type);
	int midiByte2(EventType type, QPointF pos);
	bool moveAfterPressed()
	{
		return false;
	}
	ControlType controlType()
	{
		return Button;
	}
};

// Handles MIDI Absolute Slider controllers
class MidiSliderAbsolute : public MidiButton
{
public:
	MidiSliderAbsolute(const MidiMessage& midiMsg, QDomElement controlNode, QRect rect);
	virtual ~MidiSliderAbsolute();

	// Overrides
public:
	int midiStatusByte(EventType type);
	int midiByte2(EventType type, QPointF pos);
	bool moveAfterPressed()
	{
		return true;
	}
	ControlType controlType()
	{
		return SliderAbsolute;
	}

protected:
	QRect m_rect;
	Qt::Orientation m_orientation;
	int m_minimum;
	int m_maximum;
	int m_nudge;
	bool m_reverse;
	bool m_buttonSliderValue;
};

// Handles MIDI Relative Slider controllers
class MidiSliderRelative : public MidiSliderAbsolute
{
public:
	MidiSliderRelative(const MidiMessage& midiMsg, QDomElement controlNode, QRect rect);
	virtual ~MidiSliderRelative();

	// Overrides
public:
	int midiByte2(EventType type, QPointF pos);
	bool moveAfterPressed()
	{
		return false;
	}
	ControlType controlType()
	{
		return SliderRelative;
	}

protected:
	int m_midiByte2;
};


// Handles MIDI Absolute Wheel controllers
class MidiWheelAbsolute : public MidiButton
{
public:
	MidiWheelAbsolute(const MidiMessage& midiMsg, QDomElement controlNode, GraphicsMidiArea area);
	virtual ~MidiWheelAbsolute();

	// Overrides
public:
	int midiStatusByte(EventType type);
	bool moveAfterPressed()
	{
		return true;
	}
	int midiByte2(EventType type, QPointF pos);
	ControlType controlType()
	{
		return WheelRelative;
	}

protected:
	int dialValueFromPoint(const QPoint &p, bool invertedAppearance) const;
	inline int bound(int val) const { return qMax(m_minimum, qMin(m_maximum, val)); }

	GraphicsMidiArea m_area;
	Qt::Orientation m_orientation;
	int m_minimum;
	int m_maximum;
	int m_nudge;
	bool m_reverse;
	bool m_buttonWheelValue;
};


// TODO Handles MIDI Relative Wheel controllers
class MidiWheelRelative : public MidiWheelAbsolute
{
public:
	MidiWheelRelative(const MidiMessage& midiMsg, QDomElement controlNode, GraphicsMidiArea area);
	virtual ~MidiWheelRelative();

	// Overrides
public:
	int midiByte2(EventType type, QPointF pos);
	bool moveAfterPressed()
	{
		return false;
	}
	ControlType controlType()
	{
		return WheelRelative;
	}

protected:
	int m_midiByte2;
};



class GraphicsMidiOutputItem : public GraphicsMidiControlItem
{
public:
	GraphicsMidiOutputItem(QDomElement controlNode, const PixmapMap& pixmapMap);
	virtual ~GraphicsMidiOutputItem();
};

#endif /* CGRAPHICSMIDIOUTPUTITEM_H_ */
