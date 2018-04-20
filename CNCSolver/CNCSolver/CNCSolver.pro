#-------------------------------------------------
#
# Project created by QtCreator 2016-11-10T18:43:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CNCSolver
TEMPLATE = app

DESTDIR = ../bin


SOURCES += main.cpp\
        mainwindow.cpp \
    tree.cpp \
    solver.cpp \
    schedule.cpp \
    task.cpp \
    solverworker.cpp \
    solverresult.cpp \
    cncsolver.cpp \
    checkscheduleform.cpp

HEADERS  += mainwindow.h \
    tree.h \
    solver.h \
    schedule.h \
    task.h \
    solverworker.h \
    solverresult.h \
    cncsolver.h \
    checkscheduleform.h

FORMS    += mainwindow.ui \
    checkscheduleform.ui

# remove possible other optimization flags
#QMAKE_CXXFLAGS_RELEASE -= -O
#QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2

# add the desired -O3 if not present
QMAKE_CXXFLAGS_RELEASE *= -O3
