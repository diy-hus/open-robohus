TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    motorcontrol.cpp \
    timer.cpp \
    armcontrol.cpp \
    ballpicker.cpp \
    config.cpp \
    detectball.cpp \
    mpu9250.cpp

LIBS += `pkg-config opencv --cflags --libs`
LIBS += -lwiringPi -lpigpio -lpcd8544

HEADERS += \
    motorcontrol.h \
    timer.h \
    armcontrol.h \
    ballpicker.h \
    config.h \
    detectball.h \
    mpu9250.h
