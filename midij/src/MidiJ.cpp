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

#include "MidiJ.h"
#include <QApplication>
#include <QtGui/QGraphicsScene>
#include <QtGui/QPixmap>
#include <QtGui/QGraphicsPixmapItem>
#include "gui/GraphicsClickItem.h"
#include "mixxx/MidiObject.h"
#include "MidiObjectGuiEmul.h"
#include "MidiObjectUdp.h"
#include "RouterObject.h"
#include "settings/SettingsDialog.h"
#include "Utils.h"
#include "gui/GuiUtils.h"
#include "gst/GstUdpAudioPlayer.h"

#ifdef Q_WS_MAEMO_5
#include <QtDBus>
#endif // Q_WS_MAEMO_5



#define WINDOW_BAR_HEIGHT			68

#define LSPB_INIT					1
#define LSPB_SCHEMEFILE				2
#define LSPB_GUIMAPPING_MIDICONTRL	3
#define LSPB_GUIMAPPING_ROUTER		4
#define LSPB_GUIMAPPING_END			5


MidiJ::MidiJ(QWidget *parent)
    : QWidget(parent)
{
#ifdef Q_WS_MAEMO_5
//	setAttribute(Qt::WA_Maemo5StackedWindow, true); // BUG: some flickering while trying to reopen advanced settings
#endif
	m_pView = new QGraphicsView(this);
//    setStyleSheet(QString::fromUtf8("border: 0px"));
//    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	m_pView->setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing);
	m_pView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_pView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_scene.setBackgroundBrush(QBrush(Qt::black));
	m_pView->setScene(&m_scene);

	m_scene.setSceneRect(QRect(QPoint(0, 0), desktopSize()));
	m_menuVisible = false;
	m_menuToggleButton = NULL;
	m_menuGroup = NULL;
	m_loadingGroup = NULL;
	m_netcastManager = NULL;

	m_router = new RouterObject();
	m_midiControler = new MidiObjectGuiEmul();
	m_midiConnection = new MidiObjectUdp(MidiObjectUdp::BidirClient);
	m_midiConnection->updateDeviceList();
	m_netcastManager = new NetcastManager();
	m_settings = new SettingsManager();

	QVBoxLayout* layout = new QVBoxLayout();
	layout->setMargin(0);
	layout->addWidget(m_pView);
	setLayout(layout);
}


MidiJ::~MidiJ()
{
	m_pView->setScene(NULL);
	_delete(m_router);
	_delete(m_midiControler);
	_delete(m_midiConnection);
	_delete(m_netcastManager);
	_delete(m_settings);
}


void MidiJ::showLoadingScreen(bool show)
{
	if (!m_loadingGroup)
	{
		// create "Loading screen" items
		QPixmap loadingBg(desktopSize());
		loadingBg.fill(QColor(46,44,45,255));
		QGraphicsItem* loadingBgItem = new QGraphicsPixmapItem(loadingBg);
		loadingBgItem->setPos(0, 0);

		QGraphicsSimpleTextItem* loadingItem = new QGraphicsSimpleTextItem("Loading...");
		QFont font = loadingItem->font();
		font.setPointSize(12);
		loadingItem->setFont(font);
		loadingItem->setPen(QPen(Qt::white));
		loadingItem->setBrush(Qt::white);

	//	QRectF r(0, 0 , 480, 800);
		QPoint desktopCenter = QRect(QPoint(), desktopSize()).center();
		QRectF br = loadingItem->boundingRect();
		br.moveCenter(desktopCenter);
		loadingItem->setPos(br.topLeft());

		m_progressItem = new GraphicsProgressItem(LSPB_GUIMAPPING_END);  // total of 5 steps during the "Loading..." screen
		br = m_progressItem->boundingRect();
		br.moveCenter(desktopCenter);
		m_progressItemRect = QRect(br.bottomLeft().toPoint(), br.size().toSize());
		m_progressItem->setPos(br.bottomLeft());

		m_loadingGroup = new GraphicsObjectItemGroup();
		m_loadingGroup->setZValue(100);
		m_loadingGroup->addToGroup(loadingBgItem);
		m_loadingGroup->addToGroup(loadingItem);
		m_loadingGroup->addToGroup(m_progressItem);
		m_scene.addItem(m_loadingGroup);
	}

	if (show)
	{
		// update full screen
		m_pView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	}
	else
	{
		// reset progress in Hide
		m_progressItem->setProgress(0);
	}
	m_loadingGroup->setVisible(show);
}


