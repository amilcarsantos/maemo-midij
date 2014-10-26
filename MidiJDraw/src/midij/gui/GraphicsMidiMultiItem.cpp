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

#include "GraphicsMidiMultiItem.h"
#include <QDebug>

#define ITEM_CONTROL 	"control"
#define ITEM_STATE 		"state"


GraphicsMidiMultiItem::GraphicsMidiMultiItem(QDomElement multiControlNode, const QHash<QString, GraphicsMidiControlItem*>& midiControlsMap)
{
	m_offState = INVALID_STATE_ID; // no off state??
	setVisible(false);	// and not visible...

	// map a MIDI message to a
	m_inputMidiMessages.append(MidiMessage(multiControlNode));

	m_name = multiControlNode.attribute(CONTROL_NAME, "");

	// get other controls
	QDomElement controlNode = multiControlNode.firstChildElement(ITEM_CONTROL);

	while (!controlNode.isNull())
	{
		QString control = controlNode.text();
		// map range to m_state2Pixmap
		GraphicsMidiControlItem* controlItem = midiControlsMap.value(control);
		if (controlItem && controlItem->hasInput() && controlItem->isVisible())
		{
			// accept control
			m_midiControls.append(controlItem);
		}
		else
			qWarning() << "control not valid" << control;

		controlNode = controlNode.nextSiblingElement(ITEM_CONTROL);
	}

	// get states for controls
	QDomElement stateNode = multiControlNode.firstChildElement(ITEM_STATE);
	while (!stateNode.isNull())
	{
		QString stateValue = stateNode.attribute("value").trimmed();
		QHash<GraphicsMidiControlItem*, ushort> controls2State;
		bool ok = false;
		ushort stateId = stateValue.toUShort(&ok, 0);
		if (ok)
		{
			QDomElement control2Node = stateNode.firstChildElement(ITEM_CONTROL);
			while (!control2Node.isNull())
			{
				QString control = control2Node.text();
				QString controlState = control2Node.attribute("state").trimmed();
				bool ok = false;
				ushort controlStateId = controlState.toUShort(&ok, 0);

				GraphicsMidiControlItem* controlItem = midiControlsMap.value(control);
				if (controlItem && controlItem->hasInput() && controlItem->isVisible() && ok)
				{
					// accept control
					controls2State.insert(controlItem, controlStateId);
				}
				else
					qWarning() << m_name << "state/control not valid" << stateValue << control;

				control2Node = control2Node.nextSiblingElement(ITEM_CONTROL);
			}
			m_states.insert(stateId, controls2State);
		}
		else
			qWarning() << m_name << "state not valid" << stateValue;

		stateNode = stateNode.nextSiblingElement(ITEM_STATE);
	}

	m_hasInput = !m_states.isEmpty();
}

GraphicsMidiMultiItem::~GraphicsMidiMultiItem()
{
}

//
// Handles the behavior of this control
void GraphicsMidiMultiItem::receivedMidiShortMsg(const MidiMessage& msg, uchar midivalue)
{
	if (m_states.contains(midivalue))
	{
		qWarning() << "GraphicsMidiMultiItem: FOUND MIDI VALUE >>>>>>" << m_name << "; midivalue:" << midivalue << " states" << m_states.value(midivalue).size();

		QHash<GraphicsMidiControlItem*, ushort> controls2State = m_states.value(midivalue);
		foreach(GraphicsMidiControlItem* midiControl, m_midiControls)
		{
			if (controls2State.contains(midiControl))
				midiControl->receivedMidiShortMsg(MidiMessage(), controls2State.value(midiControl));
			else
				midiControl->setOffState();
		}
		return;
	}
	qWarning() << "GraphicsMidiMultiItem: UNHANDLED MIDI VALUE >>>>>>" << m_name << "; midivalue:" << midivalue;
}
