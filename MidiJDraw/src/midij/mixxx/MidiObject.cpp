/*
 * MidiObject.cpp
 *
 *  Created on: 22 de Mai de 2011
 *      Author: Amilcar
 */

#include "MidiObject.h"

MidiObject::MidiObject(QObject *parent)
	: QObject(parent)
{
	m_pScriptEngine = NULL;
}

MidiObject::~MidiObject()
{
}

void MidiObject::sendShortMsg(unsigned char status, unsigned char byte1, unsigned char byte2) {
    unsigned int word = (((unsigned int)byte2) << 16) |
                        (((unsigned int)byte1) << 8) | status;
    sendShortMsg(word);
}

void MidiObject::sendShortMsg(unsigned int word) {
    qDebug() << "MIDI short message sending not yet implemented on this platform";
}

void MidiObject::sendSysexMsg(QList<int> data, unsigned int length) {
    unsigned char * sysexMsg;
    sysexMsg = new unsigned char [length];

    for (unsigned int i=0; i<length; i++) {
        sysexMsg[i] = data.at(i);
//         qDebug() << "sysexMsg" << i << "=" << sysexMsg[i] << ", data=" << data.at(i);
    }

    sendSysexMsg(sysexMsg,length);
    delete[] sysexMsg;
}

void MidiObject::sendSysexMsg(unsigned char data[], unsigned int length) {
    qDebug() << "MIDI system exclusive message sending not yet implemented on this platform";
}
