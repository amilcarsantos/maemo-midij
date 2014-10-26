CONFIG += qtestlib
DEPENDPATH += ../src
INCLUDEPATH += ../src
QT += xml script scripttools


CONFIG( script )
{ 
    :message(script build!)
    DEFINES += __MIDISCRIPT__
    
    HEADERS += ../src/mixxx/script/MidiScriptEngine.h
    SOURCES += ../src/mixxx/script/MidiScriptEngine.cpp
}


HEADERS += ../src/serato/MidiMappingSerato.h \
    ../src/serato/MidiMessageSerato.h \
    ../src/serato/SeratoControl.h \
    ../src/RouterMapping.h \ 
    ../src/RouterScriptEngine.h \ 
    ../src/RouterObject.h \ 
    ../src/MidiObjectGuiEmul.h \ 
    ../src/mixxx/MixxxControl.h \
    ../src/mixxx/MidiMapping.h \
    ../src/mixxx/MidiObject.h \
    FakeMidiObject.h \
    ../src/mixxx/MidiMessage.h \
    ../src/gui/GraphicsMidiControlItem.h \
    ../src/gui/GraphicsMidiInputItem.h \
    ../src/gui/GraphicsMidiOutputItem.h \
    ../src/gui/GraphicsMidiMultiItem.h \
    ../src/gui/MidiMappingGui.h \
    ../src/gui/MidiSysexAction.h \
    ../src/gui/MidiSysexProperties.h
    
SOURCES += ../src/serato/MidiMappingSerato.cpp \ 
    ../src/serato/MidiMessageSerato.cpp \
    ../src/serato/SeratoControl.cpp \
    ../src/RouterMapping.cpp \ 
    ../src/RouterScriptEngine.cpp \ 
    ../src/RouterObject.cpp \ 
    ../src/MidiObjectGuiEmul.cpp \ 
    ../src/mixxx/MixxxControl.cpp \
    ../src/mixxx/MidiMapping.cpp \
    ../src/mixxx/MidiObject.cpp \
    FakeMidiObject.cpp \
    ../src/mixxx/MidiMessage.cpp \
    ../src/gui/GraphicsMidiControlItem.cpp \
    ../src/gui/GraphicsMidiInputItem.cpp \
    ../src/gui/GraphicsMidiOutputItem.cpp \
    ../src/gui/GraphicsMidiMultiItem.cpp \
    ../src/gui/MidiMappingGui.cpp \
    ../src/gui/MidiSysexAction.cpp \
    ../src/gui/MidiSysexProperties.cpp \
    TestMidiJ.cpp

# keep the tree clean
DESTDIR = build
OBJECTS_DIR = build
MOC_DIR = build


