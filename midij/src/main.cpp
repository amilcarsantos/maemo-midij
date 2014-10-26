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

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QCoreApplication::setOrganizationName("MidiJ");
	QCoreApplication::setApplicationName("MidiJ");

	MidiJ w;

#ifdef Q_WS_WIN
    w.setWindowTitle(QString::fromUtf8("MidiJ"));
    w.resize(480, 800);
    QString schemeDir = "data/schemes/";
    QString altSchemeDir = "data/userSchemes/MidiJ_SSL_V1.0/midij_schemes";
    w.setWindowIcon(QIcon("icon/64x64/midij.png"));
#endif // Q_WS_WIN

#ifdef Q_WS_MAEMO_5
    QApplication::setOverrideCursor(QCursor(Qt::BlankCursor));
    w.setAttribute(Qt::WA_Maemo5PortraitOrientation, true);
    w.setWindowState(Qt::WindowFullScreen);
    QString schemeDir = "/opt/midij/schemes/";
    QString altSchemeDir = "/home/user/MyDocs/.midij_schemes/";
#endif // Q_WS_MAEMO_5

    w.showLoadingScreen(true);
	w.show();
	a.processEvents(QEventLoop::ExcludeUserInputEvents);
	if (!w.init(schemeDir, altSchemeDir))
		return 0;

	QTimer::singleShot(250, &w, SLOT(loadCurrentScheme()));
/// *
//void NetworkApplication::started()
//{
//    QCoreApplication::processEvents();
//    onlineStateChanged(configManager->isOnline());
//    connect(configManager, SIGNAL(onlineStateChanged(bool)),
//            this, SLOT(onlineStateChanged(bool)));
//}
// * /

    return a.exec();
}
