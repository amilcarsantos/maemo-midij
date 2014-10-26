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

#include "SettingsDialog.h"
#include "../gui/GuiUtils.h"
#include <QtGui>

SettingsDialog::SettingsDialog()
{
#ifdef Q_WS_MAEMO_5
    setAttribute(Qt::WA_Maemo5PortraitOrientation, true);
#endif // Q_WS_MAEMO_5

	ui.setupUi(this);
	if (true) // TODO zzzz; auto-detect orientation
	{
		// switch to portrait mode
		QHBoxLayout* buttonBoxLayout = new QHBoxLayout();
		ui.buttonBoxSpacer->changeSize(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
		ui.buttonBoxLayout->removeItem(ui.buttonBoxSpacer);
#ifdef BUTTONBOXSPACER
		buttonBoxLayout->addItem(ui.buttonBoxSpacer);
#else
		_delete(ui.buttonBoxSpacer);
#endif
		ui.verticalLayout->removeWidget(ui.buttonBox);
		buttonBoxLayout->addWidget(ui.buttonBox);

		ui.gridLayout->removeItem(ui.buttonBoxLayout);
		_delete(ui.buttonBoxLayout);
		ui.gridLayout->addItem(buttonBoxLayout, 1, 0, 1, 1);
	}
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::launchHomepage()
{
	QDesktopServices::openUrl(QUrl(ui.homepageButton->valueText()));
	reject();
}

QMaemo5ValueButton* SettingsDialog::buttonFromName(QString name)
{
	return findChild<QMaemo5ValueButton*>(name);
}

void SettingsDialog::settingsSwap()
{
	setSwapSettings(true);
	reject();
}