void MidiJ::progressLoadingScreen(int pos)
{
	if (m_loadingGroup != NULL && m_loadingGroup->isVisible())
	{
		m_progressItem->setProgress(pos);
		m_pView->viewport()->repaint(m_progressItemRect);
	}
}


bool MidiJ::init(QString schemeHomeDir, QString schemeAltDir)
{
	// icon 48x48 for Schemes without icon
	QIcon defaultSchemeIcon48 = QIcon(":/images/midij_48.png");
	m_netcastManager->init();
	m_settings->init(schemeHomeDir, schemeAltDir, defaultSchemeIcon48, m_midiConnection, m_netcastManager);

	// create "Menu" items
	QPixmap menuBg(desktopSize().width(), WINDOW_BAR_HEIGHT);
	menuBg.fill(QColor(32,32,32,100));
//	menuBg.fill(QColor(160,160,160,255));
	GraphicsClickItem* menuBgItem = new GraphicsClickItem(menuBg, QPixmap(), QRect(0,0,480, WINDOW_BAR_HEIGHT));
	menuBgItem->setZValue(0);
	connect(menuBgItem, SIGNAL(clicked()), SLOT(menuHide()));

	// CLOSE button
	QRect closeButtonRect(desktopSize().width() - 80, 0, 80, 65);
	QPixmap closePix, closeHiPix;
	closePix.load(":/images/close04_62.png");
	closeHiPix.load(":/images/close04_hi.png");
	if (closePix.isNull() || closeHiPix.isNull())
	{
		qFatal("fail to open CLOSE icons");
		return false;
	}
	GraphicsClickItem* closeButton = new GraphicsClickItem(closePix, closeHiPix, closeButtonRect);
//	m_scene.addItem(closeButton);
	connect(closeButton, SIGNAL(clicked()), SLOT(doClose()));

	// task switch button
	QRect taskButtonRect(0, 0, 80, 65);
	QPixmap taskPix, taskHiPix;
	taskPix.load(":/images/task04_62.png");
	taskHiPix.load(":/images/task04_hi.png");
	if (taskPix.isNull() || taskHiPix.isNull())
	{
		qFatal("fail to open TASK icons");
		return false;
	}
	GraphicsClickItem* taskButton = new GraphicsClickItem(taskPix, taskHiPix, taskButtonRect);
//	m_scene.addItem(taskButton);
	connect(taskButton, SIGNAL(clicked()), SLOT(doTaskView()));

	// settings button
	QRect setupButtonRect(2 + 80, 0, 80, 65);
	QPixmap toolsPix, toolsHiPix;
	toolsPix.load(":/images/tools04_62.png");
	toolsHiPix.load(":/images/tools04_hi.png");
	if (toolsPix.isNull() || toolsHiPix.isNull())
	{
		qFatal("fail to open TOOLS icons");
		return false;
	}
	GraphicsClickItem* settingsButton = new GraphicsClickItem(toolsPix, toolsHiPix, setupButtonRect);
	connect(settingsButton, SIGNAL(clicked()), SLOT(doSettings()), Qt::QueuedConnection);

	m_menuGroup = new GraphicsObjectItemGroup();
	m_menuGroup->addToGroup(menuBgItem);
	m_menuGroup->addToGroup(taskButton);
	m_menuGroup->addToGroup(settingsButton);
	m_menuGroup->addToGroup(closeButton);
	m_menuGroup->setZValue(3);
	m_scene.addItem(m_menuGroup);

	m_menuAnim.setTargetObject(m_menuGroup);
	m_menuAnim.setPropertyName("y");
	m_menuAnim.setDuration(550);
	m_menuAnim.setEasingCurve(QEasingCurve::OutSine);

	m_menuHideTimer.setInterval(4000);
	m_menuHideTimer.setSingleShot(true);
	connect(&m_menuHideTimer, SIGNAL(timeout()), SLOT(menuHide()));
	m_menuHideFastTimer.setInterval(1000);
	m_menuHideFastTimer.setSingleShot(true);
	connect(&m_menuHideFastTimer, SIGNAL(timeout()), SLOT(menuHideFast()));

	m_menuVisible = true;

	m_midiConnection->devOpen(m_settings->midiConnection());
	m_midiConnection->setParams(m_settings->midiConnParams());

	m_netcastManager->setConnection(m_settings->netcastConnection(), m_settings->netcastParams());

	progressLoadingScreen(LSPB_INIT);
	return true;
}


