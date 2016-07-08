TEMPLATE = app
CONFIG += windows c++11
QT += core gui widgets

LIBS += -L../../framework/release -L../debug -lframework -lneuro -lgdi32
INCLUDEPATH += ../../framework ..

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h
