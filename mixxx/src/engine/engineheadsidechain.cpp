/***************************************************************************

***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

/* This class provides a way to do audio processing that does not need
 * to be executed in real-time. For example, m_shoutcast encoding/broadcasting
 * and recording encoding can be done here. This class uses double-buffering
 * to increase the amount of time the CPU has to do whatever work needs to
 * be done, and that work is executed in a separate thread. (Threading
 * allows the next buffer to be filled while processing a buffer that's is
 * already full.)
 *
 */

#include <QtCore>
#include <QtDebug>

#include "engine/engineheadsidechain.h"
#include "engine/enginebuffer.h"
#include "engine/enginenetcastudp.h"
#include "../sampleutil.h"

EngineHeadSideChain::EngineHeadSideChain(ConfigObject<ConfigValue> * pConfig) {
    m_pConfig = pConfig;

	m_pNetcast = new EngineNetcastUdp(m_pConfig);

	m_pNetcast->updateFromPreferences();

	EngineNetcastUdp::trace("start\n");
    debug_timer.start();
}

EngineHeadSideChain::~EngineHeadSideChain() {

    if (m_pNetcast)
    	m_pNetcast->shutdown();

    //Free up memory
    delete m_pNetcast;
}

#include <QFile>

/** Submit a buffer of samples to be processed in the headsidechain*/
void EngineHeadSideChain::submitSamples(const CSAMPLE* newBuffer, int bufferSize)
{
    EngineNetcastUdp::trace(QString("timer:%1; buffer_size:%2; b0: %3 ---->").arg(debug_timer.restart(), 10).arg(bufferSize, 10).arg(newBuffer[0],10));

    if (m_pNetcast->isEnabled())
    {
        bool hasSound = checkMinSoundPeak(newBuffer, bufferSize);
        if (hasSound)
        {
            // need to process samples right now... always ahead of the output
            m_pNetcast->process(newBuffer, newBuffer, bufferSize);
        }
        else
        {
            // process silence
            m_pNetcast->process(NULL, NULL, 0);
        }
    }
}

bool EngineHeadSideChain::checkMinSoundPeak(const CSAMPLE* pBuffer, int bufferSize)
{
	static const CSAMPLE minSoundPeak = 1.0f;

    for (int i = 4; i >= 0; i--)
    {
    	// search at the beginning
    	if (qAbs(pBuffer[i]) > minSoundPeak)
    		return true;
    	// search at the end
    	if (qAbs(pBuffer[bufferSize - i -1]) > minSoundPeak)
    		return true;
    }
    return false;
}

void EngineHeadSideChain::updateFromPreferences()
{
    qDebug() << "EngineHeadSideChain: updating from preferences";
    EngineNetcastUdp::trace(QString("EngineHeadSideChain__updateFromPreferences ---->"));
}

void EngineHeadSideChain::stateChanged()
{
	// FIXME conectar aos m_netcast para receber updates de ele estar ou nao activo...
}
