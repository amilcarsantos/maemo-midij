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

#include "enginenetcastudp.h"
#include "configobject.h"
#include "dlgprefnetcast.h"


//TODO #include "recording/encodervorbis.h"
#include "recording/encodermp3.h"

#ifdef __WINDOWS__
	#include <windows.h>
	//sleep on linux assumes seconds where as Sleep on Windows assumes milliseconds
	#define sleep(x) Sleep(x*1000)
#endif


#include <QDebug>
#include <QtNetwork>


//QHostAddress EngineNetcastUdp::audioMulticast = QHostAddress("192.168.2.6");


/*
 * Initialize EngineShoutcast
 */
EngineNetcastUdp::EngineNetcastUdp(ConfigObject<ConfigValue> *_config)
        : m_pConfig(_config),
          m_pEncoder(NULL),
          m_pUdpSocket(NULL),
          m_targetAddress(QHostAddress::Broadcast)
{

    m_pUpdateFromPrefs = new ControlObject(ConfigKey(NETCAST_PREF_KEY, "update_from_prefs"));
    m_pUpdateNetcastFromPrefs = new ControlObjectThreadMain(m_pUpdateFromPrefs);

    // Initialize preferences
    m_isEnabled = false;
    m_autoFlushPending = false;
    m_autoFlushMode = NETCAST_AFM_NOSOUND_VALUE;
    m_autoFlushTimer.invalidate();
    m_targetPort = 0;
    QObject::connect(m_pUpdateNetcastFromPrefs, SIGNAL(valueChanged(double)), this, SLOT(updateFromPreferences()));

    debug_timer.start();
}

void EngineNetcastUdp::shutdown()
{
    QObject::disconnect(m_pUpdateNetcastFromPrefs, 0, this, 0);
    if (m_pEncoder){
		m_pEncoder->flush();
    }
}


/*
 * Cleanup EngineShoutcast
 */
EngineNetcastUdp::~EngineNetcastUdp()
{
 //   QMutexLocker locker(&m_shoutMutex);

    if (m_pEncoder)
    {
		m_pEncoder->flush();
		delete m_pEncoder;
	}

    if (m_pUdpSocket)
    {
    	delete m_pUdpSocket;
    }

    delete m_pUpdateFromPrefs;
    delete m_pUpdateNetcastFromPrefs;
}

bool EngineNetcastUdp::isEnabled()
{
	return m_isEnabled;
}

void EngineNetcastUdp::setDisabled()
{
	m_isEnabled = false;
	m_pConfig->set(ConfigKey(NETCAST_PREF_KEY, "enabled"), ConfigValue("0"));
}

/*
 * Update EngineNetcast values from the preferences.
 */
void EngineNetcastUdp::updateFromPreferences()
{
    qDebug() << "EngineNetcastUdp: updating from preferences";
    EngineNetcastUdp::trace(QString("EngineNetcastUdp__updateFromPreferences ---->"));

    m_pUpdateNetcastFromPrefs->slotSet(0.0f);

    m_isEnabled         = m_pConfig->getValueString(ConfigKey(NETCAST_PREF_KEY, "enabled")).toInt() == 1;

    if (!m_isEnabled)
    {
    	// delete resources
    	if (m_pEncoder != NULL)
    	{
    		delete m_pEncoder;
    		m_pEncoder = NULL;
    	}
    	if (m_pUdpSocket != NULL)
    	{
    		delete m_pUdpSocket;
    		m_pUdpSocket = NULL;
    	}
    	return;
    }

    QString sHost       = m_pConfig->getValueString(ConfigKey(NETCAST_PREF_KEY, "host"));
    QString sNetPort    = m_pConfig->getValueString(ConfigKey(NETCAST_PREF_KEY, "port"));
    QString sBitrate    = m_pConfig->getValueString(ConfigKey(NETCAST_PREF_KEY, "bitrate"));
    QString sAutoFlush  = m_pConfig->getValueString(ConfigKey(NETCAST_PREF_KEY, "autoflush"));

    // FIXME sHost....
    m_targetAddress = QHostAddress::Broadcast;
    foreach(QString address, DlgPrefNetcast::allBroadcastAddresses())
    {
    	if (address == sHost)
    	{
    		m_targetAddress.setAddress(sHost);
    		break;
    	}
    }
    trace(QString("sNetPort $1, bitrate %2, autoflush: %3").arg(sNetPort).arg(sBitrate).arg(sAutoFlush));

    int format;
    int len;
    int protocol;

    ushort port = sNetPort.toUShort();
    if (port < NETCAST_MINVALID_PORT) {
		errorDialog("Error setting port!", "Invalid port number: " + sNetPort);
        return;
    }
    m_targetPort = port;
    if (m_pUdpSocket == NULL) {
    	m_pUdpSocket = new QUdpSocket(this);
    }

    if ((len = sBitrate.indexOf(' ')) != -1)
    {
        sBitrate.resize(len);
    }
    bool convCheck = false;
    int iBitrate = sBitrate.toInt(&convCheck);

    if (!convCheck)
    {
    	iBitrate = 96;
    }

    // Initialize m_encoder
	if(m_pEncoder)
	{
		delete m_pEncoder;		//delete m_encoder if it has been initalized (with maybe) different bitrate
	}

	m_pEncoder = new EncoderMp3(this);

    if (m_pEncoder->initEncoder(iBitrate) < 0)
    {
		//e.g., if lame is not found
		//init m_encoder itself will display a message box
		qDebug() << "**** Encoder init failed";
		delete m_pEncoder;
		m_pEncoder = NULL;
		setDisabled();
    }
}


