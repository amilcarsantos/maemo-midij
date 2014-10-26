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

#include "AdvancedSettingsWnd.h"
#include <QtGui>
#include "../Utils.h"

AdvancedSettingsWnd::AdvancedSettingsWnd(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
#ifdef Q_WS_MAEMO_5
	setWindowFlags(windowFlags() | Qt::Window);
//	setAttribute(Qt::WA_Maemo5StackedWindow, true);
	// workaround for stacked window not working correclty:
	setAttribute(Qt::WA_Maemo5PortraitOrientation, true);
	setWindowModality(Qt::WindowModal);
#else
    setParent(NULL);
#endif
	if (true) // TODO zzzz; auto-detect orientation
	{
		// switch to portrait mode
		QHBoxLayout* buttonBoxLayout = new QHBoxLayout();
		ui.buttonBoxSpacer->changeSize(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
		ui.buttonBoxLayout->removeItem(ui.buttonBoxSpacer);
		_delete(ui.buttonBoxSpacer);
		ui.verticalLayout->removeWidget(ui.buttonBox);
		buttonBoxLayout->addWidget(ui.buttonBox);

		ui.gridLayout->removeItem(ui.buttonBoxLayout);
		_delete(ui.buttonBoxLayout);
		ui.gridLayout->addItem(buttonBoxLayout, 1, 0, 1, 1);
	}
    m_result = QDialog::Rejected;
}

AdvancedSettingsWnd::~AdvancedSettingsWnd()
{
}


QMaemo5ValueButton* AdvancedSettingsWnd::buttonFromName(QString name)
{
	return findChild<QMaemo5ValueButton*>(name);
}

void AdvancedSettingsWnd::closeEvent(QCloseEvent *event)
{
	qDebug("AdvancedSettingsWnd::closeEvent\n");
	emit(closing());
	return QWidget::closeEvent(event);
}


void AdvancedSettingsWnd::exec()
{
	// simulate has if this is a dialog window
	QEventLoop loop;
	connect(this, SIGNAL(closing()), &loop, SLOT(quit()), Qt::QueuedConnection);

	show();
	loop.exec();
}

void AdvancedSettingsWnd::accept()
{
	m_result = QDialog::Accepted;
	close();
}


void AdvancedSettingsWnd::launchHomepage()
{
	QDesktopServices::openUrl(QUrl(ui.homepageButton->valueText()));
	close();
}

void AdvancedSettingsWnd::settingsSwap()
{
	setSwapSettings(true);
	close();
}
