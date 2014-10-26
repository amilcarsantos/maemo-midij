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

#ifndef GRAPHICSMIDIINPUTITEM_H_
#define GRAPHICSMIDIINPUTITEM_H_

#include "GraphicsMidiControlItem.h"

class GraphicsMidiInputItem : public GraphicsMidiControlItem
{
public:
	GraphicsMidiInputItem(QDomElement controlNode, const PixmapMap& pixmapMap);
	virtual ~GraphicsMidiInputItem();

public:
	void mergeInto(GraphicsMidiControlItem* target);
	static QPixmap pixmapFromState(const QDomElement& stateNode, QPixmap bgPixmap, QRect sourceRect, const PixmapMap& pixmapMap, const QString& mask);
};

#endif /* GRAPHICSMIDIINPUTITEM_H_ */
