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

#include "MidiMappingGui.h"
#include "GraphicsMidiInputItem.h"
#include "GraphicsMidiOutputItem.h"
#include "GraphicsMidiMultiItem.h"
#include <qlist.h>
#include "GuiUtils.h"
#include "ACMidiDefines.h"
#include "MidiSysexAction.h"
//#include <MidiJ.h>

QByteArray& operator<<(QByteArray& ba, int n)
{
	ba.append(n);
	return ba;
}

QByteArray& operator<<(QByteArray& ba, const QByteArray& bn)
{
	ba.append(bn);
	return ba;
}

#define SCHEME_DESCTIPTION		"Description"
#define SCHEME_ICON48			"Icon48"
#define SCHEME_TOGGLEBUTTONAREA	"ToggleArea"
#define SCHEME_SOURCES_TAG		"Sources"
#define SCHEME_SOURCE			"source"



SchemeGuiHeader::SchemeGuiHeader(QString schemeFile)
{
	QDomElement root = MidiMappingGui::openXMLFile(schemeFile, "schemeGuiHeader");

	if (root.isNull())
		return;
	m_id = QFileInfo(schemeFile).absoluteFilePath();
	m_description = root.firstChildElement(SCHEME_DESCTIPTION).text();
	QFileInfo iconFile(root.firstChildElement(SCHEME_ICON48).text());
	if (iconFile.exists())
		m_iconFilePath = iconFile.absoluteFilePath();
}
/*
SchemeGuiHeader::SchemeGuiHeader(QString id, QString description)
{
	m_id = id;
	m_description = description;
}
*/
SchemeGuiHeader::~SchemeGuiHeader()
{
}

QIcon SchemeGuiHeader::icon48(const QIcon& defaultIcon)
{
	if (!m_icon.isNull())
		return m_icon;
	if (!m_iconFilePath.isEmpty())
	{
		QPixmap pix(m_iconFilePath);
		m_iconFilePath.clear();
		if (!pix.isNull())
		{
			m_icon.addPixmap(pix);
			return m_icon;
		}
	}
	return defaultIcon;
}




MidiMappingGui::MidiMappingGui()
{
	m_receiver = NULL;
	m_receiverMember = NULL;
	m_minSubIdLevel = 0;
	m_maxSubIdLevel = 4;

	// System Messages
	m_controlMessages.insert(MidiMessage(MIDI_STATUS_CC, MIDI_CC_ALL_NOTES_OFF, 0), "ctrlAllNotesOff");


	/*
	 * --> Here is the Identity Request message:
		0xF0  SysEx
		0x7E  Non-Realtime
		0x7F  The SysEx channel. Could be from 0x00 to 0x7F. Here we set it to "disregard channel".
		0x06  Sub-ID -- General Information
		0x01  Sub-ID2 -- Identity Request
		0xF7  End of SysEx

		Here is the Identity Reply message:

		0xF0  SysEx
		0x7E  Non-Realtime
		0x7F  The SysEx channel. Could be from 0x00 to 0x7F. Here we set it to "disregard channel".
		0x06  Sub-ID -- General Information
		0x02  Sub-ID2 -- Identity Reply
		0xID  Manufacturer's ID				--> "Educational Use  0x7D"
		0xf1  The f1 and f2 bytes make up the family code. Each
		0xf2  manufacturer assigns different family codes to his products.
		0xp1  The p1 and p2 bytes make up the model number. Each
		0xp2  manufacturer assigns different model numbers to his products.
		0xv1  The v1, v2, v3 and v4 bytes make up the version number.
		0xv2
		0xv3
		0xv4
		0xF7  End of SysEx
	 *
	 */
	QByteArray identReqSysexMsg;
	identReqSysexMsg   << 0x7e << 0x7f << 0x06 << 0x01;
	QByteArray identReqSysexReply;
	identReqSysexReply << 0x7e << 0x7f << 0x06 << 0x02
			<< 0x7d
			<< 0x00 << 0x01						//
			<< 0x09 << 0x00						// N900 model
			<< 0x00 << 0x01 << 0x01 << 0x00;	// version 0.1-1 (same has build)

	// SysexAction for Identity Reply....
	MidiSysexAction* sysexAction = new MidiSysexActionEx(identReqSysexMsg, identReqSysexReply, this);
	m_midiSysexActionMap.insert(sysexAction->getMapKey(), sysexAction);
	m_midiSysexBg = NULL;
}