bool MidiJ::loadSchemeGui(QString schemeFile)
{
	if (!m_guiMapping.loadScheme(schemeFile, m_guiItems))
		return false;

	foreach (QGraphicsItem *item, m_guiItems)
		m_scene.addItem(item);

	QPixmap background = m_guiMapping.background();
	QRect menuButtonRect = m_guiMapping.toggleButtonRect(WINDOW_BAR_HEIGHT); //(140, 0, 200, 60);

	_delete(m_menuToggleButton);
	m_menuToggleButton = new GraphicsClickItem(background.copy(menuButtonRect), QPixmap(), menuButtonRect);
	m_menuToggleButton->setZValue(0.5);
	m_scene.addItem(m_menuToggleButton);
	connect(m_menuToggleButton, SIGNAL(clicked()), this, SLOT(menuToggle()));

//	m_scene.addItem(m_menuGroup);

	progressLoadingScreen(LSPB_SCHEMEFILE);
	return true;
}


bool MidiJ::loadEngineRouterPreset()
{
	// setup connections:
	// GUI -> MidiController/Engine
	m_guiMapping.connectMidiOutPort(m_midiControler, SLOT(emulReceive(uchar,uchar,uchar)));
	connect(m_midiControler, SIGNAL(emulSendShortMsg(uchar,uchar,uchar)), &m_guiMapping, SLOT(midiShortMsgIn(uchar,uchar,uchar)));
	connect(m_midiControler, SIGNAL(emulSendSysexMsg(uchar*,uint)), &m_guiMapping, SLOT(midiSysexMsgIn(uchar*,uint)), Qt::DirectConnection);


	// MidiController/Engine -> Router -> Gateway
	m_router->setMidiObjects(m_midiControler, m_midiConnection);
	connect(m_midiControler, SIGNAL(midiThru(MidiMessage,char)), m_router, SLOT(midiSourceReceive(MidiMessage,char)));
	connect(m_midiControler->getMidiScriptEngine(), SIGNAL(initialized()), m_router, SLOT(sourceReload()));

	// Gateway -> GUI mapping (SysEx messages from gateway)
	connect(m_midiConnection, SIGNAL(midiSysexMsgOut(uchar*,uint)), &m_guiMapping, SLOT(midiSysexMsgIn(uchar*,uint)), Qt::DirectConnection);

	// GUI mapping -> Router (SysEx messages reply)
	connect(&m_guiMapping, SIGNAL(midiSysexMsgOut(uchar*,uint)), m_router, SLOT(midiGatewaySysexMsgOut(uchar*,uint)), Qt::DirectConnection);


	progressLoadingScreen(LSPB_GUIMAPPING_MIDICONTRL);
	QDomElement engineXml = m_guiMapping.sourcePreset("engine");
	QFileInfo engineFile(MidiMappingGui::sourceFile(engineXml));
	qDebug() << engineFile.absoluteFilePath() << QDir::currentPath();
	if (engineFile.exists())
	{
		DirContext dc(engineFile);
		m_midiControler->getMidiMapping()->loadPreset(engineXml, false);
		progressLoadingScreen(LSPB_GUIMAPPING_ROUTER);
	}
	else
		return false;

	qDebug("router mappings....");
	if (m_router->loadMappings(m_guiMapping.sourcePreset("router"), m_guiMapping.sourcePreset("router.gateway")))
	{
		progressLoadingScreen(LSPB_GUIMAPPING_END);
		return true;
	}

	// fail to load router mappings...
	return false;
}

bool MidiJ::loadSimplePreset()
{
	// setup connections:
	m_router->setMidiObjects(NULL, m_midiConnection);
	// GUI mapping -> MidiConnection
	m_guiMapping.connectMidiOutPort(m_router, SLOT(midiGatewayShortMsgOut(uchar,uchar,uchar)));
	// GUI mapping -> MidiConnection (SysEx messages reply)
	connect(&m_guiMapping, SIGNAL(midiSysexMsgOut(uchar*,uint)), m_router, SLOT(midiGatewaySysexMsgOut(uchar*,uint)), Qt::DirectConnection);

	// MidiConnection -> GUI
	connect(m_midiConnection, SIGNAL(midiThru(MidiMessage,char)), &m_guiMapping, SLOT(midiShortMsgIn(MidiMessage,char)));
	connect(m_midiConnection, SIGNAL(midiSysexMsg(uchar*,uint)), &m_guiMapping, SLOT(midiSysexMsgIn(uchar*,uint)), Qt::DirectConnection);

	progressLoadingScreen(LSPB_GUIMAPPING_END);
	return false;//not yet// true;
}



