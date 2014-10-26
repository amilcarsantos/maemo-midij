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

#include "GraphicsMidiOutputItem.h"
#include <QDebug>
#include <qmath.h>
#include "GuiUtils.h"
#include <QStyle>

// XML tags
#define OUTPUT_CONTROLLER_TYPE			"type"
#define OUTPUT_CONTROLLER_IMAGE			"image"


GraphicsMidiOutputItem::GraphicsMidiOutputItem(QDomElement controlNode, const PixmapMap& pixmapMap)
{
	m_outputMidiMessage = MidiMessage(controlNode);

	m_name = controlNode.attribute(CONTROL_NAME, "").trimmed();
	m_groupName = controlNode.attribute(CONTROL_GROUP, "").trimmed();
	m_drawGroupName = controlNode.attribute(CONTROL_DRAWGROUP, "").trimmed();
	if (!m_drawGroupName.isEmpty())
		setData(DRAWGROUP_DATAID, m_drawGroupName);

	QString pixmapName = controlNode.attribute(OUTPUT_CONTROLLER_IMAGE, GuiUtils::background).trimmed();

	GraphicsMidiArea area = parseAreaElement(controlNode);
	if (area.isNull())
		return;

	QString mask = parsePixmapMask(controlNode);

	QPixmap pixmap = pixmapFromMap(pixmapName, area.rect, pixmapMap, mask);
	setSceneRect(area.rect);
	checkDebug(controlNode.attribute("debug", ""), pixmap, area);
	setPixmap(pixmap);

	QString controlType = controlNode.attribute(OUTPUT_CONTROLLER_TYPE, "Button");
	QMetaEnum controllerEnum = MidiOutputController::enumerator("ControlType");
	switch ((MidiOutputController::ControlType)controllerEnum.keyToValue(controlType.toLatin1()))
	{
	default:
	case MidiOutputController::Button:
		qDebug() << m_name << "type: Button.";
		m_outputController = new MidiButton(m_outputMidiMessage);
		break;
	case MidiOutputController::SliderAbsolute:
		qDebug() << m_name << "type: SliderAbsolute!";
		m_outputController = new MidiSliderAbsolute(m_outputMidiMessage, controlNode, area.rect);
		break;
	case MidiOutputController::SliderRelative:
		qDebug() << m_name << "type: SliderRelative!";
		m_outputController = new MidiSliderRelative(m_outputMidiMessage, controlNode, area.rect);
		break;
	case MidiOutputController::WheelAbsolute:
		qDebug() << m_name << "type: WheelRelative!";
		m_outputController = new MidiWheelAbsolute(m_outputMidiMessage, controlNode, area);
		break;
	case MidiOutputController::WheelRelative:
		qDebug() << m_name << "type: WheelRelative!";
		// TODO MidiOutputController::WheelRelative
		m_outputController = new MidiWheelRelative(m_outputMidiMessage, controlNode, area);
		break;
	};

	setPos(area.rect.topLeft());
	setZValue(1);

	m_hasOutput = true;
}

GraphicsMidiOutputItem::~GraphicsMidiOutputItem()
{
//	qDebug() << "GraphicsMidiOutputItem destruido";
}




/**
 * Simple Press button
 */
MidiButton::MidiButton(const MidiMessage& midiMsg)
	: MidiOutputController(midiMsg)
{
}

MidiButton::~MidiButton()
{
}


int MidiButton::midiStatusByte(EventType type)
{
	switch(type)
	{
	case Pressed:
		return MIDI_STATUS_NOTE_ON;
	case Move:
		return 0;
	case Release:
		return MIDI_STATUS_NOTE_OFF;
	}
	return 0;
};


int MidiButton::midiByte2(EventType type, QPointF pos)
{
	Q_UNUSED(pos);

	switch(type)
	{
	case Pressed:
		return m_outputMidiMessage.getMidiByte2On();
	case Move:
		return -1;
	case Release:
		return m_outputMidiMessage.getMidiByte2Off();
	}
	return -1;
}

// XML tags
#define CONTROL_SLIDER				"slider"
#define CONTROL_SLIDER_REVERSE		"reverse"
#define CONTROL_SLIDER_NOTEVALUE	"noteval"
#define CONTROL_SLIDER_NUDGE		"nudge"

