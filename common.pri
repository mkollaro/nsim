# settings and source files shared between the targets

PROJ_DIR = $$PWD
DESTDIR = $$PWD"/bin"
OBJECTS_DIR = $$PROJ_DIR"/build"
MOC_DIR = $$PROJ_DIR"/build"
UI_DIR = $$PROJ_DIR"/build"
RCC_DIR = $$PROJ_DIR"/build"

QT += xml
CONFIG += c++11
QMAKE_CXXFLAGS += -Wall -Wextra -pedantic

SOURCES +=  $$files($$PROJ_DIR"/src/physics/*.cpp")\
            $$files($$PROJ_DIR"/src/algorithms/*.cpp")\

HEADERS +=  $$files($$PROJ_DIR"/src/physics/*.h")\
            $$files($$PROJ_DIR"/src/algorithms/*.h")\
            $$PROJ_DIR"/src/exceptions.h"\
