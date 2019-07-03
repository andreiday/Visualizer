#-------------------------------------------------
#
# Project created by QtCreator 2019-06-25T16:35:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Visualizer
TEMPLATE = app

LIBS      += -lopenal

packagesExist(hidapi-libusb) {
  unix:LIBS += -lhidapi-libusb
} else {
  packagesExist(hidapi) {
    unix:LIBS += -lhidapi
  } else {
    unix:LIBS += -lhidapi-libusb
  }
}

DISTFILES +=

HEADERS += \
    VisualizerCommon/chuck_fft.h \
    VisualizerCommon/hsv.h \
    VisualizerCommon/LEDStrip.h \
    VisualizerCommon/net_port.h \
    VisualizerCommon/Visualizer.h \
    VisualizerCommon/VisualizerDefines.h \
    VisualizerQT/VisDlg.h

SOURCES += \
    VisualizerCommon/hsv.cpp \
    VisualizerCommon/LEDStrip.cpp \
    VisualizerCommon/net_port.cpp \
    VisualizerCommon/Visualizer.cpp \
    VisualizerCommon/chuck_fft.c \
    VisualizerQT/VisDlg.cpp \
    VisualizerQT/main.cpp

RESOURCES += \
    VisualizerQT/resources.qrc

FORMS += \
    VisualizerQT/visualizer.ui
