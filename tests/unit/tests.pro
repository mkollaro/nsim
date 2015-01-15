TARGET = unit_tests

include("../../common.pri")
INCLUDEPATH += $$PROJ_DIR"/src/"
INCLUDEPATH += $$PROJ_DIR"/tools/"

HEADERS +=  $$PROJ_DIR"/tools/catch.h"\


SOURCES +=  tests.cpp\
            test_algorithms.cpp\
            test_vector.cpp\
            test_simulation_history.cpp\


# files not included in common.pri (because they are not used by both the CLI
# and GUI)
HEADERS += $$PROJ_DIR"/src/simulationhistory.h"\

SOURCES += $$PROJ_DIR"/src/simulationhistory.cpp"\
