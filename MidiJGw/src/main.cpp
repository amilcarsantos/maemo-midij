/*
 * MidiJGw - MIDI gateway for MidiJ application
 * Copyright (C) 2011  Amilcar Santos <amilcar.santos@gmail.com>
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

#include "MidiJGw.h"

#include <QtGui>
#include <QtSingleApplication>

// FIXME novo icon...
int main(int argc, char *argv[])
{
	QtSingleApplication a(argc, argv);
	QCoreApplication::setOrganizationName("Amilcar Santos");
	QCoreApplication::setApplicationName("MidiJ Gateway");

	if (a.isRunning())
	{
		a.sendMessage("awakeWnd");
    	return 0;
	}
    MidiJGw w;
    a.setActivationWindow(&w, false);

    w.show();

    w.connect(&a, SIGNAL(messageReceived(QString)), SLOT(instanceMessage(QString)));

	if (!QSystemTrayIcon::isSystemTrayAvailable())
	{
		QMessageBox::critical(0, QObject::tr("Systray"),
				QObject::tr("I couldn't detect any system tray on this system."));
		return 1;
	}
	QApplication::setQuitOnLastWindowClosed(false);

    return a.exec();
}

