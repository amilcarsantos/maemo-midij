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

#include "SeratoControl.h"
//#include <QtCore>

SeratoControl::SeratoControl(const QString& name)
{
	m_midiName = name;
}

SeratoControl::SeratoControl(QDomElement& node, bool isOutputNode)
{
	Q_UNUSED(isOutputNode);
    QDomElement optionsNode = node.firstChildElement("options");

    // At the moment, use one element, in future iterate through options
    QString strMidiOption;
    if (optionsNode.hasChildNodes()) {
        strMidiOption = optionsNode.firstChild().nodeName().toLower();
    } else {
        strMidiOption = "normal";
    }

    if (strMidiOption == "normal")
        m_midiOption = MIDI_OPT_NORMAL;
    else if (strMidiOption == "invert")
        m_midiOption = MIDI_OPT_INVERT;
    else if (strMidiOption == "rot64")
        m_midiOption = MIDI_OPT_ROT64;
    else if (strMidiOption == "rot64inv")
        m_midiOption = MIDI_OPT_ROT64_INV;
    else if (strMidiOption == "rot64fast")
        m_midiOption = MIDI_OPT_ROT64_FAST;
    else if (strMidiOption == "diff")
        m_midiOption = MIDI_OPT_DIFF;
    else if (strMidiOption == "button")
        m_midiOption = MIDI_OPT_BUTTON;
    else if (strMidiOption == "switch")
        m_midiOption = MIDI_OPT_SWITCH;
    else if (strMidiOption == "hercjog")
        m_midiOption = MIDI_OPT_HERC_JOG;
    else if (strMidiOption == "spread64")
        m_midiOption = MIDI_OPT_SPREAD64;
    else if (strMidiOption == "selectknob")
        m_midiOption = MIDI_OPT_SELECTKNOB;
    else if (strMidiOption == "script-binding")
        m_midiOption = MIDI_OPT_SCRIPT;
    else {
        m_midiOption = MIDI_OPT_NORMAL;
        qDebug() << "Warning: Unknown midioption" << strMidiOption << "in" << __FILE__;
    }

	m_midiName = node.attribute("name");
	m_key = node.firstChildElement("key").text();
}


SeratoControl::~SeratoControl()
{
}

QString SeratoControl::getMidiName() const
{
    return m_midiName;
}

QString SeratoControl::getKey() const
{
    return m_key;
}


uint qHash(const SeratoControl& key)
{
    return qHash(key.getMidiName());
}


