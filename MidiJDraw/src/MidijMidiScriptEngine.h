/*
 * MidijMidiSciptEngine.h
 *
 *  Created on: 26 de Jun de 2011
 *      Author: Amilcar
 */

#ifndef MIDIJMIDISCIPTENGINE_H_
#define MIDIJMIDISCIPTENGINE_H_

#include "midij/mixxx/script/MidiScriptEngine.h"

class MidijMidiScriptEngine : public MidiScriptEngine
{
public:
	MidijMidiScriptEngine(MidiObject* midi_object=0);
	virtual ~MidijMidiScriptEngine();

	void initEngine(MidiObject* midi_object=0);
	void stopEngine();

	void debugging(bool b);
	bool isDebugging();

protected:
	bool m_debugging;
};


inline bool MidijMidiScriptEngine::isDebugging()
{
	return m_debugging;
};

#endif /* MIDIJMIDISCIPTENGINE_H_ */
