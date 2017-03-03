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
    tonerow.cpp \
    spectrogram.cpp

HEADERS  += mainwindow.h \
    tonegenerator.h \
    tonerow.h \
    audioconstants.h \
    spectrogram.h

FORMS    += mainwindow.ui

DISTFILES += \
    README.md
