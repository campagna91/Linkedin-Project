#TEMPLATE = app
#CONFIG += console
#CONFIG -= app_bundle
#CONFIG -= qt
QT       += widgets
QT       += xml
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += main.cpp \
    baddb.cpp \
    badpermission.cpp \
    languages.cpp \
    linkediner.cpp \
    db.cpp \
    linkedinerbasic.cpp \
    linkedinerbusiness.cpp \
    linkedinerexecutive.cpp \
    profile.cpp \
    qualifications.cpp \
    works.cpp \
    clientstart.cpp \
    clientuserfind.cpp \
    clientuser.cpp \
    clientadmin.cpp \
    clientadminnewuser.cpp 

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    baddb.h \
    badpermission.h \
    languages.h \
    linkediner.h \
    db.h \
    linkedinerbasic.h \
    linkedinerbusiness.h \
    linkedinerexecutive.h \
    profile.h \
    qualifications.h \
    works.h \
    clientstart.h \
    clientuserfind.h \
    clientuser.h \
    clientadmin.h \
    clientadminnewuser.h 