/**
 * Slider in Absolute mode
 */
MidiSliderAbsolute::MidiSliderAbsolute(const MidiMessage& midiMsg, QDomElement controlNode, QRect rect)
	: MidiButton(midiMsg)
{
	m_minimum = 0;		// minimum MIDI value
	m_maximum = 127;	// maximum MIDI value
	m_nudge = 12; 		// in px, touch size for Min and Max values
	m_reverse = false;  // reverse touch values (false => min is in top; max is in bottom)
	m_buttonSliderValue = false; // send slider values during NOTE on/off

	m_rect = rect;
	QDomElement sliderNode = controlNode.firstChildElement(CONTROL_SLIDER);
	if (!sliderNode.isNull())
	{
		m_reverse = sliderNode.attribute(CONTROL_SLIDER_REVERSE, "false") == QString("true");
		m_buttonSliderValue = sliderNode.attribute(CONTROL_SLIDER_NOTEVALUE, "") == QString("slider");
		m_nudge = GuiUtils::toIntBound(sliderNode.attribute(CONTROL_SLIDER_NUDGE), 0, 30, m_nudge);
		QString text = sliderNode.text();
		if (text.contains(','))
		{
			QStringList minMax = text.split(",");
			if (minMax.size() == 2)
			{
				int num1 = GuiUtils::toIntBound(minMax.at(0), 0, 127);
				int num2 = GuiUtils::toIntBound(minMax.at(1), 0, 127);
				m_minimum = qMin(num1, num2);
				m_maximum = qMax(num1, num2);
			}
		}
		else
			m_maximum = GuiUtils::toIntBound(text, m_minimum + 1, 127, m_maximum);
	}
}

MidiSliderAbsolute::~MidiSliderAbsolute()
{
}

int MidiSliderAbsolute::midiStatusByte(EventType type)
{
	if (type == Move)
		return m_outputMidiMessage.getMidiStatusByte();

	// button events
	return MidiButton::midiStatusByte(type);
}


int MidiSliderAbsolute::midiByte2(EventType type, QPointF pos)
{
	if (type == Move || m_buttonSliderValue)
	{
		int sliderPos;
		int sliderMax;
		QPoint pt = pos.toPoint();
		qDebug()<< "MidiSliderAbsolute::midiByte2() pt:" << pt;
		if (m_orientation == Qt::Horizontal)
		{
			sliderPos = pt.x() - m_nudge;
			qDebug() << "sliderPos from x:" << sliderPos;
			sliderMax = m_rect.width() - m_nudge * 2;
		}
		else
		{
			sliderPos = pt.y() - m_nudge;
			sliderMax = m_rect.height() - m_nudge * 2;
			qDebug() << "sliderPos from y:" << sliderPos << ";rect:" << m_rect;
		}
		return QStyle::sliderValueFromPosition(m_minimum, m_maximum, sliderPos, sliderMax, m_reverse);
	}
	// button events
	return MidiButton::midiByte2(type, pos);
}

/**
 * Slider in Relative mode
 */
MidiSliderRelative::MidiSliderRelative(const MidiMessage& midiMsg, QDomElement controlNode, QRect rect)
	: MidiSliderAbsolute(midiMsg, controlNode, rect)
{
}

MidiSliderRelative::~MidiSliderRelative()
{
}

int MidiSliderRelative::midiByte2(EventType type, QPointF pos)
{
	int lastMidiByte2;
	switch (type)
	{
	case Pressed:
		m_midiByte2 = MidiSliderAbsolute::midiByte2(Move, pos);
		return MidiButton::midiByte2(Pressed, pos); // skip slider absolute controller
	case Move:
		lastMidiByte2 = m_midiByte2;
		m_midiByte2 = MidiSliderAbsolute::midiByte2(Move, pos);;
		return 64 + m_midiByte2 - lastMidiByte2;
	default:
		return MidiButton::midiByte2(type, pos); // skip slider absolute controller
	}
}


// XML tags
#define CONTROL_WHEEL 			"wheel"
#define CONTROL_WHEEL_REVERSE 	CONTROL_SLIDER_REVERSE
#define CONTROL_WHEEL_NOTEVALUE CONTROL_SLIDER_NOTEVALUE

