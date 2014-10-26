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

#ifndef ENGINENETCASTUDP_H
#define ENGINENETCASTUDP_H

#include <QObject>
#include <QMutex>
#include <QMessageBox>

#include "engineabstractrecord.h"
#include "configobject.h"
#include "controlobject.h"
#include "controlobjectthreadmain.h"
//XXX#include "trackinfoobject.h"
#include "errordialoghandler.h"
#include "recording/encoder.h"

#include <QtCore>
#include <qudpsocket.h>

class EncoderVorbis;

class EngineNetcastUdp : public EngineAbstractRecord {
    Q_OBJECT
  public:
    EngineNetcastUdp(ConfigObject<ConfigValue> *_config);
    virtual ~EngineNetcastUdp();

    void process(const CSAMPLE *pIn, const CSAMPLE *pOut, const int iBufferSize);
    // writes to UDP socket
    void write(unsigned char *header, unsigned char *body,
               int headerLen, int bodyLen);
    void flush();
    /** connects to server **/
    bool serverConnect();
    bool serverDisconnect();
    bool isEnabled();

    void shutdown();

    QElapsedTimer debug_timer;
    static void trace(QString str);
  public slots:
    /** Update the libshout struct with info from Mixxx's shoutcast preferences.*/
    void updateFromPreferences();

  private:
    void setDisabled();
    void autoFlush(bool hasSound);
    void updateMetaData();

    QUdpSocket* m_pUdpSocket;

    ConfigObject<ConfigValue> *m_pConfig;
    Encoder *m_pEncoder;
    ControlObject* m_pUpdateFromPrefs;
    ControlObjectThreadMain* m_pUpdateNetcastFromPrefs;

    bool m_isEnabled;
    int m_autoFlushMode;
    QElapsedTimer m_autoFlushTimer;
    bool m_autoFlushPending;

    QByteArray m_baFormat;
    /* Standard error dialog */
    void errorDialog(QString text, QString detailedError);
    /** we static metadata is used, we only need calling shout_set_metedata once */
    QHostAddress m_targetAddress;
    ushort m_targetPort;
};

#endif