void MidiJ::loadCurrentScheme()
{
	QFileInfo schemeFile(m_settings->guiScheme());
	DirContext dc(schemeFile);
	bool guiLoaded = loadSchemeGui(schemeFile.fileName());
	if (guiLoaded)
	{
		m_pView->setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
		if (m_guiMapping.hasSourcePreset("engine"))
			loadEngineRouterPreset();
		else
			loadSimplePreset();

		QTimer::singleShot(250, this, SLOT(runScheme()));
    }
    else
    {
		QRect menuButtonRect(QRect(QPoint(0,0), desktopSize()));
		_delete(m_menuToggleButton);
		QPixmap buttonBg(menuButtonRect.size());
		buttonBg.fill(QColor(46,44,45,255));
		m_menuToggleButton = new GraphicsClickItem(buttonBg, QPixmap(), menuButtonRect);
		m_menuToggleButton->setZValue(0.0);
		m_scene.addItem(m_menuToggleButton);
		connect(m_menuToggleButton, SIGNAL(clicked()), this, SLOT(menuToggle()));
		showLoadingScreen(false);
		menuShow();
		QMaemo5InformationBox::information(this, "Fail to load scheme");
		m_menuHideTimer.stop();
    }
}

void MidiJ::runScheme()
{
	m_guiMapping.init();
	showLoadingScreen(false);

	QApplication::instance()->processEvents();

	// if engine, engine->callInit()
	if (m_midiControler != NULL)
	{
		MidiMapping* midiMapping = m_midiControler->getMidiMapping();
		if (midiMapping != NULL)
			midiMapping->callInit();
	}
	if (m_router != NULL)
	{
		RouterMapping* routerMapping = m_router->getRouterMapping();
		if (routerMapping != NULL)
			routerMapping->callInit();
	}

	if (m_midiConnection->getOpenDevice().isEmpty())
	{
		QMaemo5InformationBox::information(this, "Fail to open network MIDI connection");
	}
	if (m_netcastManager->hasError())
	{
		QMaemo5InformationBox::information(this, m_netcastManager->errorMessage());
	}

	QTimer::singleShot(1000, this, SLOT(menuHide()));
}

void MidiJ::doClose()
{
	m_scene.clear();
//    w.setAttribute(Qt::WA_Maemo5LandscapeOrientation, true);
//	QApplication::instance()->processEvents();

	if (m_settings)
		m_settings->close();
	if (m_midiControler)
		m_midiControler->devClose();
	if (m_midiConnection)
		m_midiConnection->devClose();
	if (m_netcastManager)
		m_netcastManager->close();

	close();
}

void MidiJ::doTaskView()
{
	m_menuHideFastTimer.start();
#ifdef Q_WS_WIN
	showMinimized();
#endif // Q_WS_WIN
#ifdef Q_WS_MAEMO_5
	QDBusConnection session = QDBusConnection::sessionBus();
	QDBusMessage msg = QDBusMessage::createSignal("/com/nokia/hildon_desktop", "com.nokia.hildon_desktop", "exit_app_view");
	session.send(msg);
	session.send(msg); // for some reason only 1msg is not enough...
#endif // Q_WS_MAEMO_5
}

//#include <QtGui/QMessageBox>

void MidiJ::doSettings()
{
	menuHideFast();

/*	settings->initScheme(m_defaultSchemeIcon48, m_schemeList, m_currentSchemeId);
	settings->initMidiConnection(*m_midiConnection->getDeviceList(), m_currentMidiConn);
	settings->initNetcast(m_netcastManager->connectionList(), m_currentNetcastConn);*/
	m_settings->execDialog(this);
	if (m_settings->result() == QDialog::Accepted)
	{
		if (m_settings->isMidiConnectionChanged())
		{
			m_midiConnection->reopen(m_settings->midiConnection());
			m_midiConnection->setParams(m_settings->midiConnParams());
		}

		if (m_settings->isGuiSchemeChanged())
		{
//			QMessageBox::information(this, "debug", QString("<font color=white>newScheme: %1<br>newMidiConn: %2</font>").arg(newScheme).arg(newMidiConn));
			showLoadingScreen(true);

			// Unload current scheme
			qDeleteAll(m_guiItems);
			m_guiItems.clear();
			m_guiMapping.clear();
			m_router->clear();
			m_midiControler->disconnect();
			m_midiControler->devClose();
			m_midiControler->getMidiMapping()->clearPreset();

			// load new skin...
			QTimer::singleShot(250, this, SLOT(loadCurrentScheme()));
			progressLoadingScreen(LSPB_INIT);
		}

		if (m_settings->isNetcastConnectionChanged())
		{
			m_netcastManager->setConnection(m_settings->netcastConnection(), m_settings->netcastParams());
		}
	}
}