MidiMappingGui::~MidiMappingGui()
{
	qDeleteAll(m_midiSysexActionMap.values());
	qDeleteAll(m_midiControlsList);
}

// Connects all output controls to a 'Slot(uchar,uchar,uchar)'
void MidiMappingGui::connectMidiOutPort(const QObject *receiver, const char* receiverSlot, Qt::ConnectionType conType)
{
	// connect MidiOut events
	foreach(GraphicsMidiControlItem* outputControl, m_midiControlsMap.values())
	{
		if (outputControl->hasOutput())
			connect(outputControl, SIGNAL(midiShortMsgOut(uchar,uchar,uchar)), receiver, receiverSlot, conType);
	}
};




bool MidiMappingGui::loadScheme(const QString& schemeFile, QList<QGraphicsItem*>& guiItems)
{
	QDomElement root = openXMLFile(schemeFile, QString("midijScheme"));
	if (root.isNull())
		return false;

	QDomElement toggleButtonNode = root.firstChildElement(SCHEME_TOGGLEBUTTONAREA);
	m_toggleButtonRect = QRect();
	QStringList coords = toggleButtonNode.text().split(",");
	if (coords.size() == 4)
		m_toggleButtonRect = QRect(coords.at(0).toInt(), coords.at(1).toInt(), coords.at(2).toInt(), coords.at(3).toInt());
	else
		qWarning() << "invalid toggle button rect:" << toggleButtonNode.text() << "; line:" << toggleButtonNode.lineNumber();

	// load other source files
	QDomElement source = root.firstChildElement(SCHEME_SOURCES_TAG).firstChildElement(SCHEME_SOURCE);
	while (!source.isNull())
	{
		QString name = source.attribute("id");
		if (!name.isEmpty())
		{
			QString file = source.text();
			m_sources.insert(name, file);
		}
		source = source.nextSiblingElement(SCHEME_SOURCE);
	}

	QFileInfo guiFile(m_sources.value("gui"));
	DirContext dc(guiFile);
	return loadGraphicItems(openXMLFile(guiFile.fileName(), QString("guiMapping")), guiItems);
}



#define CONTROL_INPUT					"Input"
#define CONTROL_OUTPUT					"Output"
#define CONTROL_ITEM					"Control"
#define CONTROL_ITEM_MULTI				"MultiControl"
#define SYSEX_INPUT_MESSAGES_BLOCK		"Messages"
#define SYSEX_INPUT_MESSAGE				"message"
#define SYSEX_INPUT_INIT				"Init"

