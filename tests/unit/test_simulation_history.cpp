#include "catch.h"
#include <QVector>
#include "simulationhistory.h"
#include "physics/universemodel.h"
#include "physics/simulationtime.h"
#include "physics/vector.h"
#include "physics/precision.h"


TEST_CASE("Empty simulation history")
{
    SimulationHistory history;
    REQUIRE(history.universeSize() == 0);
    physics::UniverseModel universe;
    physics::SimulationTime time;
    REQUIRE_THROWS(history.load(0, &universe, &time));
    REQUIRE_THROWS(history.load(0, &universe, nullptr));
    REQUIRE_THROWS(history.load(0, nullptr, &time));
    REQUIRE_THROWS(history.bodyPosition(0, 0));
    REQUIRE_THROWS(history.bodyPositions(0));
}

TEST_CASE("Empty universe in history")
{
    SimulationHistory history;
    physics::UniverseModel universe;
    physics::SimulationTime time;
    REQUIRE_THROWS(history.save(universe, time));
    REQUIRE(history.universeSize() == 0);
}

TEST_CASE("Normal usage")
{
    SimulationHistory history;
    physics::Body planet;
    planet.position.set(1, 2, 3);
    planet.velocity.set(4, 5, 6);
    physics::UniverseModel universe {planet};
    physics::SimulationTime time;

    history.save(universe, time);
    // some computation is done, and we want to save the state at t=5
    universe[0].position.set(7, 8, 9);
    universe[0].velocity.set(10, 11, 12);
    time.updateTime();
    history.save(universe, time);
    // more computation, save the state at t=8
    universe[0].position.set(6, 6, 6);
    universe[0].velocity.set(7, 7, 7);
    time.updateTime();
    time.updateTime();
    history.save(universe, time);

    SECTION("Check bounds") {
        REQUIRE(history.universeSize() == 1);  // one planet
        REQUIRE(history.historySize() == 3);
        // body index 1 is invalid
        REQUIRE_THROWS(history.bodyPosition(1, 0));
        // history index is invalid
        REQUIRE_THROWS(history.bodyPosition(0, 3));
    }

    SECTION("Load universe at index 0") {
        history.load(0, &universe, &time);
        REQUIRE(universe[0].position == physics::Vector(1, 2, 3));
        REQUIRE(universe[0].velocity == physics::Vector(4, 5, 6));
        REQUIRE(physics::equal(time.time(), 0.0));
    }

    SECTION("Load universe at index 1") {
        history.load(1, &universe, &time);
        REQUIRE(universe[0].position == physics::Vector(7, 8, 9));
        REQUIRE(universe[0].velocity == physics::Vector(10, 11, 12));
        REQUIRE(physics::equal(time.time(), time.timeStep()));
    }

    SECTION("Load universe at index 2") {
        history.load(2, &universe, &time);
        REQUIRE(universe[0].position == physics::Vector(6, 6, 6));
        REQUIRE(universe[0].velocity == physics::Vector(7, 7, 7));
        REQUIRE(physics::equal(time.time(), time.timeStep()*3));
    }

    SECTION("Load universe at index 3 (invalid)") {
        REQUIRE_THROWS(history.load(3, &universe, &time));
    }

    SECTION("Get body position at index 0") {
        QVector3D vector = history.bodyPosition(0, 0);
        REQUIRE(qFuzzyCompare(vector, QVector3D(1, 2, 3)));
    }

    SECTION("Get body position at index 2") {
        QVector3D vector = history.bodyPosition(0, 2);
        REQUIRE(qFuzzyCompare(vector, QVector3D(6, 6, 6)));
    }

    SECTION("Get raw data") {
        const float expected[] = {1,2,3, 7,8,9, 6,6,6};
        auto result = history.bodyPositions(0);
        for(unsigned i = 0; i < history.historySize() * 3; i++) {
            REQUIRE(physics::equal(expected[i], result[i]));
        }
    }

    SECTION("Clear data") {
        history.clear();
        REQUIRE(history.universeSize() == 0);
        REQUIRE(history.historySize() == 0);
        REQUIRE(history.empty());
    }

    SECTION("Clear data from index 0") {
        history.clear(0);
        REQUIRE(history.universeSize() == 0);
        REQUIRE(history.historySize() == 0);
        REQUIRE(history.empty());
    }

    SECTION("Clear data from index 2") {
        history.clear(2);
        REQUIRE(history.universeSize() == 1);
        REQUIRE(history.historySize() == 2);
        REQUIRE(history.bodyPositions(0).size() == 2 * 3);
        REQUIRE(!history.empty());
    }

    SECTION("Clear data from index 3 (invalid)") {
        history.clear(3);  // nothing should happen
        REQUIRE(history.universeSize() == 1);
        REQUIRE(history.historySize() == 3);
        REQUIRE(history.bodyPositions(0).size() == 3 * 3);
        REQUIRE(!history.empty());
    }
}
