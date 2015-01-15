# graphical interface for NSim
TARGET = nsim
include(../common.pri)

CONFIG += qt
QT += widgets opengl

HEADERS +=  $$files(gui/*.h)\
            projectparser.h\
            buffer.h\
            simulation.h\
            simulationhistory.h\

SOURCES +=  $$files(gui/*.cpp)\
            projectparser.cpp\
            main.cpp\
            simulation.cpp\
            simulationhistory.cpp\

FORMS += gui/mainwindow.ui
RESOURCES += gui/nbody.qrc
