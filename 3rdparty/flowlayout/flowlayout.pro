DEFINES += FLOWLAYOUT_LIBRARY

QTDIR_build {
# This is only for the Qt build. Do not use externally. We mean it.
PLUGIN_TYPE = designer
PLUGIN_CLASS_NAME = FlowLayoutPlugin
load(qt_plugin)
} else {
# Public example:

TEMPLATE = lib
CONFIG  += plugin

unix:TARGET   = flowlayout
win32:TARGET = ../../../dist/bin/flowlayout

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS += target

}

QT      += widgets designer

CONFIG += c++11


HEADERS     = flowlayout.h \
              flowlayout_global.h

SOURCES     = flowlayout.cpp
