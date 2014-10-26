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

#include "GraphicsMidiControlItem.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QGraphicsView>
//#include <QStyle>
#include <QBitmap>
#include "GuiUtils.h"


// XML tags
#define CONTROL_AREA			"area"
#define CONTROL_AREA_TYPE		"type"
#define CONTROL_AREA_TYPESIZE	"size"
#define CONTROL_AREA_MASK		"mask"



#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const GraphicsMidiArea &r) {
    dbg.nospace() << "GraphicsMidiArea(" << r.rect << ')';
    return dbg.space();
}
#endif


GraphicsMidiControlItem::GraphicsMidiControlItem()
{
	setCacheMode(DeviceCoordinateCache);
	m_hasInput = false;
	m_hasOutput = false;
	m_currentState = INVALID_STATE_ID;
	m_offState = 0;
	m_outputController = NULL;
}

GraphicsMidiControlItem::~GraphicsMidiControlItem()
{
	if (m_outputController != NULL)
		delete m_outputController;
}

int GraphicsMidiControlItem::type() const
{
	return GraphicsMidiControlItem::Type;
}


/**
 * Draws a border with a specified color if <code>debugColor</code> is a valid color
 */
void GraphicsMidiControlItem::checkDebug(QString debugColor, QPixmap& pixmap, const GraphicsMidiArea& area)
{
	if (debugColor.isEmpty())
	{
		if (GuiUtils::debugColor.isEmpty())
			return;
		debugColor = GuiUtils::debugColor;
	}
	QColor penColor(debugColor);
	if (!penColor.isValid())
		return;

	QPixmap tmp = pixmap;
	QRect rect = pixmap.rect();
	QPainter dc(&tmp);
	dc.setPen(penColor);
	if (area.path.isEmpty())
		dc.drawRect(0,0,rect.width()-1, rect.height()-1);
	else
		dc.drawPath(area.path);
	pixmap = tmp;
}

/**
 * Parses the area from a <AREA> Xml tag
 */
GraphicsMidiArea GraphicsMidiControlItem::parseAreaElement(const QDomElement& areaParentNode)
{
	GraphicsMidiArea area;
	QDomElement areaNode = areaParentNode.firstChildElement(CONTROL_AREA);
	QString areaCoords = areaNode.text();

	QStringList coords = areaCoords.split(",");
	if (coords.size() != 4)
	{
		qCritical() << "invalid area size:" << areaCoords << "; line:" << areaNode.lineNumber();
		return area;
	}
	area.rect = QRect(coords.at(0).toInt(), coords.at(1).toInt(), coords.at(2).toInt(), coords.at(3).toInt());

	QString areaType = areaNode.attribute(CONTROL_AREA_TYPE);
	if (areaType.startsWith("Wheel"))
	{
		QRect rect = area.rect;
		rect.moveTo(0, 0);
		area.path.setFillRule(Qt::OddEvenFill);
		area.path.addEllipse(rect);

		int size = GuiUtils::toIntBound(areaNode.attribute(CONTROL_AREA_TYPESIZE), 1, rect.height() / 2, rect.height() / 5);
		rect.adjust(size, size, -size,-size);
		area.path.addEllipse(rect);
	}
	return area;
}

GraphicsMidiArea GraphicsMidiControlItem::parseAreaAttribute(const QDomElement& node)
{
	GraphicsMidiArea area;
	QString areaCoords = node.attribute(CONTROL_AREA);
	if (areaCoords.isNull())
		return area;
	QStringList coords = areaCoords.split(",");
	if (coords.size() != 4)
	{
		qCritical() << "invalid area size:" << area << "; line:" << node.lineNumber();
		return area;
	}
	area.rect = QRect(coords.at(0).toInt(), coords.at(1).toInt(), coords.at(2).toInt(), coords.at(3).toInt());
	return area;
}

