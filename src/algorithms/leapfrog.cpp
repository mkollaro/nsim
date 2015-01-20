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

#include "algorithms/leapfrog.h"

#include <cassert>
#include "physics/simulationtime.h"

namespace algorithms
{
void
Leapfrog::computeStepImplementation(physics::UniverseModel *universe,
                                    physics::DOUBLE time_step)
{
    for(auto& body : *universe) {
        body.position += time_step   * body.velocity
                         + time_step/2 * body.acceleration;
        body.velocity += time_step * (body.acceleration
                                      + computeAcceleration(universe,
                                              body, body.position))/2;
    }
}
}  // namespace
