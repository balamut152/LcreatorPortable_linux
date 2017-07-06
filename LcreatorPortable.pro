#-------------------------------------------------
#
# Project created by QtCreator 2016-05-21T17:14:57
#
#-------------------------------------------------

QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LcreatorPortable
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    connection.h \
    additiondatadialog.h

RESOURCES += \
    icons.qrc

FORMS += \
    additiondatadialog.ui
