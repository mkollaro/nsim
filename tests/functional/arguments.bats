#!/usr/bin/env bats
#set -x

load common

RESULT=$BATS_TMPDIR"/result.csv"

@test "no parameters" {
    run $CMD 
    [ $status -eq 1 ]
    [[ "$output" =~ "No input file name specified." ]]
}

@test "print help" {
    run $CMD -h
    [ $status -eq 0 ]
    [[ "$output" =~ "Usage:" ]]
}

@test "default settings" {
    run $CMD -f $EXAMPLE_FILES"/solar system.xml"
    [ $status -eq 0 ]
    [[ "$output" =~ "Sun_x Sun_y Sun_z" ]]
}

@test "center on Sun (default)" {
    FILE=$EXAMPLE_FILES"/earth-moon-sun.xml"
    EXPECTED=$TEST_FILES"/expected_earth-moon-sun.csv"

    $CMD -f $FILE -c 0 > $RESULT
    $DIFF --epsilon 0.01 $EXPECTED $RESULT
}

@test "center on Earth" {
    FILE=$EXAMPLE_FILES"/earth-moon-sun.xml"
    EXPECTED=$TEST_FILES"/expected_earth-moon-sun_centered-on-earth.csv"

    $CMD -f $FILE -c 1 > $RESULT
    $DIFF --epsilon 0.01 $EXPECTED $RESULT
}

@test "center on Moon" {
    FILE=$EXAMPLE_FILES"/earth-moon-sun.xml"
    EXPECTED=$TEST_FILES"/expected_earth-moon-sun_centered-on-moon.csv"

    $CMD -f $FILE -c 2 > $RESULT
    $DIFF --epsilon 0.01 $EXPECTED $RESULT
}

@test "center on barycenter" {
    FILE=$EXAMPLE_FILES"/earth-moon-sun.xml"
    EXPECTED=$TEST_FILES"/expected_earth-moon-sun_centered-on-barycenter.csv"

    $CMD -f $FILE -b > $RESULT
    $DIFF --epsilon 0.01 $EXPECTED $RESULT
}
