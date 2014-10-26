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

#ifndef ENGINEHEADSIDECHAIN_H
#define ENGINEHEADSIDECHAIN_H

#include <QtCore>
#include "defs.h"
#include "configobject.h"
#include "controlobject.h"
#include "controlobjectthreadmain.h"
#include "errordialoghandler.h"

class EngineNetcastUdp;


class EngineHeadSideChain : public QObject {
  Q_OBJECT
  public:
    EngineHeadSideChain(ConfigObject<ConfigValue> * pConfig);
    virtual ~EngineHeadSideChain();
    void submitSamples(const CSAMPLE* pBuffer, int bufferSize);
  protected slots:
    /** Update the libshout struct with info from Mixxx's shoutcast preferences.*/
    void updateFromPreferences();
    void stateChanged(); // FIXME conectar aos m_netcast para receber updates de ele estar ou nao activo...

  private:
    ConfigObject<ConfigValue> * m_pConfig;
    EngineNetcastUdp* m_pNetcast;

    static bool checkMinSoundPeak(const CSAMPLE* pBuffer, int bufferSize);



    QElapsedTimer debug_timer;
};

#endif
