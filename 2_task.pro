QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    contact.cpp \
    addressbook.cpp \
    validator.cpp \
    QTfile.cpp \
    QTmodelcontacts.cpp \
    addeditdialog.cpp \
    searchdialog.cpp

HEADERS += \
    libcontact.h \
    libaddressbook.h \
    libvalidator.h \
    libQTfile.h \
    libQTmodelcontacts.h \
    QTaddeditdialog.h \
    mainwindow.h \
    searchdialog.h

FORMS += \
    mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
