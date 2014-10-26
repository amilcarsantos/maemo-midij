/*
 * MidiMappingDraw.h
 *
 *  Created on: 11 de Abr de 2011
 *      Author: pt101821
 */

#ifndef MIDIMAPPINGDRAW_H_
#define MIDIMAPPINGDRAW_H_

#include "midij/gui/MidiMappingGui.h"

class MidiMappingDraw: public MidiMappingGui
{
public:
	MidiMappingDraw();
	virtual ~MidiMappingDraw();


	QList<GraphicsMidiControlItem* > controls()
	{
		return m_midiControlsMap.values();
	}

	QMap<QString, MidiSysexAction*> midiSysexActionMap()
	{
		return m_midiSysexActionMap;
	}

};

#endif /* MIDIMAPPINGDRAW_H_ */
