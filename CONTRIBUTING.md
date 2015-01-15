# How to contribute

The project attempts to follow the [google C++ style
guide](https://google-styleguide.googlecode.com/svn/trunk/cppguide.html),
though it uses the K&R indentation, which you can achieve by using `astyle`:

    $ astyle --style=kr FILE

Before committing code, please check it with:

    $ cppcheck --enable=all --inline-suppr src/

## Developer documentation

You will need `doxygen`, `graphviz` and LaTeX to generate the source code
documentation. On Ubuntu and related systems, you can install them like this:

    $ sudo apt-get install doxygen graphviz texlive-base

To generate it, run:

    $ qmake && make docs

## Testing

To run the unit tests:

    $ bin/unit_tests

The project uses [bats](https://github.com/sstephenson/bats) for functional
tests. After installing `bats`, run them tests like this:

    $ bats tests/functional/

Executing them all:

    $ make alltests
