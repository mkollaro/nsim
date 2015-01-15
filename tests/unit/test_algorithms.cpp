#include "catch.h"
#include "physics/universemodel.h"
#include "physics/simulationtime.h"
#include "algorithms/euler.h"
#include "algorithms/rk4.h"
#include "algorithms/leapfrog.h"
#include "algorithms/adams-bashforth.h"
#include "algorithms/abm.h"


TEST_CASE("Universe without objects shouldn't compute anything", "[algorithms]")
{
    physics::UniverseModel universe;
    physics::SimulationTime time;
    algorithms::Euler alg;
    alg.computeStep(&universe, time.timeStep());  // nothing happens
}

TEST_CASE("Check that a single static planet doesn't move", "[algorithms]")
{
    physics::Body static_planet;
    physics::UniverseModel universe {static_planet};
    physics::SimulationTime time;

    SECTION("Euler's algorithm") {
        algorithms::Euler alg;
        alg.computeStep(&universe, time.timeStep());
    }
    SECTION("Leapfrog algorithm") {
        algorithms::Leapfrog alg;
        alg.computeStep(&universe, time.timeStep());
    }
    SECTION("Runge-Kutta algorithm") {
        algorithms::RungeKutta alg {4};
        alg.computeStep(&universe, time.timeStep());
    }
    SECTION("Adams-Bashforth algorithm, order 4") {
        algorithms::AdamsBashforth alg {4};
        alg.computeStep(&universe, time.timeStep());
    }
    SECTION("Adams-Bashforth algorithm, order 8") {
        algorithms::AdamsBashforth alg {8};
        alg.computeStep(&universe, time.timeStep());
    }
    SECTION("Adams-Bashforth-Moulton algorithm, order 4") {
        algorithms::AdamsBashforthMoulton alg {4};
        alg.computeStep(&universe, time.timeStep());
    }
    SECTION("Adams-Bashforth-Moulton algorithm, order 8") {
        algorithms::AdamsBashforthMoulton alg {8};
        alg.computeStep(&universe, time.timeStep());
    }
    REQUIRE(universe[0].position == physics::Vector(0, 0, 0));
    REQUIRE(universe[0].velocity == physics::Vector(0, 0, 0));
}

/*
 * The planet is moving 1km/s in the x-axis and we set the time step to 1s.
 * Therefore, in one computational step, the planet should be 1km further on
 * the x-axis, and the velocity should stay constant, since there are no other
 * planets.
 */
TEST_CASE("Check that a single planet moves correctly", "[algorithms]")
{
    physics::Body moving_planet;
    moving_planet.velocity.set(1, 0, 0);
    physics::UniverseModel universe {moving_planet};
    physics::SimulationTime time;
    time.setTimeStep(1);

    SECTION("Euler's algorithm") {
        algorithms::Euler alg;
        alg.computeStep(&universe, time.timeStep());
    }
    SECTION("Leapfrog algorithm") {
        algorithms::Leapfrog alg;
        alg.computeStep(&universe, time.timeStep());
    }
    SECTION("Runge-Kutta algorithm") {
        algorithms::RungeKutta alg {4};
        alg.computeStep(&universe, time.timeStep());
    }
    SECTION("Adams-Bashforth algorithm, order 4") {
        algorithms::AdamsBashforth alg {4};
        alg.computeStep(&universe, time.timeStep());
    }
    SECTION("Adams-Bashforth algorithm, order 8") {
        algorithms::AdamsBashforth alg {8};
        alg.computeStep(&universe, time.timeStep());
    }
    SECTION("Adams-Bashforth-Moulton algorithm, order 4") {
        algorithms::AdamsBashforthMoulton alg {4};
        alg.computeStep(&universe, time.timeStep());
    }
    SECTION("Adams-Bashforth-Moulton algorithm, order 8") {
        algorithms::AdamsBashforthMoulton alg {8};
        alg.computeStep(&universe, time.timeStep());
    }
    REQUIRE(universe[0].position == physics::Vector(1, 0, 0));
    REQUIRE(universe[0].velocity == physics::Vector(1, 0, 0));
}
