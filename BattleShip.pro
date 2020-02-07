#-------------------------------------------------
#
# Project created by QtCreator 2016-07-16T14:48:24
#
#-------------------------------------------------

QT       += multimedia
QT       += core gui
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BattleShip
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    usersfield.cpp \
    computersfield.cpp \
    cell.cpp \
    abstractfield.cpp \
    ship.cpp \
    opponentofuser.cpp \
    shipfinisher.cpp

HEADERS  += mainwindow.h \
    usersfield.h \
    computersfield.h \
    cell.h \
    abstractfield.h \
    ship.h \
    opponentofuser.h \
    shipfinisher.h

FORMS    += mainwindow.ui