void MidiJ::menuHide()
{
	if (m_menuVisible && m_menuAnim.state() != QAbstractAnimation::Running)
	{
		m_menuHideTimer.stop();
		m_menuVisible = false;
		m_menuAnim.setEndValue(-WINDOW_BAR_HEIGHT);
		m_menuAnim.start();
	}
}

void MidiJ::menuHideFast()
{
	if (m_menuVisible && m_menuAnim.state() != QAbstractAnimation::Running)
	{
		m_menuVisible = false;
		m_menuGroup->setY(-WINDOW_BAR_HEIGHT);
	}
}

void MidiJ::menuShow()
{
	if (m_menuVisible || m_menuAnim.state() == QAbstractAnimation::Running)
		return;
	m_menuVisible = true;
	m_menuAnim.setEndValue(0);
	m_menuAnim.start();
	m_menuHideTimer.start();
}

void MidiJ::menuToggle()
{
	if (m_menuAnim.state() == QAbstractAnimation::Running)
		return;
	if (m_menuVisible)
		menuHide();
	else
		menuShow();
}



GraphicsProgressItem::GraphicsProgressItem(int max)
{
	m_max = qBound(1, max, 10);
	m_progress = 0;
}


#define GPI_SINGLESTEP_HEIGHT		24
#define GPI_SINGLESTEP_WIDTH		24
#define GPI_SINGLESTEP_WIDTH_XS		(24 + 4)

#define GPI_OUTERCIRCLE_SIZE		22
#define GPI_OUTERCIRCLE_ADJUST		(GPI_SINGLESTEP_WIDTH - GPI_OUTERCIRCLE_SIZE) / 2

#define GPI_INNERCIRCLE_SIZE		16
#define GPI_INNERCIRCLE_ADJUST		(GPI_SINGLESTEP_WIDTH - GPI_INNERCIRCLE_SIZE) / 2


QRectF GraphicsProgressItem::boundingRect() const
{
    return QRectF(0, 0, GPI_SINGLESTEP_WIDTH_XS * m_max, GPI_SINGLESTEP_HEIGHT);
}



void GraphicsProgressItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)
	painter->setRenderHint(QPainter::Antialiasing);
	painter->setPen(Qt::NoPen);

	QLinearGradient gradient(0, 0,GPI_SINGLESTEP_WIDTH_XS * m_max / 2, 0);
	gradient.setSpread(QGradient::ReflectSpread);
	gradient.setColorAt(0.0, Qt::gray);
	gradient.setColorAt(1.0, Qt::white);
	painter->setBrush(gradient);

//	painter->setBrush(QColor(96, 96, 96, 255));
	int i;
	for (i = 0; i < m_progress; i++)
	{
		// inner circle
		painter->drawEllipse(i * GPI_SINGLESTEP_WIDTH_XS + GPI_INNERCIRCLE_ADJUST,
				GPI_INNERCIRCLE_ADJUST,
				GPI_INNERCIRCLE_SIZE, GPI_INNERCIRCLE_SIZE);
	}
	painter->setPen(QPen(Qt::lightGray, 2));
//	painter->setBrush(Qt::white);
	painter->setBrush(Qt::NoBrush);
//	painter->drawRect(0 ,0, 24*m_max, 24);

	for (i = 0; i < m_max; i++)
	{
		// outer circle
		painter->drawEllipse(i * GPI_SINGLESTEP_WIDTH_XS + GPI_OUTERCIRCLE_ADJUST,
				GPI_OUTERCIRCLE_ADJUST,
				GPI_OUTERCIRCLE_SIZE, GPI_OUTERCIRCLE_SIZE);
	}

    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::NoBrush);
}

