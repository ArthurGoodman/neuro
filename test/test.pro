TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle qt

LIBS += -L../../framework/release -L../release -lframework -lneuro -lgdi32
INCLUDEPATH += ../../framework ..

SOURCES += \
    main.cpp
