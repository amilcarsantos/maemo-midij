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

#ifndef ADVANCEDSETTINGSWND_H
#define ADVANCEDSETTINGSWND_H

#include <QtGui/QWidget>
#include "ui_AdvancedSettingsWnd.h"
#include "SettingsWnd.h"

class AdvancedSettingsWnd : public QWidget, public SettingsWnd
{
    Q_OBJECT

public:
    AdvancedSettingsWnd(QWidget *parent = 0);
    ~AdvancedSettingsWnd();

signals:
	void closing();

public slots:
	void accept();
	void launchHomepage();
	void settingsSwap();

public:
	void exec();
	int result();

protected:
	virtual void closeEvent(QCloseEvent *);
    virtual QMaemo5ValueButton* buttonFromName(QString name);

private:
    Ui::AdvancedSettingsWndClass ui;
    int m_result;
};

inline int AdvancedSettingsWnd::result()
{
	return m_result;
}

#endif // ADVANCEDSETTINGSWND_H
