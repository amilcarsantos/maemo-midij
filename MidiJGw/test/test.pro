CONFIG += qtestlib
DEPENDPATH += ../src
INCLUDEPATH += ../src
QT += network scripttools

DEFINES += __MIDIUDP_LOCALHOST__

HEADERS += ../src/MidiObjectUdp.h \
    ../src/mixxx/MidiMessage.h \
    ../src/mixxx/MidiMapping.h \
	../src/mixxx/MidiObject.h \
	FakeMidiReceiver.h

SOURCES += ../src/MidiObjectUdp.cpp \
    ../src/mixxx/MidiMessage.cpp \
    ../src/mixxx/MidiMapping.cpp \
	../src/mixxx/MidiObject.cpp \ 
	FakeMidiReceiver.cpp \
	TestMidiUdp.cpp


# keep the tree clean
DESTDIR = build
OBJECTS_DIR = build
MOC_DIR = build


