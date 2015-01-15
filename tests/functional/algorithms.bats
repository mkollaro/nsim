#!/usr/bin/env bats
#set -x

load common

FILE=$PROJ_DIR"/examples/earth-moon-sun.xml"
EXPECTED=$BATS_TEST_DIRNAME"/files/expected_earth-moon-sun.csv"
RESULT=$BATS_TMPDIR"/result.csv"

@test "compare abm8 results of earth-moon-sun" {
    $CMD -f $FILE -a abm8 > $RESULT
    $DIFF --epsilon 0.0001 $EXPECTED $RESULT
}

@test "compare abm4 results of earth-moon-sun" {
    $CMD -f $FILE -a abm4 > $RESULT
    $DIFF --epsilon 0.0001 $EXPECTED $RESULT
}

@test "compare ab8 results of earth-moon-sun" {
    $CMD -f $FILE -a ab8 > $RESULT
    $DIFF --epsilon 0.0001 $EXPECTED $RESULT
}

@test "compare ab4 results of earth-moon-sun" {
    $CMD -f $FILE -a ab4 > $RESULT
    $DIFF --epsilon 0.001 $EXPECTED $RESULT
}

@test "compare rk4 results of earth-moon-sun" {
    $CMD -f $FILE -a rk4 > $RESULT
    $DIFF --epsilon 0.01 $EXPECTED $RESULT
}

@test "compare leapfrog results of earth-moon-sun" {
    $CMD -f $FILE -a leapfrog > $RESULT
    $DIFF --epsilon 0.1 $EXPECTED $RESULT
}

@test "compare euler results of earth-moon-sun" {
    $CMD -f $FILE -a euler > $RESULT
    $DIFF --epsilon 0.1 $EXPECTED $RESULT
}
