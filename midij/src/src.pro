TEMPLATE = app
TARGET = MidiJ
VERSION = 0.1.4
QT += core \
    gui \
    xml \
    script \
    network

# Maemo specific
unix:!symbian {
	QT += dbus maemo5
}

# For development (send MIDI packects only to localhost)
##DEFINES += __MIDIUDP_LOCALHOST__

CONFIG( script )
{
    :message(script build!)
    DEFINES += __MIDISCRIPT__
    HEADERS += mixxx/script/MidiScriptEngine.h
    SOURCES += mixxx/script/MidiScriptEngine.cpp
}

unix {
	:message(gstreamer build!)
	CONFIG += link_pkgconfig
	PKGCONFIG += gstreamer-0.10
}

HEADERS += gui/GraphicsClickItem.h \
    gui/GuiUtils.h \
    MidiObjectUdp.h \
    MidiJ.h \
    MidiObjectGuiEmul.h \
    RouterMapping.h \
    RouterScriptEngine.h \
    RouterObject.h \
    settings/AdvancedSettingsWnd.h \
    settings/SettingsDialog.h \
    settings/SettingsManager.h \
    settings/SettingsWnd.h \
    Utils.h \
    NetcastManager.h \
    serato/MidiMappingSerato.h \
    serato/MidiMessageSerato.h \
    serato/SeratoControl.h \
    mixxx/MixxxControl.h \
    mixxx/MidiMapping.h \
    mixxx/MidiObject.h \
    mixxx/MidiMessage.h \
    gui/GraphicsMidiInputItem.h \
    gui/GraphicsMidiOutputItem.h \
    gui/GraphicsMidiMultiItem.h \
    gui/GraphicsMidiControlItem.h \
    gui/MidiSysexAction.h \
    gui/MidiSysexProperties.h \
    gui/MidiMappingGui.h \
    gst/gstplay-enum.h \
    gst/GstEngine.h \
    gst/GstUdpAudioPlayer.h
 
SOURCES += gui/GraphicsClickItem.cpp \
    MidiObjectUdp.cpp \
    MidiJ.cpp \
    MidiObjectGuiEmul.cpp \
    RouterMapping.cpp \
    RouterScriptEngine.cpp \
    RouterObject.cpp \
    settings/AdvancedSettingsWnd.cpp \
    settings/SettingsDialog.cpp \
    settings/SettingsManager.cpp \
    settings/SettingsWnd.cpp \
    NetcastManager.cpp \
    serato/MidiMappingSerato.cpp \
    serato/MidiMessageSerato.cpp \
    serato/SeratoControl.cpp \
    mixxx/MixxxControl.cpp \
    mixxx/MidiMapping.cpp \
    mixxx/MidiObject.cpp \
    mixxx/MidiMessage.cpp \
    gui/GraphicsMidiInputItem.cpp \
    gui/GraphicsMidiOutputItem.cpp \
    gui/GraphicsMidiMultiItem.cpp \
    gui/GraphicsMidiControlItem.cpp \
    gui/MidiSysexAction.cpp \
    gui/MidiSysexProperties.cpp \
    gui/MidiMappingGui.cpp \
    gst/GstEngine.cpp \
    gst/GstUdpAudioPlayer.cpp \
    main.cpp

win32 {
	# Only to work on MS Windows ----
	HEADERS += gui/maemo5/qmaemo5valuebutton.h \
		gui/maemo5/qmaemo5style.h \
		gui/maemo5/qmaemo5listpickselector.h \
		gui/maemo5/qmaemo5informationbox.h \
		gui/maemo5/qmaemo5abstractpickselector.h

	SOURCES += gui/maemo5/qmaemo5valuebutton.cpp \
		gui/maemo5/qmaemo5style.cpp \
		gui/maemo5/qmaemo5listpickselector.cpp \
		gui/maemo5/qmaemo5informationbox.cpp \
		gui/maemo5/qmaemo5abstractpickselector.cpp

	# gstreamer emulation (just to compile on Windows)
	include(../FakeGstreamer/FakeGstreamer.pri)
}

FORMS += settings/SettingsDialog.ui \
	settings/AdvancedSettingsWnd.ui
UI_HEADERS_DIR = settings

RESOURCES += images/MidiJ.qrc

# keep the tree clean
DESTDIR = ../build
OBJECTS_DIR = ../build
MOC_DIR = ../build
RCC_DIR = ../build

# In Eclipse, "clean" removes binaries too.  This is also needed to work around timestamp
# issues between host and VM in ESbox.
QMAKE_CLEAN += $$DESTDIR/MidiJ

# Maemo specific
unix:!symbian { 
    # Variables
    isEmpty(PREFIX):PREFIX = /usr/bin
    BINDIR = $$PREFIX/bin
    DATADIR = $$PREFIX/share
    APPDIR = /opt/midij
    DEFINES += DATADIR=\\\"$$DATADIR\\\" \
        PKGDATADIR=\\\"$$PKGDATADIR\\\"
    
    # MAKE INSTALL
    INSTALLS += target \
        schemes \
        bluescheme \
        stantonscheme \
        desktop \
        icon64

    target.path = $$APPDIR
    images.path = $$APPDIR/images
    images.files += ../data/images/*.*
    schemes.path = $$APPDIR/schemes
    schemes.files += ../data/schemes/*.*
    bluescheme.path = $$APPDIR/schemes/blue
    bluescheme.files += ../data/schemes/blue/*.*
    stantonscheme.path = $$APPDIR/schemes/stantonscs3d
    stantonscheme.files += ../data/schemes/stantonscs3d/*.*
    desktop.path = $$DATADIR/applications/hildon
    desktop.files += midij.desktop
    icon64.path = $$DATADIR/icons/hicolor/64x64/apps
    icon64.files += ../data/icon/64x64/midij.png
}
