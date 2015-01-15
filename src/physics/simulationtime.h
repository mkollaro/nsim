/*
 * Copyright 2010 Martina Kollarova
 *
 * This file is part of NSim.
 *
 * NSim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 2 of the License, or (at your option) any later
 * version.
 *
 * NSim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * NSim. If not, see http://www.gnu.org/licenses/.
 */

/**
 * @file
 */

#ifndef __PHYSICS_SIMULATIONTIME_H__
#define __PHYSICS_SIMULATIONTIME_H__

#include "physics/precision.h"

namespace physics
{
/// Default time step of the Simulation algorithm in seconds.
const unsigned DEFAULT_STEP = 60*60;

/// Information about the Simulation - time, time step, etc.
/// @todo Counter overflow? How long can the simulation go on if the step is
/// very small and int is 32 bytes?
class SimulationTime
{
    physics::DOUBLE t = 0;
    physics::DOUBLE initTime = 0;
    physics::DOUBLE step = DEFAULT_STEP;
    unsigned counter = 0;

public:
    SimulationTime() {}

    /// Time from start of simulation in seconds.
    physics::DOUBLE time() const {
        return initTime + t;
    }

    /// Increases time by the time step.
    void updateTime() {
        counter++;
        // t += step would be less precise - errors would accumulate
        t = step * counter;
    }

    /// Sets the simulation time (in seconds) - use when loading Simulation
    /// position from Buffer.
    void setTime(physics::DOUBLE time) {
        initTime = time;
        t = 0;
        counter = 0;
    }

    /// Algorithm time step (in seconds).
    physics::DOUBLE timeStep() const {
        return step;
    }

    /// Set computation time step - how precise it should be (in seconds).
    void setTimeStep(physics::DOUBLE timeStep) {
        step = timeStep;
    }
};
}  // namespace

#endif  // __PHYSICS_SIMULATIONTIME_H__