bool MidiMappingGui::loadGraphicItems(QDomElement root, QList<QGraphicsItem*>& guiItems)
{
	if (root.isNull())
		return false;

	m_Bindings = root;
	guiItems.clear();

	// loading map of Pixmaps
	PixmapMap pixmapMap;
	QDomElement image = m_Bindings.firstChildElement("Images").firstChildElement("image");
	if (image.isNull())
	{
		qCritical() << "missing images tag";
		return false;
	}
	while (!image.isNull())
	{
		QString name = image.attribute("id", "undef");
		QString file = image.text();
		QPixmap pixmap(file);
		if (pixmap.isNull())
		{
#ifdef MIDICONTROL_XMLELEMENT_OUTPUT
			pixmap = QPixmap(QSize(480,800));
			QString pseudoColor = name;
			if (pseudoColor.contains('_'))
				pseudoColor = pseudoColor.left(pseudoColor.indexOf('_'));
			if (pseudoColor.contains(' '))
				pseudoColor = pseudoColor.left(pseudoColor.indexOf(' '));
			pixmap.fill(QColor(pseudoColor));
#else
			qCritical() << "missing image file:" << file;
			return false;
#endif
		}
		pixmapMap.insert(name, pixmap);

		if (GuiUtils::isBackground(name))
		{
			QGraphicsPixmapItem* bgi = new QGraphicsPixmapItem(pixmap);
			bgi->setPos(0,0);
			bgi->setZValue(0);
			guiItems.append(bgi);
		}
		image = image.nextSiblingElement("image");
	}

	m_background = pixmapMap[GuiUtils::background];
	if (m_background.isNull())
	{
		qCritical() << "missing background image";
		return false;
	}


	m_midiControlsMap.clear();
	QDomElement output = m_Bindings.firstChildElement(CONTROL_OUTPUT).firstChildElement(CONTROL_ITEM);
	//Iterate through each <control> block in the XML
	while (!output.isNull())
	{
		//Unserialize these objects from the XML
		//MidiMessage midiMessage(output);
		GraphicsMidiControlItem* outputControl = new GraphicsMidiOutputItem(output, pixmapMap);

		if (outputControl->isValid() && !m_midiControlsMap.contains(outputControl->name()))
		{
			//Add to the gui midi control mapping.
			m_midiControlsMap.insert(outputControl->name(), outputControl);
			guiItems.append(outputControl);
#ifdef MIDICONTROL_XMLELEMENT_OUTPUT
			outputControl->setData(MIDICONTROL_XMLELEMENT_OUTPUT, qVariantFromValue((void*) new QDomElement(output)));
#endif
		}
		else
			delete outputControl; // "there can be only one"!!

		output = output.nextSiblingElement(CONTROL_ITEM);
	}
	qDebug() << "MidiMappingGui: Output parsed!";

	QDomElement input = m_Bindings.firstChildElement(CONTROL_INPUT).firstChildElement(CONTROL_ITEM);
	//Iterate through each <control> block in the XML
	while (!input.isNull())
	{
		//Unserialize these objects from the XML
		//MidiMessage midiMessage(output);
		GraphicsMidiInputItem* inputControl = new GraphicsMidiInputItem(input, pixmapMap);

		if (m_midiControlsMap.contains(inputControl->name()))
		{
			// merge into existing control
			GraphicsMidiControlItem* midiControl = m_midiControlsMap.value(inputControl->name());
			inputControl->mergeInto(midiControl);
#ifdef MIDICONTROL_XMLELEMENT_INPUT
			midiControl->setData(MIDICONTROL_XMLELEMENT_INPUT, qVariantFromValue((void*) new QDomElement(input)));
#endif
			delete inputControl;
		}
		else if (inputControl->isValid())
		{
			//Add new gui midi control
			m_midiControlsMap.insert(inputControl->name(), inputControl);
			inputControl->setAcceptedMouseButtons(Qt::NoButton); // display only...
#ifdef MIDICONTROL_XMLELEMENT_INPUT
			inputControl->setData(MIDICONTROL_XMLELEMENT_INPUT, qVariantFromValue((void*) new QDomElement(input)));
#endif
			guiItems.append(inputControl);
		}
		else
			delete inputControl;

		input = input.nextSiblingElement(CONTROL_ITEM);
	}

	QDomElement multiItem = m_Bindings.firstChildElement(CONTROL_INPUT).firstChildElement(CONTROL_ITEM_MULTI);
	//Iterate through each <control> block in the XML
	while (!multiItem.isNull())
	{
		//Unserialize these objects from the XML
		//MidiMessage midiMessage(output);
		GraphicsMidiMultiItem* multiControl = new GraphicsMidiMultiItem(multiItem, m_midiControlsMap);

		if (m_midiControlsMap.contains(multiControl->name()) || !multiControl->isValid())
		{
			// control is not valid!!
			qWarning() << "multi control deleted:" << multiControl->name();
			delete multiControl;
		}
		else
		{
			//Add new MIDI control
			m_midiControlsMap.insert(multiControl->name(), multiControl);
			m_midiControlsList.append(multiControl); // to be deleted!!
		}
		multiItem = multiItem.nextSiblingElement(CONTROL_ITEM_MULTI);
	}

	// map MidiMessage to InputItem
	m_inputMapping.clear();
	foreach (GraphicsMidiControlItem* item, m_midiControlsMap.values())
	{
		if (item->hasInput())
		{
			foreach (MidiMessage msg, item->inputMidiMessages())
			{
				m_inputMapping.insert(msg, item);
			}
		}
	}

	// Sysex Input.....
	QDomElement sysexInputNode = m_Bindings.firstChildElement("SysexInput");
	QDomElement sysexInputMsg = sysexInputNode.firstChildElement(SYSEX_INPUT_MESSAGES_BLOCK);
	//Iterate through each <Messages> block in the XML
	while (!sysexInputMsg.isNull())
	{
		QDomElement sysexMessage = sysexInputMsg.firstChildElement(SYSEX_INPUT_MESSAGE);
		while (!sysexMessage.isNull())
		{
			//Unserialize these objects from the XML
			MidiSysexAction* sysexAction = new MidiSysexAction(sysexInputMsg, sysexMessage, this, pixmapMap);
			QString mapKey = sysexAction->getMapKey();
			if (sysexAction->isValid() && !m_midiSysexActionMap.contains(mapKey))
			{
				m_midiSysexActionMap.insert(mapKey, sysexAction);
				QGraphicsItem* bgItem = sysexAction->background();
				if (bgItem != NULL)
					guiItems.append(bgItem);
				//qDebug() << "add sysex:" << sysexAction->getMapKey();
			}
			else
			{
				delete sysexAction;
			}
			sysexMessage = sysexMessage.nextSiblingElement(SYSEX_INPUT_MESSAGE);
		}
		sysexInputMsg = sysexInputMsg.nextSiblingElement(SYSEX_INPUT_MESSAGES_BLOCK);
	}
	QDomElement sysexInputInit = sysexInputNode.firstChildElement(SYSEX_INPUT_INIT);
	while (!sysexInputInit.isNull())
	{
		QByteArray sysexInitMsg = MidiSysexAction::parseId(sysexInputInit.text());
		if (sysexInitMsg.count() > 0)
		{
			m_sysexInitMsgList.append(sysexInitMsg);
		}
		sysexInputInit = sysexInputInit.nextSiblingElement(SYSEX_INPUT_INIT);
	}

	qDebug() << __FUNCTION__  << ": Input parsed!";
	return true;
}


