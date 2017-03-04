#-------------------------------------------------
#
# Project created by QtCreator 2017-02-27T22:29:00
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpectroDraw
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    tonegenerator.cpp \
    spectrogram.cpp \
    tonegrid.cpp

HEADERS  += mainwindow.h \
    tonegenerator.h \
    audioconstants.h \
    spectrogram.h \
    styles.h \
    tonegrid.h

FORMS    += mainwindow.ui

DISTFILES += \
    README.md
