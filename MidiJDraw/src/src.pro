TEMPLATE = app
TARGET = MidiJDraw 

 
QT        += core gui xml script scripttools

HEADERS   += MidijDraw.h \
	MidiMappingDraw.h \
	MidijDrawGraphicsScene.h \
	MidijStateEditor.h \
	MidijMidiScriptEngine.h \
	MidijMidiObject.h \
	XmlHighlighter.h \
	midij/gui/MidiMappingGui.h \
	midij/gui/GraphicsMidiControlItem.h \
	midij/gui/GraphicsMidiInputItem.h \
	midij/gui/GraphicsMidiMultiItem.h \
	midij/gui/GraphicsMidiOutputItem.h \
	midij/gui/MidiSysexAction.h \
	midij/gui/MidiSysexProperties.h \
	midij/mixxx/MidiMapping.h \
	midij/mixxx/MidiMessage.h \
	midij/mixxx/MidiObject.h \
	midij/mixxx/MixxxControl.h \
	midij/mixxx/script/MidiScriptEngine.h

SOURCES   += main.cpp \
    MidijDraw.cpp \
	MidiMappingDraw.cpp \
	MidijDrawGraphicsScene.cpp \
	MidijStateEditor.cpp \
	MidijMidiScriptEngine.cpp \
	MidijMidiObject.cpp \
	XmlHighlighter.cpp \
	midij/gui/MidiMappingGui.cpp \
	midij/gui/GraphicsMidiControlItem.cpp \
	midij/gui/GraphicsMidiInputItem.cpp \
	midij/gui/GraphicsMidiMultiItem.cpp \
	midij/gui/GraphicsMidiOutputItem.cpp \
	midij/gui/MidiSysexAction.cpp \
	midij/gui/MidiSysexProperties.cpp \
	midij/mixxx/MidiMapping.cpp \
	midij/mixxx/MidiMessage.cpp \
	midij/mixxx/MidiObject.cpp \
	midij/mixxx/MixxxControl.cpp \
	midij/mixxx/script/MidiScriptEngine.cpp

FORMS     += MidijDraw.ui \
	MidijStateEditor.ui

RESOURCES += res/MidijDraw.qrc

DEFINES +=__MIDISCRIPT__

#	# Script testing	
#	QT += scripttools		



include($$[QT_INSTALL_PREFIX]/tools/shared/qtpropertybrowser/qtpropertybrowser.pri)
