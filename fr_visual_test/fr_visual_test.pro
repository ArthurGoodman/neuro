TEMPLATE = app
CONFIG += windows c++11
QT += core gui widgets

LIBS += -L../../framework/release -L../release -lframework -lneuro -lgdi32 -lgdiplus
INCLUDEPATH += ../../framework ..

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h
