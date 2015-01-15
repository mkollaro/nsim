# build multiple targets, e.g. the GUI and the CLI version
TEMPLATE = subdirs
SUBDIRS = cmd gui tests
 
gui.file = src/nsim.gui.pro
cmd.file = src/nsim.cmd.pro
tests.file = tests/unit/tests.pro

# build the documentation with 'make docs'
docs.target = docs
docs.commands = doxygen doc/doxygen.conf;
docs.depends =
QMAKE_EXTRA_TARGETS += docs


alltests.target = alltests
alltests.commands += bin/unit_tests &&\
                     echo "== CLI arguments tests ==" &&\
                     tests/functional/arguments.bats &&\
                     echo "== Functional tests of numeric algorithms ==" &&\
                     tests/functional/algorithms.bats

QMAKE_EXTRA_TARGETS += alltests
