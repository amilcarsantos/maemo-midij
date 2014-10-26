/***************************************************************************
                          dlgprefnetcast.cpp  -  description
                             -------------------
    begin                : Thu Jun 19 2021
    copyright            : (C) 2012 by Amilcar Santos
    email                : amilcar.santos@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "dlgprefnetcast.h"
#include "defs_urls.h"
#include <QtDebug>
#include <QtCore>
#include <QtGui>
#include <QNetworkInterface>

DlgPrefNetcast::DlgPrefNetcast(QWidget *parent, ConfigObject<ConfigValue> *_config) : QWidget(parent), Ui::DlgPrefNetcastDlg()
{
    m_pConfig = _config;
    int tmp_index = 0;  //Used for finding the index of an element by name in a combobox.
    QString tmp_string;
    setupUi(this);

    m_pUpdateShoutcastFromPrefs = new ControlObjectThreadMain(ControlObject::getControl(ConfigKey(NETCAST_PREF_KEY, "update_from_prefs")));

    //Enable LAN PFL broadcast checkbox
    enableHeadphoneBroadcast->setChecked((bool)m_pConfig->getValueString(ConfigKey(NETCAST_PREF_KEY,"enabled")).toInt());
    
    //Protocol type combobox
    tmp_index = comboBoxProtocolType->findText(m_pConfig->getValueString(ConfigKey(NETCAST_PREF_KEY,"protocoltype")));
    if (tmp_index < 0) //Set default if invalid.
        tmp_index = 0;
    comboBoxProtocolType->setCurrentIndex(tmp_index);

    //Host
    foreach(QString address, allBroadcastAddresses())
    {
    	comboBoxHost->addItem(address);
    }
    tmp_index = comboBoxHost->findText(m_pConfig->getValueString(ConfigKey(NETCAST_PREF_KEY,"host")));
    if (tmp_index < 0) //Set default if invalid.
        tmp_index = 0;
    comboBoxHost->setCurrentIndex(tmp_index);

    //Port
    tmp_string = m_pConfig->getValueString(ConfigKey(NETCAST_PREF_KEY,"port"));
    if (tmp_string.isEmpty())
        tmp_string = QString(NETCAST_DEFAULT_PORT);
    port->setText(tmp_string);

    //Encoding bitrate combobox
    tmp_index = comboBoxEncodingBitrate->findText(m_pConfig->getValueString(ConfigKey(NETCAST_PREF_KEY,"bitrate")));
    if (tmp_index < 0) //Set default if invalid.
        tmp_index = 7; // "96 kpbs"
    comboBoxEncodingBitrate->setCurrentIndex(tmp_index);

    //Encoding format combobox
    tmp_index = comboBoxEncodingFormat->findText(m_pConfig->getValueString(ConfigKey(NETCAST_PREF_KEY,"format")));
    if (tmp_index < 0) //Set default if invalid.
        tmp_index = 0;
    comboBoxEncodingFormat->setCurrentIndex(tmp_index);

    //Encoding channels combobox
    tmp_index = comboBoxEncodingChannels->findText(m_pConfig->getValueString(ConfigKey(NETCAST_PREF_KEY,"channels")));
    if (tmp_index < 0) //Set default if invalid.
        tmp_index = 0;
    comboBoxEncodingChannels->setCurrentIndex(tmp_index);    

    tmp_index = comboBoxAutoFlush->findText(autoFlushString(m_pConfig->getValueString(ConfigKey(NETCAST_PREF_KEY, "autoflush")).toInt(), comboBoxAutoFlush));
    if (tmp_index < 0) //Set default if invalid.
        tmp_index = 0;
    comboBoxAutoFlush->setCurrentIndex(tmp_index);


    slotApply();
    
}

DlgPrefNetcast::~DlgPrefNetcast()
{
}

QString DlgPrefNetcast::autoFlushString(int value, QComboBox* combobox)
{
	QString mask = "silence";
	if (NETCAST_AFM_ALWAYS(value))
	{
		mask = "frame";
	}
	else if (value == 500)
	{
		mask = "half";
	}
	else if (NETCAST_AFM_TIMER(value))
	{
		mask = QString("%1 ").arg(value / 1000);
	}

	int index = qMax(combobox->findText(mask, Qt::MatchContains), 0);
	return combobox->itemText(index);
}

int DlgPrefNetcast::autoFlushValue(QString value)
{
	if (value.contains("silence", Qt::CaseInsensitive))
	{
		return NETCAST_AFM_NOSOUND_VALUE;
	}
	if (value.contains("frame", Qt::CaseInsensitive))
	{
		return NETCAST_AFM_ALWAYS_VALUE;
	}
	// in seconds
	if (value.contains("half", Qt::CaseInsensitive))
	{
		return 500;
	}

	int len;
	if ((len = value.indexOf(' ')) != -1)
	{
		value.resize(len);
		int interval = value.toInt() * 1000;
		if (NETCAST_AFM_TIMER(interval))
			return interval;
	}
	// by default
	return NETCAST_AFM_NOSOUND_VALUE;
}

void DlgPrefNetcast::slotUpdate()
 {
	int state = m_pConfig->getValueString(ConfigKey(NETCAST_PREF_KEY, "enabled")).toInt();
	enableHeadphoneBroadcast->setChecked((bool)state);
	enableGroupBoxes(state);
}

void DlgPrefNetcast::enableGroupBoxes(int state)
{
    groupBox->setEnabled(state > 0);
    groupBox_2->setEnabled(state > 0);
    groupBox_3->setEnabled(state > 0);
}

void DlgPrefNetcast::slotApply()
{
    m_pConfig->set(ConfigKey(NETCAST_PREF_KEY, "enabled"),       ConfigValue(enableHeadphoneBroadcast->isChecked()));
    m_pConfig->set(ConfigKey(NETCAST_PREF_KEY, "protocoltype"),  ConfigValue(comboBoxProtocolType->currentText()));
    m_pConfig->set(ConfigKey(NETCAST_PREF_KEY, "host"),          ConfigValue(comboBoxHost->currentText()));
    m_pConfig->set(ConfigKey(NETCAST_PREF_KEY, "port"),          ConfigValue(port->text()));
    m_pConfig->set(ConfigKey(NETCAST_PREF_KEY, "bitrate"),       ConfigValue(comboBoxEncodingBitrate->currentText()));
    m_pConfig->set(ConfigKey(NETCAST_PREF_KEY, "format"),        ConfigValue(comboBoxEncodingFormat->currentText()));
    m_pConfig->set(ConfigKey(NETCAST_PREF_KEY, "channels"),      ConfigValue(comboBoxEncodingChannels->currentText()));
    m_pConfig->set(ConfigKey(NETCAST_PREF_KEY, "autoflush"),     ConfigValue(autoFlushValue(comboBoxAutoFlush->currentText())));

    //Tell the EngineShoutcast object to update with these values by toggling this control object.
    m_pUpdateShoutcastFromPrefs->slotSet(1.0f);
}


QList<QString> DlgPrefNetcast::allBroadcastAddresses()
{
	QList<QString> addressList;
	QList<QNetworkInterface> hostInterfaces = QNetworkInterface::allInterfaces();
	foreach(QNetworkInterface interface, hostInterfaces)
	{
		foreach(QNetworkAddressEntry entry, interface.addressEntries())
		{
			QString address = entry.broadcast().toString().trimmed();
			if (!address.isEmpty())
				addressList.append(address);
		}
	}

	return addressList;
}
