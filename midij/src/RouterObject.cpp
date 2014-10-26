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

#include "RouterObject.h"
#include <QtDebug>
#include "gui/MidiMappingGui.h"
#include "Utils.h"

RouterObject::RouterObject()
{
	m_sourceMidiObject = NULL;
	m_gatewayMidiObject = NULL;
	m_pEngine = new RouterScriptEngine();
	m_lastError = 0; //NoError;
}

RouterObject::~RouterObject()
{
	if (m_pEngine)
		delete m_pEngine;
}

bool RouterObject::loadMappings(QDomElement routerXml, QDomElement seratoXml)
{
	if (routerXml.isNull())
	{
		qWarning() << "falhou abrir RouterMappings";
		m_lastError = -1;
		return false;
	}
	QFileInfo routerFile(MidiMappingGui::sourceFile(routerXml));
	if (m_pEngine->init(m_sourceMidiObject, m_gatewayMidiObject, this) && routerFile.exists())
	{
		m_routerMapping.setScriptEngine(m_pEngine);
		DirContext dc(routerFile);
		if (!m_routerMapping.loadPreset(routerXml))
		{
			m_lastError = -1;
			return false;
		}
	}
	else
	{
		m_lastError = -1;
		return false;
	}

	// check if Serato XML is supplied
	if (seratoXml.isNull())
		return true;
	QFileInfo seratoFile(MidiMappingGui::sourceFile(seratoXml));
	if (seratoFile.exists())
	{
		DirContext dc(seratoFile);
		if (m_seratoMapping.loadPreset(seratoXml))
			return true;
	}
	// fail to load Serato mappings
	m_lastError = -1;
	return false;
}

void RouterObject::setMidiObjects(MidiObject* source, MidiObject* gateway)
{
//	Q_CHECK_PTR(source);
	Q_CHECK_PTR(gateway);
	m_sourceMidiObject = source;
	m_gatewayMidiObject = gateway;
}

void RouterObject::midiSourceReceive(MidiMessage inputCommand, char midivalue)
{
//	qDebug() << "RouterObject::midiSourceReceive() tr(" << QThread::currentThreadId() << ") - " << (Qt::HANDLE)inputCommand.getMidiStatusByte() << (Qt::HANDLE)inputCommand.getMidiNo() << (int)midivalue;

	if (!m_routerMapping.isMidiMessageMapped(inputCommand))
		return;

	SeratoControl seratoControl = m_routerMapping.getOutputSeratoControl(inputCommand);
//	qDebug() << "midiSourceReceive() - " << seratoControl.toString() << "; key:" << seratoControl.getKey() << ";option:" << seratoControl.getMidiOption();

	if (seratoControl.getMidiOption() == MIDI_OPT_SCRIPT)
	{
		m_pEngine->execute(seratoControl.getKey(), inputCommand.getMidiChannel(), inputCommand.getMidiNo(), midivalue, (MidiStatusByte)(inputCommand.getMidiStatusByte() & 0xF0));
		return;
	}

	if (!m_seratoMapping.isOutputSeratoMessageMapped(seratoControl.getKey()))
		return;

	MidiMessageSerato mms = m_seratoMapping.getOutputSeratoMessage(seratoControl.getKey());
	m_gatewayMidiObject->sendShortMsg(mms.m_midiStatusByte + (mms.m_midiChannel), mms.m_midiNo, midivalue);
}

void RouterObject::sourceReload()
{
	qDebug("RouterObject::sourceReload()");
	m_pEngine->init(m_sourceMidiObject, m_gatewayMidiObject, this);
}


void RouterObject::midiGatewayShortMsgOut(uchar statusByte, uchar midino, uchar midivalue)
{
	qDebug("midiGatewaySysexMsgOut(uchar* data, uint size)");
	if (m_gatewayMidiObject)
		m_gatewayMidiObject->sendShortMsg(statusByte, midino, midivalue);
}

void RouterObject::midiGatewaySysexMsgOut(uchar* data, uint size)
{
	qDebug("midiGatewaySysexMsgOut(uchar* data, uint size)");
	if (m_gatewayMidiObject)
		m_gatewayMidiObject->sendSysexMsg(data, size);
}


//QDomElement RouterObject::openXMLFile(QString path, QString name)
//{
//    QDomDocument doc(name);
//    QFile file(path);
//    if (!file.open(QFile::ReadOnly))
//    {
//        qDebug() << "Could not open xml file:" << file.fileName();
//        return QDomElement();
//    }
//    if (!doc.setContent(&file))
//    {
//        qWarning() << "Error parsing xml file:" << file.fileName();
//        file.close();
//        return QDomElement();
//    }
//
//    file.close();
//    return doc.documentElement();
//}


void RouterObject::clear()
{
	m_routerMapping.clear();
	m_seratoMapping.clear();
	if (m_sourceMidiObject)
	{
		m_sourceMidiObject->disconnect(this);
		m_sourceMidiObject->getMidiScriptEngine()->disconnect(this);
	}
	m_sourceMidiObject = NULL;
	if (m_gatewayMidiObject)
	{
		m_gatewayMidiObject->disconnect(this);
	}
	disconnect();
	m_gatewayMidiObject = NULL;
	m_pEngine->clear();
	m_lastError = 0;
}

QString RouterObject::getValue(QString group, QString name)
{
	qDebug() << "RouterMapping::getValue:" << group << name;
	return m_seratoMapping.getValueFromBindings(group, name);
}
