/***************************************************************************
                          midiobjectwin.cpp  -  description
                          -----------------
    begin                : Thu Jul 4 2002
    copyright            : (C) 2002 by Tue & Ken Haste Andersen
                            heavily modified by Sean M. Pappalardo
    email                : haste@diku.dk
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include <QtCore>
#include <QtDebug>
//MidiJ_DEL//#include "midiledhandler.h"
#include "midiobjectwin.h"
#include <qapplication.h>

MidiObjectWin::MidiObjectWin(uint restrictionFlags) : MidiObject(restrictionFlags)
{
    QStringList commandLineArgs = QApplication::arguments();
    midiDebug = commandLineArgs.indexOf("--midiDebug");

    updateDeviceList();
    /*
       // Don't open the device yet, it gets opened via dlgprefmidi soon
       // This is how the ALSA one does it anyway... -Adam
       // Open device
       if (device_valid)
       devOpen(device);
       else
       if (devices.count()==0)
        qDebug() << "MIDI: No MIDI devices available.";
        */
}

MidiObjectWin::~MidiObjectWin()
{
    shutdown(); // From parent MidiObject
    // Close devices and delete buffer
//    while (openDevices.count() > 0) devClose(openDevices.takeFirst());
    while (openDevices.count() > 0) {
        openDevices.removeFirst();
        devClose();
    }
}

void MidiObjectWin::updateDeviceList() {

    // Fill in list of available input devices
    devices.clear();

    if (m_restrictionFlags & MIDI_OBJECT_ONLY_OUTPUT) // MidiJ++
    {
        MIDIOUTCAPS info;
        for (unsigned int i=0; i<midiOutGetNumDevs(); i++)
        {
            MMRESULT res = midiOutGetDevCaps(i, &info, sizeof(MIDIOUTCAPS));
            QString device_name= QString::fromUtf16((const ushort*)info.szPname);
            qDebug() << "Midi device" << device_name << "found.";

            if (!device_name.isEmpty())
                devices.append(device_name);
            else
                devices.append(QString("Device %1").arg(i));
        }
        return;
    }

    MIDIINCAPS info;
    for (unsigned int i=0; i<midiInGetNumDevs(); i++)
    {
        MMRESULT res = midiInGetDevCaps(i, &info, sizeof(MIDIINCAPS));
        QString device_name= QString::fromUtf16((const ushort*)info.szPname);
        qDebug() << "Midi device" << device_name << "found.";

        if (!device_name.isEmpty())
            devices.append(device_name);
        else
            devices.append(QString("Device %1").arg(i));
    }
}

void MidiObjectWin::devOpen(QString device)
{    
    if (openDevices.contains(device)) 
        return;
    
    // Create list of output devices
    MMRESULT res;
    if (m_restrictionFlags & MIDI_OBJECT_ONLY_OUTPUT) //MidiJ++
    	goto openOutputDev;

    // Select device. If not found, select default (first in list).
    unsigned int i;
    MIDIINCAPS info;
    for (i=0; i<midiInGetNumDevs(); i++)
    {
        MMRESULT res = midiInGetDevCaps(i, &info, sizeof(MIDIINCAPS));
        QString device_name = QString::fromUtf16((const ushort*)info.szPname);
        if ((!device_name.isEmpty() && (device_name == device))|| (QString("Device %1").arg(i) == device))
        {
            qDebug() << "Using MIDI input device #" << i << ":" << device_name;
            break;
        }
    }
    if (i==midiInGetNumDevs()) {
        qDebug() << "Error: Unable to find requested MIDI input device" << device;
        return;
    }

    HMIDIIN handle;
    res = midiInOpen(&handle, i, (DWORD_PTR)MidiInProc, (DWORD_PTR) this, CALLBACK_FUNCTION);
    if (res == MMSYSERR_NOERROR) {
        // Should follow selected device !!!!
        openDevices.append(device);
    } else {
        qDebug() << "Error opening midi input device";
        return;
    }

    m_deviceName = device;

    // Add device and input handle to list
    handles.insert(device, handle);

    res = midiInStart(handle);
    if (res != MMSYSERR_NOERROR)
        qDebug() << "Error starting midi input.";

    if (m_restrictionFlags & MIDI_OBJECT_ONLY_INPUT) //MidiJ++
    	return;
openOutputDev:

    // Same things, but for output device now
    MIDIOUTCAPS outInfo;
    qDebug() << "Searching for MIDI output devices:";
    for (i=0; i<midiOutGetNumDevs(); i++)
    {
        MMRESULT res = midiOutGetDevCaps(i, &outInfo, sizeof(MIDIOUTCAPS));
        QString output_device_name = QString::fromUtf16((const ushort*)outInfo.szPname);
        qDebug() << "Found" << output_device_name;

        // Ignore "From" and "To" text in the device names
        QString outputString = output_device_name;
        QString deviceName = device;
//        if (device.indexOf("from",0,Qt::CaseInsensitive)!=-1) deviceName = device.right(device.length()-4);
//        if (output_device_name.indexOf("to",0,Qt::CaseInsensitive)!=-1) outputString = output_device_name.right(output_device_name.length()-2);

        if ((!outputString.isEmpty() && (outputString == deviceName))|| (QString("Device %1").arg(i) == deviceName))
        {
            qDebug() << "Using MIDI Output Device #" << i << ":" << output_device_name;
            break;
        }
    }
    if (i==midiOutGetNumDevs())
        qDebug() << "Error: Unable to find requested MIDI output device" << device;
    else {
        HMIDIOUT outhandle;
        res = midiOutOpen(&outhandle, i, NULL, NULL, CALLBACK_NULL);
        if (res != MMSYSERR_NOERROR)
            qDebug() << "Error opening midi output device";
        else
        {
            outHandles.insert(device, outhandle);    // Add device and output handle to list

            if (m_restrictionFlags & MIDI_OBJECT_ONLY_OUTPUT) //MidiJ++
            {
                m_deviceName = device;
                openDevices.append(device);
            }
        }
    }

#ifdef __MIDISCRIPT__
    MidiObject::run();  // Load the initial MIDI preset
#endif
}

