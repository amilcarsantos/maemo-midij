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

#ifndef SETUPDIALOG_H_
#define SETUPDIALOG_H_

#include "ui_SettingsDialog.h"
#include "../gui/MidiMappingGui.h"
#include "SettingsWnd.h"

class QMaemo5ListPickSelector;

class SettingsDialog : public QDialog, public SettingsWnd
{
	Q_OBJECT

public:
	SettingsDialog();
	virtual ~SettingsDialog();

public slots:
	void launchHomepage();
	void settingsSwap();

protected:
	virtual QMaemo5ValueButton* buttonFromName(QString name);

private:
    Ui::SettingsDialogClass ui;
};

#endif /* SETUPDIALOG_H_ */
