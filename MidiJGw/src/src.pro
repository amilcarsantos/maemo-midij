TEMPLATE = app
TARGET = MidiJGw

# Matches first 3 numbers with MidiJ release
VERSION = 0.1.2.3

QT        += core gui network

# For development (send/receives MIDI packects only to/from localhost)
DEFINES += __MIDIUDP_LOCALHOST__
DEFINES += MIDIJGW_VERSION=\\\"$$VERSION\\\"

HEADERS   += MidiJGw.h \
    MidiObjectUdp.h \
    MidiDeviceHandler.h \
    ActionId.h \
    LightLine.h \
    Utils.h \
    mixxx/MidiMessage.h \
    mixxx/MidiMapping.h \
    mixxx/MidiObject.h

SOURCES   += main.cpp \
    MidiJGw.cpp \
    MidiObjectUdp.cpp \
    MidiDeviceHandler.cpp \
    ActionId.cpp \
    LightLine.cpp \
    mixxx/MidiMessage.cpp \
    mixxx/MidiMapping.cpp \
    mixxx/MidiObject.cpp

win32 {
	QMAKE_TARGET_COMPANY = Amilcar Santos
	QMAKE_TARGET_DESCRIPTION = MIDI gateway for MidiJ application
	QMAKE_TARGET_COPYRIGHT = GNU GENERAL PUBLIC LICENSE V2
	QMAKE_TARGET_PRODUCT = MidiJ Gateway
	
	HEADERS += mixxx/MidiObjectWin.h
	SOURCES += mixxx/MidiObjectWin.cpp
	LIBS += -lwinmm
	## Comment the next line to generate a new "MidiJGw_resource.rc"
	RC_FILE = MidiJGw.rc
	## uncomment to generate the application
}

FORMS     += MidiJGw.ui
RESOURCES += res/MidijGw.qrc

include(../qtsingleapplication/src/qtsingleapplication.pri)

# keep the tree clean
DESTDIR = build
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
