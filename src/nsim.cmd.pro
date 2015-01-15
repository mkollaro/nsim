# command line interface for NSim
TARGET = nsim-cmd
include(../common.pri)

QT += xml
CONFIG += console

HEADERS +=  arguments.h\
            projectparser.h\

SOURCES +=  main-cmd.cpp\
            projectparser.cpp\
            arguments.cpp\