void MidiObjectWin::devClose()
{
    HMIDIIN handle = handles.value(m_deviceName);
    midiInReset(handle);
    midiInClose(handle);
    handles.remove(m_deviceName);

    HMIDIOUT outhandle = outHandles.value(m_deviceName);
    midiOutReset(outhandle);
    midiOutClose(outhandle);
    outHandles.remove(m_deviceName);

    openDevices.removeAll(m_deviceName);
}

void MidiObjectWin::stop()
{
    MidiObject::stop();
}

void MidiObjectWin::run()    // This function never executes because we only use callbacks
{
    unsigned static id = 0; //the id of this thread, for debugging purposes //XXX copypasta (should factor this out somehow), -kousu 2/2009
    QThread::currentThread()->setObjectName(QString("MidiObjectWin %1").arg(++id));
    
//    qDebug() << QString("MidiObjectWin: Thread ID=%1").arg(this->thread()->currentThreadId(),0,16);
#ifdef __MIDISCRIPT__
    MidiObject::run();
#endif
}

void MidiObjectWin::handleMidi(char status, char midicontrol, char midivalue)
{
    if (midiDebug != -1) qDebug() << QString("MIDI status: %1, ctrl: %2, val: %3")
        .arg(QString::number(status & 255, 16).toUpper())
        .arg(QString::number(midicontrol, 16).toUpper())
        .arg(QString::number(midivalue, 16).toUpper());

    receive((MidiStatusByte)(status & 255), status & 15, midicontrol, midivalue); // void receive(MidiStatusByte status, char channel, char control, char value);
}

// C/C++ wrapper function
void CALLBACK MidiInProc(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
    MidiObjectWin * midi = (MidiObjectWin *)dwInstance;
    switch (wMsg) {
    case MIM_DATA:
//        qDebug() << "MIM_DATA";
        midi->handleMidi(dwParam1 & 0x000000ff, (dwParam1 & 0x0000ff00) >> 8, (dwParam1 & 0x00ff0000) >> 16);
//    qDebug() << "MIM_DATA done.";
        break;
    case MIM_LONGDATA:
        qDebug() << "MIM_LONGDATA";
        // for a MIM_LONGDATA implementation example refer to "void CALLBACK MidiInProc" @ http://www.csee.umbc.edu/help/sound/TiMidity++-2.13.2/interface/rtsyn_winmm.c
        break;
    }
}

void MidiObjectWin::sendShortMsg(unsigned int word) {
    HMIDIOUT outhandle = outHandles.value(m_deviceName);
    // This checks your compiler isn't assigning some wierd type hopefully
    DWORD raw = word;
    midiOutShortMsg(outhandle, word);
}

void MidiObjectWin::sendSysexMsg(unsigned char data[], unsigned int length)
{
    HMIDIOUT outhandle = outHandles.value(m_deviceName);
    MIDIHDR header;
    memset (&header, 0, sizeof(header));
    
    header.lpData = (LPSTR)data;
    header.dwBufferLength = DWORD(length);
    header.dwBytesRecorded = DWORD(length);
    
    midiOutPrepareHeader(outhandle, &header, sizeof(header));
    midiOutLongMsg(outhandle, &header, sizeof(header));
    midiOutUnprepareHeader(outhandle, &header, sizeof(header));
}
