TEMPLATE = app
CONFIG += c++11
QT += core gui widgets

LIBS += -L../../framework/debug -L../debug -lframework -lneuro -lgdi32
INCLUDEPATH += ../../framework ..

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h