void EngineNetcastUdp::trace(QString str)
{
	static int debug_index = 0;
	static QString data;

	data += str;
	data += "\n";
	debug_index++;

	if (debug_index >= 499)
	{
		QFile trace("C:\\netmidi.txt");

		if (trace.open(QIODevice::Append | QIODevice::Text))
		{
			trace.write(data.toLocal8Bit());
			trace.close();
		}
		data.clear();
		debug_index = 0;
	}
}


/*
 * Called by the encoder in method 'encodeBuffer()' to flush the stream to the server.
 */
void EngineNetcastUdp::write(unsigned char *header, unsigned char *body,
                                int headerLen, int bodyLen)
{

    // TODO guardar estatistica
	trace(QString("timer:%1; headerLen:%2; bodyLen: %3; body4b: %4")
			.arg(debug_timer.restart(), 10)
			.arg(headerLen, 10)
			.arg(bodyLen,10)
			.arg(*(uint*)body, 8, 16, QChar('0')));

    if (!m_pUdpSocket || m_targetPort < NETCAST_MINVALID_PORT)
        return;
    m_pUdpSocket->writeDatagram((const char*)body, bodyLen, m_targetAddress, m_targetPort);
}


void EngineNetcastUdp::flush()
{
	if(m_pEncoder == NULL)
		return;

	m_pEncoder->flush();
}

/*
 * This is called by the Engine implementation for each sample.
 * Encode and send the stream, as well as manage the auto flush.
 */
void EngineNetcastUdp::process(const CSAMPLE *, const CSAMPLE *pOut, const int iBufferSize)
{
    if (m_isEnabled)
    {
        if (m_pEncoder)
        {
	    	if (iBufferSize > 0)
	    	{
	    		m_pEncoder->encodeBuffer(pOut, iBufferSize); //encode and send to the LAN
	    		autoFlush(true);
	    	}
	    	else
	    		autoFlush(false);
	    }
        else
        {
        	//no encoder?
        	setDisabled();
        }
    }
}

void EngineNetcastUdp::autoFlush(bool hasSound)
{
    if (NETCAST_AFM_ALWAYS(m_autoFlushMode))
    {
		if (hasSound)
		{
    		// sound detected
			m_pEncoder->flush();
		}
		m_autoFlushPending = false;
		return;
    }

    if (NETCAST_AFM_TIMER(m_autoFlushMode))
    {
		if (m_autoFlushPending)
		{
			if (hasSound)
			{
				if (m_autoFlushTimer.isValid())
				{
					if (m_autoFlushTimer.elapsed() > m_autoFlushMode)
					{
						trace("timer flush - timeout");
						// time elapsed, flush encoder and restart timer
						m_pEncoder->flush();
						m_autoFlushTimer.restart();
					}
				}
				else
				{
					m_autoFlushTimer.start();
				}
			}
			else
			{
				// silence detected (!hasSound), flush encoder and stop timer
				trace("timer flush - silence");
				m_pEncoder->flush();
				m_autoFlushTimer.invalidate();
			}
		}
		m_autoFlushPending = hasSound;
		return;
    }

    if (!hasSound && m_autoFlushPending)
    {
        // silence detected
		trace("silence flush");
    	m_pEncoder->flush();
    }
    m_autoFlushPending = hasSound;
}

/* -------- ------------------------------------------------------
Purpose: Common error dialog creation code for run-time exceptions
         Notify user when connected or disconnected and so on
Input:   Detailed error string
Output:  -
-------- ------------------------------------------------------ */
void EngineNetcastUdp::errorDialog(QString text, QString detailedError) {
    qWarning() << "Shoutcast error: " << detailedError;
    ErrorDialogProperties* props = ErrorDialogHandler::instance()->newDialogProperties();
    props->setType(DLG_WARNING);
    props->setTitle(tr("Headphone Broadcast"));
    props->setText(text);
    props->setDetails(detailedError);
    props->setKey(detailedError);   // To prevent multiple windows for the same error
    props->setDefaultButton(QMessageBox::Close);

    props->setModal(false);

    ErrorDialogHandler::instance()->requestErrorDialog(props);
}
