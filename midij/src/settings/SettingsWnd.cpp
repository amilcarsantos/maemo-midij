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

#include "SettingsWnd.h"
#include "../gui/GuiUtils.h"
#include <QtGui>

SettingsWnd::SettingsWnd()
{
	m_swapSettings = false;
	m_schemeSelector = NULL;
}

SettingsWnd::~SettingsWnd()
{
}


void SettingsWnd::initScheme(QIcon icon, const QList<SchemeGuiHeader*>& schemeList, const QString& currentScheme)
{
	QStandardItemModel *model = new QStandardItemModel(0, 1);
	QStandardItem* selectedItem = NULL;
	foreach(SchemeGuiHeader* scheme, schemeList)
	{
    	QStandardItem *item = new QStandardItem(scheme->description());
    	item->setIcon(scheme->icon48(icon));
    	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    	item->setData(scheme->id(), Qt::UserRole);
    	model->appendRow(item);
    	if (scheme->id() == currentScheme)
    		selectedItem = item;
	}
	model->sort(0);
	m_schemeSelector = new QMaemo5ListPickSelector();
	m_schemeSelector->setModel(model);
	m_schemeSelector->setCurrentIndex(selectedItem == NULL? 0: selectedItem->row());
    buttonFromName("schemeButton")->setPickSelector(m_schemeSelector);
}


QString SettingsWnd::guiScheme()
{
	if (m_schemeSelector)
	{
		QModelIndex currentIndex = m_schemeSelector->model()->index(m_schemeSelector->currentIndex(), 0);
		if (currentIndex.isValid())
			return m_schemeSelector->model()->data(currentIndex, Qt::UserRole).toString();
	}
	return QString();
}

int initButtonModel(QStandardItemModel* model, const QStringList& nameList, const QString& currentItemName)
{
	int currentIndex = 0;
	foreach (QString itemName, nameList)
	{
		QStandardItem *item = new QStandardItem(itemName);
		item->setTextAlignment(Qt::AlignCenter); // the Maemo 5 design spec recommends this.
		item->setEditable(false); // prevent editing of the item
		model->appendRow(item);
		if (currentItemName == itemName)
			currentIndex = item->row();
	}
	return currentIndex;
}

void initSelector(QStandardItemModel* model, int currentIndex, QMaemo5ValueButton* button)
{
	QMaemo5ListPickSelector *selector1 = new QMaemo5ListPickSelector();
	selector1->setModel(model);
	selector1->setCurrentIndex(currentIndex);
	// not setting the current index means that the value is empty.
	button->setPickSelector(selector1);
}


void SettingsWnd::initMidiConnection(const QStringList& connections, const QString& currentConnection)
{
	QMaemo5ValueButton* midiConnButton = buttonFromName("midiConnButton");
	QStandardItemModel* model = new QStandardItemModel(0, 1, midiConnButton);
	int currentIndex = initButtonModel(model, connections, currentConnection);

	initSelector(model, currentIndex, midiConnButton);

#ifdef __MIDIUDP_LOCALHOST__
	midiConnButton->setText("MIDIUDP_LOCALHOST");
#endif
}

QString SettingsWnd::midiConnection()
{
	return buttonFromName("midiConnButton")->valueText();
}

void SettingsWnd::initMidiConnHost(const QStringList& ipList, const QString& currentIp)
{
	QMaemo5ValueButton* midiConnIpButton = buttonFromName("midiConnIpButton");
	if (midiConnIpButton == NULL)
		return;

	QStandardItemModel* model = new QStandardItemModel(0, 1, midiConnIpButton);
	int currentIndex = initButtonModel(model, ipList, currentIp);

	initSelector(model, currentIndex, midiConnIpButton);
}

QString SettingsWnd::midiConnHost()
{
	return buttonFromName("midiConnIpButton")->valueText();
}

void SettingsWnd::initNetcast(const QStringList& connections, const QString& currentConnection)
{
	QMaemo5ValueButton* netcastButton = buttonFromName("netcastButton");
	QStandardItemModel* model = new QStandardItemModel(0, 1, netcastButton);
	int currentIndex = initButtonModel(model, connections, currentConnection);

	initSelector(model, currentIndex, netcastButton);
}

QString SettingsWnd::netcastConnection()
{
	return buttonFromName("netcastButton")->valueText();
}

void SettingsWnd::initNetcastHost(const QStringList& ipList, const QString& currentIp)
{
	QMaemo5ValueButton* netcastIpButton = buttonFromName("netcastIpButton");
	if (netcastIpButton != NULL)
	{
		QStandardItemModel* model = new QStandardItemModel(0, 1, netcastIpButton);
		int currentIndex = initButtonModel(model, ipList, currentIp);
		initSelector(model, currentIndex, netcastIpButton);
	}
}

QString SettingsWnd::netcastHost()
{
	QMaemo5ValueButton* button = buttonFromName("netcastIpButton");
	if (button != NULL)
	{
		return button->valueText();
	}
	return QString();
}


void SettingsWnd::initNetcastBuffer(const QStringList& bufferList, const QString& currentBuffer)
{
	QMaemo5ValueButton* netcastBufferButton = buttonFromName("netcastBufferButton");
	if (netcastBufferButton != NULL)
	{
		QStandardItemModel* model = new QStandardItemModel(0, 1, netcastBufferButton);
		int currentIndex = initButtonModel(model, bufferList, currentBuffer);

		initSelector(model, currentIndex, netcastBufferButton);
	}
}

QString SettingsWnd::netcastBuffer()
{
	QMaemo5ValueButton* button = buttonFromName("netcastBufferButton");
	if (button != NULL)
	{
		return button->valueText();
	}
	return QString();
}

void SettingsWnd::setSwapSettings(bool flag)
{
	m_swapSettings = flag;
}


bool SettingsWnd::swapSettings()
{
	return m_swapSettings;
}

