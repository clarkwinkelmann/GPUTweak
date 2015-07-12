#-------------------------------------------------
#
# Project created by QtCreator 2015-07-11T00:13:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GPUTweak
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    gpuinfowindow.cpp \
    gpunvidia.cpp \
    nvidiasettingsadapter.cpp \
    gputweakwindow.cpp \
    gpustatswindow.cpp

HEADERS  += mainwindow.h \
    gpuinfowindow.h \
    gpunvidia.h \
    gpu.h \
    nvidiasettingsadapter.h \
    gputweakwindow.h \
    gpustatswindow.h

FORMS    += mainwindow.ui \
    gpuinfowindow.ui \
    gputweakwindow.ui \
    gpustatswindow.ui