void MidiMappingGui::init()
{
	foreach(QByteArray sysexInitMsg, m_sysexInitMsgList)
	{
		QByteArray msg;
		msg.append(0xF0);
		msg.append(sysexInitMsg);
		msg.append(0xF7);
		midiSysexMsgIn((uchar*)msg.data(), (uint)msg.size());
	}
}


void MidiMappingGui::clear()
{
	m_Bindings.clear();
	m_inputMapping.clear(); // values are deleted by main class
	m_midiControlsMap.clear();
	qDeleteAll(m_midiControlsList);
	m_midiControlsList.clear();

	QMutableMapIterator<QString, MidiSysexAction*> midiSysexIt(m_midiSysexActionMap);
	while (midiSysexIt.hasNext())
	{
		midiSysexIt.next();
		if (midiSysexIt.value()->isUserDefined())
		{
			delete midiSysexIt.value();
			midiSysexIt.remove();
		}
	}
	m_sources.clear();
	m_background = QPixmap();
	m_receiver = NULL;
	m_receiverMember = NULL;
	m_midiSysexBg = NULL;
	m_sysexInitMsgList.clear();
	//m_presetPath;
	//m_controlMessages;
}

QDomElement MidiMappingGui::openXMLFile(QString path, QString domName)
{
	QDomDocument doc(domName);
	QFile file(path);
	if (!file.open(QFile::ReadOnly))
	{
		qWarning() << "Could not open xml file:" << file.fileName();
		return QDomElement();
	}
	QString error;
	if (!doc.setContent(&file, &error))
	{
		qWarning() << "Error parsing xml file:" << file.fileName() << "\n" << error;
		file.close();
		return QDomElement();
	}
	file.close();
	QDomElement root = doc.documentElement();
	root.setAttribute("sourceFile", path);
	return root;
}

