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

#ifndef MIDIJ_H
#define MIDIJ_H

#include <QGraphicsView>
#include "gui/MidiMappingGui.h"
#include <qgraphicsitem.h>
#include "NetcastManager.h"
#include "settings/SettingsManager.h"

class MidiObject;
class MidiObjectUdp;
class RouterObject;
class GraphicsClickItem;

// Graphics group with properties for animation
class GraphicsObjectItemGroup : public QObject, public QGraphicsItemGroup
{
	Q_OBJECT
	Q_PROPERTY(QPointF pos READ pos WRITE setPos)
	Q_PROPERTY(qreal x READ x WRITE setX)
	Q_PROPERTY(qreal y READ y WRITE setY)

public:
	GraphicsObjectItemGroup()
	{
		setHandlesChildEvents(false);
		setFiltersChildEvents(false);
	};
	virtual ~GraphicsObjectItemGroup()
	{};
};

// Simple progress bar item
class GraphicsProgressItem : public QGraphicsItem
{
public:
	GraphicsProgressItem(int max);

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	void setProgress(int p)
	{
		m_progress = qBound(0, p, m_max);
	}
	void stepProgress()
	{
		m_progress = qMin(m_progress + 1, m_max);
	}

protected:
	int m_max;
	int m_progress;
};

// Main widget
class MidiJ : public QWidget
{
	Q_OBJECT

	// IMPROVEMENTS:
	// TODO zzzz; o usar QXmlSimpleReader para o xml manager
	// TODO zzzz; simular "multi-touch"

public:
	MidiJ(QWidget *parent = 0);
	virtual ~MidiJ();

	void showLoadingScreen(bool show);
	bool init(QString schemeHomeDir, QString schemeAltDir);

	bool loadSchemeGui(QString schemeFile);
	bool loadEngineRouterPreset();
	bool loadSimplePreset();

public slots:
	void loadCurrentScheme();
	void runScheme();
	void doClose();
	void doTaskView();
	void doSettings();
	void menuHide();
	void menuHideFast();
	void menuShow();
	void menuToggle();

protected:
	void progressLoadingScreen(int pos);
	QSize desktopSize()
	{
		// TODO zzzz; Support landscape mode
		return QSize(480, 800);
	};

private:
	QGraphicsView* m_pView;
	QGraphicsScene m_scene;
	MidiMappingGui m_guiMapping;
	SettingsManager* m_settings;
	RouterObject*  m_router;
	MidiObject*    m_midiControler;
	MidiObjectUdp* m_midiConnection;
	NetcastManager* m_netcastManager;

	// Scheme objects
//	QString                 m_schemeHomeDir;
	QList<QGraphicsItem*>   m_guiItems;

	// System menu
	GraphicsObjectItemGroup* m_menuGroup;
	QPropertyAnimation       m_menuAnim;
	QTimer                   m_menuHideTimer;
	QTimer                   m_menuHideFastTimer;
	bool                     m_menuVisible;
	GraphicsClickItem*       m_menuToggleButton;
	QIcon                    m_defaultSchemeIcon48;

	// Loading screen objects
	QGraphicsItemGroup*   m_loadingGroup;
	GraphicsProgressItem* m_progressItem;
	QRect                 m_progressItemRect;
};

#endif // MIDIJ_H
