CONFIG += qtestlib
DEPENDPATH += ../src
INCLUDEPATH += ../src
QT += network scripttools

DEFINES += __NET_TEST__ MIDIJGW_VERSION=\\\"\\\"

HEADERS += ../src/MidiObjectUdp.h \
    ../src/MidiJGw.h \
    ../src/MidiDeviceHandler.h \
    ../src/ActionId.h \
    ../src/LightLine.h \
    ../src/mixxx/MidiMessage.h \
    ../src/mixxx/MidiMapping.h \
	../src/mixxx/MidiObject.h \
    ../test/FakeMidiReceiver.h

SOURCES += ../src/MidiObjectUdp.cpp \
    ../src/MidiJGw.cpp \
    ../src/MidiDeviceHandler.cpp \
    ../src/ActionId.cpp \
    ../src/LightLine.cpp \
    ../src/mixxx/MidiMessage.cpp \
    ../src/mixxx/MidiMapping.cpp \
	../src/mixxx/MidiObject.cpp \ 
    ../test/FakeMidiReceiver.cpp \
	TestMidiJGw.cpp

win32 {
    HEADERS += ../src/mixxx/MidiObjectWin.h
    SOURCES += ../src/mixxx/MidiObjectWin.cpp
    LIBS += -lwinmm
}


# keep the tree clean
DESTDIR = build
OBJECTS_DIR = build
MOC_DIR = build


