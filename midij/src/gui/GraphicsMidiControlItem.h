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

#ifndef GRAPHICSMIDICONTROLITEM_H_
#define GRAPHICSMIDICONTROLITEM_H_

#include <QtGui/QGraphicsPixmapItem>
#include <qdom.h>
#include "../mixxx/MidiMessage.h"
#include <QtCore/qobject.h>
#include <QMetaObject>
#include <QMetaProperty>


struct GraphicsMidiArea
{
	QRect rect;
	QPainterPath path;	// Path within the <code>rect</code>

	inline bool isNull()
	{
		return rect.isNull();
	};
};


#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug, const GraphicsMidiArea &);
#endif


// Generic MIDI control handler
////////////////////////////////////////////////////////
class MidiOutputController
{
    Q_GADGET
    Q_ENUMS(ControlType)
public:

	enum ControlType {
		Button,
		SliderAbsolute,
		SliderRelative,
		WheelAbsolute,
		WheelRelative
	};
	enum EventType
	{
		Pressed,
		Move,
		Release
	};

	MidiOutputController(const MidiMessage& outputMidiMessage)
	{
		m_outputMidiMessage = outputMidiMessage;
	};
	virtual ~MidiOutputController()
	{};

	static QMetaEnum enumerator(const char* enumName)
	{
		const QMetaObject &mo = staticMetaObject;
		return mo.enumerator(mo.indexOfEnumerator(enumName));
	};

	// Methods to override
public:
	virtual int midiStatusByte(EventType type) = 0;
	virtual int midiByte2(EventType type, QPointF pos) = 0;
	virtual bool moveAfterPressed() = 0;
	virtual ControlType controlType() = 0;

protected:
	MidiMessage m_outputMidiMessage;
};



#define INVALID_STATE_ID	0xFFFF

#define DRAWGROUP_DATAID	0xD2A0

// XML tags
#define CONTROL_NAME		"name"
#define CONTROL_GROUP		"group"
#define CONTROL_DRAWGROUP	"draw"


class PixmapMap : public QMap<QString, QPixmap>
{
public:
	PixmapMap() {};
	~PixmapMap() {};

	QPixmap load(const QString& name, const QString& fileName);
	QImage sourceImage(const QString& name) const;

private:
	QMap<QString, QImage> m_imageMap;
};

inline QPixmap PixmapMap::load(const QString& name, const QString& fileName)
{
	QImage imageFile(fileName);
	if (imageFile.isNull())
		return QPixmap();
	m_imageMap.insert(name, imageFile);
	QPixmap pixmap = QPixmap::fromImage(imageFile);
	insert(name, pixmap);
	return pixmap;
}

inline QImage PixmapMap::sourceImage(const QString& name) const
{
	return m_imageMap.value(name);
}



// Generic MIDI GUI control
////////////////////////////////////////////////////////
class GraphicsMidiControlItem : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
	Q_PROPERTY(bool visible READ isVisible WRITE setVisible)

public:
	GraphicsMidiControlItem();
	virtual ~GraphicsMidiControlItem();

	enum { Type = QGraphicsItem::UserType + 1 };
	int type() const;

	QList<MidiMessage> inputMidiMessages() const;
	bool hasInput();
	bool hasOutput();
	bool isValid();

	QString name();
	QString groupName();

	ushort currentState();

	QRectF sceneRect();
	void setSceneRect(const QRect& rect);
signals:
	// statusByte - includes category and channel
	void midiShortMsgOut(uchar statusByte, uchar midino, uchar midivalue);
	void visibleChanged();

public:
	static GraphicsMidiArea parseAreaElement(const QDomElement& areaParentNode);
	static GraphicsMidiArea parseAreaAttribute(const QDomElement& node);
	static QPixmap pixmapFromMap(const QString& name, QRect sourceRect, const PixmapMap& pixmapMap, const QString& mask);
	static QString parsePixmapMask(const QDomElement& areaParentNode);
	static QImage createMaskFromColors(const QImage& image, const QSet<QRgb>& colors, Qt::MaskMode mode);
	static QImage createAlphaFromColors(const QImage& image, const QMap<QRgb, uchar>& colors2Alpha, Qt::MaskMode mode);

	virtual void receivedMidiShortMsg(const MidiMessage& msg, uchar midivalue);
	void setOffState();


protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void propagateEvent(QGraphicsSceneMouseEvent* event);

	static void checkDebug(QString debugColor, QPixmap& pixmap, const GraphicsMidiArea& area);

	// with flags: merge a Input -> InputOutput
	bool m_hasInput;
	bool m_hasOutput;
	MidiMessage m_outputMidiMessage;
	QList<MidiMessage> m_inputMidiMessages;
	QString m_name;
	QString m_groupName;
	QString m_drawGroupName;
	ushort m_currentState;
	ushort m_offState;
	QHash<ushort, QPixmap> m_state2Pixmap;
	QRect m_sceneRect;
	QRectF m_sceneRectF;
	QMap<ushort, ushort> m_range2State; // Key: MAX range; Value: MIN range and stateKey in m_state2Pixmap;
	MidiOutputController* m_outputController;

	friend class GraphicsMidiInputItem;
};

inline bool GraphicsMidiControlItem::hasInput()
{
	return m_hasInput;
}

inline bool GraphicsMidiControlItem::hasOutput()
{
	return m_hasOutput;
}

inline bool GraphicsMidiControlItem::isValid()
{
	return m_hasInput || m_hasOutput;
}

inline QString GraphicsMidiControlItem::name()
{
	return m_name;
}

inline QString GraphicsMidiControlItem::groupName()
{
	return m_groupName;
}

inline ushort GraphicsMidiControlItem::currentState()
{
	return m_currentState;
}

inline void GraphicsMidiControlItem::setSceneRect(const QRect& rect)
{
	m_sceneRect = rect;
	m_sceneRectF = QRectF(rect);
}
inline QRectF GraphicsMidiControlItem::sceneRect()
{
	return m_sceneRectF;
}

#endif /* GRAPHICSMIDICONTROLITEM_H_ */