MidiWheelAbsolute::MidiWheelAbsolute(const MidiMessage& midiMsg, QDomElement controlNode, GraphicsMidiArea area)
	: MidiButton(midiMsg)
{
	m_minimum = 0;
	m_maximum = 127;
	m_reverse = false;
	m_buttonWheelValue = false;

	m_area = area;

	QDomElement sliderNode = controlNode.firstChildElement(CONTROL_WHEEL);
	if (!sliderNode.isNull())
	{
		m_reverse = sliderNode.attribute(CONTROL_WHEEL_REVERSE, "false") == QString("true");
		m_buttonWheelValue = sliderNode.attribute(CONTROL_WHEEL_NOTEVALUE, "") == QString(CONTROL_WHEEL);
	}
}

MidiWheelAbsolute::~MidiWheelAbsolute()
{
}

int MidiWheelAbsolute::midiStatusByte(EventType type)
{
	if (type == Move)
		return m_outputMidiMessage.getMidiStatusByte();

	// button events
	return MidiButton::midiStatusByte(type);
}

int MidiWheelAbsolute::midiByte2(EventType type, QPointF pos)
{
	// QPainterPath must be 2 centered ellipses with a excluding fill behavior (Qt::OddEvenFill)...
	if (!m_area.path.contains(pos) && type != Release)
		return -1; // not within the wheel

	if (type == Move || m_buttonWheelValue)
		return dialValueFromPoint(pos.toPoint(), m_reverse);
	return MidiButton::midiByte2(type, pos);
}


int MidiWheelAbsolute::dialValueFromPoint(const QPoint &p, bool invertedAppearance) const
{
//    Q_Q(const QDial);
	QRect q = m_area.rect;//boundingRect().toRect();
    double yy = (double)q.height()/2.0 - p.y();
    double xx = (double)p.x() - q.width()/2.0;
    double a = (xx || yy) ? qAtan2(yy, xx) : 0;

	if (a < GuiUtils::Q_PI / -2)
        a = a + GuiUtils::Q_PI * 2;

    int dist = 0;
    int minv = m_minimum, maxv = m_maximum;

    if (m_minimum < 0) {
        dist = -m_minimum;
        minv = 0;
        maxv = m_maximum + dist;
    }

    int r = maxv - minv;
    int v;
//    if (wrapping)
        v =  (int)(0.5 + minv + r * (GuiUtils::Q_PI * 3 / 2 - a) / (2 * GuiUtils::Q_PI));
//    else
//        v =  (int)(0.5 + minv + r* (Q_PI * 4 / 3 - a) / (Q_PI * 10 / 6));


    if (dist > 0)
        v -= dist;

    return !invertedAppearance ? bound(v) : m_maximum - bound(v);
}


/**
 * Slider in Relative mode
 */
MidiWheelRelative::MidiWheelRelative(const MidiMessage& midiMsg, QDomElement controlNode, GraphicsMidiArea area)
	: MidiWheelAbsolute(midiMsg, controlNode, area)
{
}

MidiWheelRelative::~MidiWheelRelative()
{
}

int MidiWheelRelative::midiByte2(EventType type, QPointF pos)
{
	if (!m_area.path.contains(pos))
		return -1; // not within the wheel

	int lastMidiByte2, deltaByte2;
	switch (type)
	{
	case Pressed:
		m_midiByte2 = MidiWheelAbsolute::midiByte2(Move, pos);
		return -1;
	case Move:
		lastMidiByte2 = m_midiByte2;
		m_midiByte2 = MidiWheelAbsolute::midiByte2(Move, pos);
		deltaByte2 = m_midiByte2 - lastMidiByte2;
		// Take wrap around into account
		if (deltaByte2 > (m_maximum - 10))
			deltaByte2 = deltaByte2 - (m_maximum - m_minimum + 1);
		else
			if (deltaByte2 < (m_minimum - 10))
				deltaByte2 = (m_maximum - m_minimum + 1) + deltaByte2;

		return 64 + deltaByte2;
	default:
		return m_outputMidiMessage.getMidiByte2Off();  // BUTTON OFF event
	}
}