QString MidiMappingGui::sourceFile(const QDomElement& root)
{
	return root.attribute("sourceFile");
}

QDomElement MidiMappingGui::sourcePreset(QString name)
{
	if (m_sources.contains(name))
	{
		return openXMLFile(m_sources.value(name), name);
	}
	qDebug() << "source preset missing" << name;
	return QDomElement();
}



void MidiMappingGui::midiShortMsgIn(uchar statusByte, uchar midino, uchar midivalue)
{
	MidiMessage inputCommand((MidiStatusByte)(statusByte &0xF0), midino, statusByte & 0xF);

	// check mappings
	QList<GraphicsMidiControlItem*> items = m_inputMapping.values(inputCommand);
	if (items.size() > 0)
	{
		foreach(GraphicsMidiControlItem* item, items)
		{
			item->receivedMidiShortMsg(inputCommand, midivalue);
		}
		return;
	}

	if (m_controlMessages.contains(inputCommand))
	{
		const char* method = m_controlMessages.value(inputCommand);
		QMetaObject::invokeMethod(this, method, Qt::AutoConnection);
		return;
	}

	qWarning(QString("MidiMappingGui: UNHANDLED MIDI MSG >>>>>> status:0x%1; ch:%2; no:0x%3; value:0x%4")
		.arg(statusByte & 0xF0,2,16)
		.arg(statusByte & 0xF)
		.arg(midino,2,16)
		.arg(midivalue,2,16).toAscii());
}


void MidiMappingGui::midiSysexMsgIn(uchar *data, uint length)
{
	QString sysexMapKey = "key";
	if (data == NULL || length == 0)
	{
		qWarning() << "NULL Sysex data";
		return;
	}
	const uchar *keyData = &data[1]; // ignore 0xF0
	int len = (int)length - 2; // ignoring 0xF7
	int subIdLevel = m_minSubIdLevel;
	while (!sysexMapKey.isEmpty() && subIdLevel <= m_maxSubIdLevel && subIdLevel < len)
	{
		sysexMapKey = MidiSysexAction::makeMapKey(keyData, subIdLevel);
		//qDebug(sysexMapKey.toAscii());
		MidiSysexAction* action = m_midiSysexActionMap.value(sysexMapKey);
		if (action != NULL)
		{
			QGraphicsPixmapItem* bg = action->background();
			if (bg != NULL)
			{
				if (m_midiSysexBg != NULL)
					m_midiSysexBg->setVisible(false);
				m_midiSysexBg = bg;
				m_midiSysexBg->setVisible(true);
			}
			action->run("");
			return;
		}
		subIdLevel++;
	}
	qWarning() << "midiSysexMsgIn: UNHANDLED SYSEX MSG" <<  MidiSysexAction::makeMapKey(keyData, subIdLevel-1);
}


void MidiMappingGui::emitMidiSysexMsgOut(const QByteArray& data)
{
	if (data.size() > 0)
	{
		QByteArray msg;
		msg << MIDI_SYSEX << data << MIDI_EOX;
		emit(midiSysexMsgOut((uchar*)msg.data(), msg.size()));
	}
}

//
// handles message: CC.0x7b
void MidiMappingGui::ctrlAllNotesOff()
{
	qDebug("Midi msg - 0xB0.0x7b - ctrlAllNotesOff()");
	foreach(GraphicsMidiControlItem* item, m_inputMapping.values())
	{
		item->setOffState();
	}
}


