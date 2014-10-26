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

#include "LightLine.h"

#include <QPainter>

LightLine::LightLine(QWidget* parent, Qt::WindowFlags f)
	: QFrame(parent, f)
{
	m_light = Qt::color0;
}

LightLine::~LightLine()
{
}

void LightLine::paintEvent(QPaintEvent *event)
{
	QPainter p(this);
//    QStyleOptionFrameV3 opt;
//    opt.init(this);
//    int frameShape  = d->frameStyle & QFrame::Shape_Mask;
//    int frameShadow = d->frameStyle & QFrame::Shadow_Mask;
//    opt.frameShape = Shape(int(opt.frameShape) | frameShape);
//    opt.rect = frameRect();
//    switch (frameShape) {
//        case QFrame::Box:
//        case QFrame::HLine:
//        case QFrame::VLine:
//        case QFrame::StyledPanel:
//        case QFrame::Panel:
//            opt.lineWidth = d->lineWidth;
//            opt.midLineWidth = d->midLineWidth;
//            break;
//        default:
//            // most frame styles do not handle customized line and midline widths
//            // (see updateFrameWidth()).
//            opt.lineWidth = d->frameWidth;
//            break;
//    }
//
//    if (frameShadow == Sunken)
//        opt.state |= QStyle::State_Sunken;
//    else if (frameShadow == Raised)
//        opt.state |= QStyle::State_Raised;
//    style()->drawControl(QStyle::CE_ShapedFrame, &opt, p, this);

	QPalette shadePalette(palette());
	if (m_light > Qt::color0)
		shadePalette.setColor(QPalette::Mid, m_light);

	QRect rect = frameRect();
    QPoint p1, p2;
    if (frameShape() == QFrame::HLine) {
        p1 = QPoint(rect.x(), rect.height() / 2);
        p2 = QPoint(rect.x() + rect.width(), p1.y());
    } else {
        p1 = QPoint(rect.x()+rect.width() / 2, 0);
        p2 = QPoint(p1.x(), rect.height());
    }

	qDrawShadeLine(&p, p1, p2, shadePalette, frameShadow() == QFrame::Sunken, lineWidth(), midLineWidth());

//	QFrame::paintEvent(event);
}
