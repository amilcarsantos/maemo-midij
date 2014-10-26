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

#ifndef GUIUTILS_H_
#define GUIUTILS_H_

#ifdef Q_WS_MAEMO_5
#include <QtMaemo5/qmaemo5valuebutton.h>
#include <QtMaemo5/qmaemo5listpickselector.h>
#include <QtMaemo5/qmaemo5informationbox.h>
#else
// Only to work on MS Windows
#include "maemo5/qmaemo5valuebutton.h"
#include "maemo5/qmaemo5listpickselector.h"
#include "maemo5/qmaemo5informationbox.h"
#endif

#include "../Utils.h"

/*
#define MIDICONTROL_XMLELEMENT_OUTPUT	0x3C00
#define MIDICONTROL_XMLELEMENT_INPUT	0x3C01
#define MIDICONTROL_XMLELEMENT_MULTI	0x3C02
*/

namespace GuiUtils
{
	static const qreal Q_PI   = qreal(3.14159265358979323846);   // pi

	// debug hilight all controls
	const QString debugColor = ""; //"#505050";

	// "background"/off state id
	const QString background = "background";

	// checks if GUI item state is the Background
	inline bool isBackground(const QString& state)
	{
		return state == background;
	}


	const QString heuristic = "heuristic";

	// check if heuristic mask is selected
	inline bool isHeuristicMask(const QString mask)
	{
		return mask == heuristic;
	}

	// return a parsed Integer or <code>defaultValue</code> if not valid
	inline int toInt(const QString& value, int defaultValue)
	{
		bool ok = false;
		int v = value.toInt(&ok);
		if (ok)
			return v;
		return defaultValue;
	}

	// return a parsed Integer or <code>defaultValue</code> if not valid
	inline int toIntBound(const QString& value, int min, int max, int defaultValue = 0)
	{
		bool ok = false;
		int v = value.toInt(&ok);
		if (ok)
			return qBound(min, v, max);
		return defaultValue;
	}

	// return a parsed Integer (dec. or hex.) or defaultValue if not valid
	inline int toIntX(const QString& value, int defaultValue)
	{
		bool ok = false;
		int v = value.toInt(&ok, 0);
		if (ok)
			return v;
		return defaultValue;
	}

	// return a parsed Integer (dec. or hex.) or <code>0 (zero)</code> and increments <code>failCount</code> if not valid
	inline int toInt(const QString& value, ushort& failCount)
	{
		bool ok = false;
		int v = value.toInt(&ok);
		if (ok)
			return v;
		failCount++;
		return 0;
	}

}
#endif /* GUIUTILS_H_ */
