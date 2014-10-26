/***************************************************************************
                          dlgprefshoutcast.h  -  description
                             -------------------
    begin                : Thu Jun 7 2007
    copyright            : (C) 2008 by Wesley Stessens
                           (C) 2007 by John Sully
                           (C) 2008 by Albert Santoni
    email                : 
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef DLGPREFNETCAST_H
#define DLGPREFNETCAST_H

#include "ui_dlgprefnetcastdlg.h"
#include "controlobject.h"
#include "configobject.h"
#include "controlobjectthreadmain.h"

#define NETCAST_PREF_KEY "[Netcast]"
#define NETCAST_DEFAULT_PORT "1234"

#define NETCAST_MINVALID_PORT		1024


// Auto flush when no sound is detected
#define NETCAST_AFM_NOSOUND_VALUE	0
// Always flush
#define NETCAST_AFM_ALWAYS_VALUE	0xFFFF
// Auto flush timer max value (in milliseconds) 1..10000
#define NETCAST_AFM_MAXTIMER_VALUE	10000


#define NETCAST_AFM_ALWAYS(x)	(x == NETCAST_AFM_ALWAYS_VALUE)
#define NETCAST_AFM_NOSOUND(x)	(x == NETCAST_AFM_NOSOUND_VALUE)
#define NETCAST_AFM_TIMER(x)	(x > NETCAST_AFM_NOSOUND_VALUE && x <= NETCAST_AFM_MAXTIMER_VALUE)




class QWidget;
/**
  *@author John Sully
  */

class DlgPrefNetcast : public QWidget, public Ui::DlgPrefNetcastDlg  {
    Q_OBJECT
public: 
    DlgPrefNetcast(QWidget *parent, ConfigObject<ConfigValue> *_config);
    ~DlgPrefNetcast();

public slots:
    /** Apply changes to widget */
    void slotApply();
    void slotUpdate(); //Does nothing but keep convention (and DlgPreferences happy)
    void enableGroupBoxes(int state);

    static QList<QString> allBroadcastAddresses();

private:
    static QString autoFlushString(int value, QComboBox* combobox);
    static int autoFlushValue(QString value);

    /** Pointer to config object */
    ConfigObject<ConfigValue> *m_pConfig;
    ControlObjectThreadMain* m_pUpdateShoutcastFromPrefs; //If set to 1, EngineShoutcast will update it's settings.

};

#endif