QPixmap GraphicsMidiControlItem::pixmapFromMap(const QString& name, QRect sourceRect, const PixmapMap& pixmapMap, const QString& mask)
{
	if (!pixmapMap.contains(name))
	{
		if (name.indexOf(".") > 0)
			return QPixmap(name).copy(QRect(QPoint(), sourceRect.size()));
		qWarning() << "pixmap with name" << name << "not found";
		return QPixmap(sourceRect.size());
	}
	QPixmap pixmap = pixmapMap.value(name).copy(sourceRect);
	if (mask.isEmpty())
		return pixmap;

	QColor color = QColor(mask);

	int r = color.red();
	int g = color.green();
	int b = color.blue();

	int i = 4; // color range (-2..2)
//	QSet<QRgb> colors;
//
//	for(int dr = -i; dr <=i; dr++)
//		for(int dg = -i; dg <=i; dg++)
//			for(int db = -i; db <=i; db++)
//				colors.insert(qRgb(r + dr, g + dg, b + db));
//
//
//	QImage maskImg = createMaskFromColors(pixmap.toImage(), colors, Qt::MaskInColor);
//	pixmap.setMask(QBitmap::fromImage(maskImg));

	uchar alpha = 0;
	QMap<QRgb, uchar> colors2Alpha;
	for(int dr = -i; dr <=i; dr++)
		for(int dg = -i; dg <=i; dg++)
			for(int db = -i; db <=i; db++)
			{
				alpha = 0; //qAbs(r - dr) + qAbs(g - dg) + qAbs(b - db) * 36;
				colors2Alpha.insert(qRgb(r + dr, g + dg, b + db), alpha);
			}
	QImage maskImg = createAlphaFromColors(pixmapMap.sourceImage(name).copy(sourceRect), colors2Alpha, Qt::MaskInColor);
	pixmap.setAlphaChannel(QPixmap::fromImage(maskImg));

	return pixmap;
}

QString GraphicsMidiControlItem::parsePixmapMask(const QDomElement& areaParentNode)
{
	QDomElement areaNode = areaParentNode.firstChildElement(CONTROL_AREA);

	QString mask = areaNode.attribute(CONTROL_AREA_MASK, "");
	if (mask.isEmpty())
		return "";
	// validate mask....
	if (QColor(mask).isValid())
		return mask;
	return "";
}


QImage GraphicsMidiControlItem::createMaskFromColors(const QImage& image, const QSet<QRgb>& colors, Qt::MaskMode mode)
{
    if (image.isNull())
        return QImage();
    QImage maskImage(image.size(), QImage::Format_MonoLSB);
    maskImage.fill(0);
    uchar *s = maskImage.bits();

    if (image.depth() == 32) {
        for (int h = 0; h < image.height(); h++) {
            const uint *sl = (uint *)image.scanLine(h);
            for (int w = 0; w < image.width(); w++) {
                if (colors.contains(sl[w]))
                    *(s + (w >> 3)) |= (1 << (w & 7));
            }
            s += maskImage.bytesPerLine();
        }
    } else {
        for (int h = 0; h < image.height(); h++) {
            for (int w = 0; w < image.width(); w++) {
                if (colors.contains((uint) image.pixel(w, h)))
                    *(s + (w >> 3)) |= (1 << (w & 7));
            }
            s += maskImage.bytesPerLine();
        }
    }
    if  (mode == Qt::MaskOutColor)
        maskImage.invertPixels();
    return maskImage;
}



QImage GraphicsMidiControlItem::createAlphaFromColors(const QImage& image, const QMap<QRgb, uchar>& colors2Alpha, Qt::MaskMode mode)
{
    if (image.isNull())
        return QImage();
    QImage maskImage(image.size(), QImage::Format_Indexed8);
    //maskImage.fill(0);
    uchar *s = maskImage.bits();

    if (image.depth() == 32) {
        for (int h = 0; h < image.height(); h++) {
            const uint *sl = (uint *)image.scanLine(h);
            for (int w = 0; w < image.width(); w++) {
            	*(s + w) = colors2Alpha.value(sl[w], 255);
            }
            s += maskImage.bytesPerLine();
        }
    } else {
        for (int h = 0; h < image.height(); h++) {
            for (int w = 0; w < image.width(); w++) {
            	*(s + w) = colors2Alpha.value((uint) image.pixel(w, h), 255);
            }
            s += maskImage.bytesPerLine();
        }
    }
    if  (mode == Qt::MaskOutColor)
        maskImage.invertPixels();
    return maskImage;
}


void GraphicsMidiControlItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (!hasOutput() || !m_outputController)
		return;

	int byte2 = m_outputController->midiByte2(MidiOutputController::Pressed, event->pos());
	if (byte2 < 0)
	{
		// if Press event is not supported => ignore next events
		event->setAccepted(false);
		return;
	}
	qDebug() << "GraphicsMidiControlItem mousePressEvent:" << m_name << "; midi byte2:" << byte2;

	emit(midiShortMsgOut(m_outputController->midiStatusByte(MidiOutputController::Pressed),
			m_outputMidiMessage.getMidiNo(),
			byte2));

	if (m_outputController->moveAfterPressed())
	{
		emit(midiShortMsgOut(m_outputController->midiStatusByte(MidiOutputController::Move),
				m_outputMidiMessage.getMidiNo(),
				m_outputController->midiByte2(MidiOutputController::Move, event->pos())));
	}
}

void GraphicsMidiControlItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (!hasOutput() || !m_outputController)
		return;

	// check for "touch extensions" / outside of item
	if (!m_sceneRectF.contains(event->scenePos()))
	{
		event->setAccepted(false);
		if (!m_drawGroupName.isEmpty())
		{
			QList<QGraphicsItem*> itemList = scene()->items(event->scenePos(), Qt::IntersectsItemShape, Qt::AscendingOrder);
			foreach(QGraphicsItem* item, itemList)
			{
				// don't propagate event to background
				if ((item->zValue() > 0) && (item != this))
				{
					if (item->data(DRAWGROUP_DATAID).toString() != m_drawGroupName)
						continue;

					QGraphicsSceneMouseEvent relEvent(QEvent::GraphicsSceneMouseRelease);
					relEvent.setScenePos(event->scenePos());
					relEvent.setPos(event->pos());
					scene()->sendEvent(this, &relEvent);
					ungrabMouse();

					item->grabMouse();
					QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
					pressEvent.setButton(event->button());
					pressEvent.setScreenPos(event->screenPos());
					pressEvent.setScenePos(event->scenePos());
					pressEvent.setPos(event->scenePos() - item->scenePos());
					scene()->sendEvent(item, &pressEvent);
					return;
				}
			}
		}
		return;
	}

	int byte2 = m_outputController->midiByte2(MidiOutputController::Move, event->pos());
	if (byte2 < 0)
		return;
	qDebug() << "GraphicsMidiControlItem mouseMoveEvent:" << m_name << "; midi byte2:" << byte2;
	emit(midiShortMsgOut(m_outputController->midiStatusByte(MidiOutputController::Move),
			m_outputMidiMessage.getMidiNo(),
			byte2));

}

void GraphicsMidiControlItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (!hasOutput() || !m_outputController)
		return;

	if (!m_drawGroupName.isEmpty())
		ungrabMouse();

	int byte2 = m_outputController->midiByte2(MidiOutputController::Release, event->pos());
	if (byte2 < 0)
		return;
	qDebug() << "GraphicsMidiControlItem mouseReleaseEvent:" << m_name << "; midi byte2:" << byte2;
	emit(midiShortMsgOut(m_outputController->midiStatusByte(MidiOutputController::Release),
			m_outputMidiMessage.getMidiNo(),
			byte2));
}

QList<MidiMessage> GraphicsMidiControlItem::inputMidiMessages() const
{
    return m_inputMidiMessages;
}



//
// Handles messages sent by the Script engine
void GraphicsMidiControlItem::receivedMidiShortMsg(const MidiMessage& msg, uchar midivalue)
{
	if (!hasInput() || !isVisible())
	{
		if (!hasInput()) qWarning() << "GraphicsMidiControlItem" << m_name << "invalid input message:" << msg.toString();
		return;
	}
	qDebug() << "Update" << m_name << "; midivalue:" << midivalue << "; old:" << m_currentState;

	if (m_currentState == midivalue)
		return;

	if (m_range2State.size() > 0)
	{
		 QMapIterator<ushort, ushort> i(m_range2State);
		 while (i.hasNext())
		 {
		     i.next();
//		     cout << i.key() << ": " << i.value() << endl;
		     if (midivalue <= i.key()) // MAX range
		     {
		    	 midivalue = i.value(); // MIN range <=> state
		    	 qDebug() << "ranged midivalue to state:" << midivalue;
		    	 break;
		     }
		 }
	}

	if (m_state2Pixmap.contains(midivalue))
	{
		QPixmap p = m_state2Pixmap.value(midivalue);
		m_currentState = midivalue;

		if (p.cacheKey() != pixmap().cacheKey())
		{
			setPixmap(p);
			foreach(QGraphicsView* view, scene()->views())
			{
				// force paintEvent() in QGraphicsView....
				view->viewport()->repaint(m_sceneRect);
			}
		}
		return;
	}

	qWarning() << "GraphicsMidiControlItem: UNHANDLED MIDI VALUE >>>>>>" << m_name << "; midivalue:" << midivalue;
}

void GraphicsMidiControlItem::setOffState()
{
	if (!hasInput())
		return;

	if (m_offState == m_currentState)
		return;

	if (!m_state2Pixmap.contains(m_offState))
	{
		qWarning() << "GraphicsMidiControlItem" << m_name << "missing off state:" << m_offState;
		return;
	}
	setPixmap(m_state2Pixmap.value(m_offState));
	m_currentState = m_offState;
}

