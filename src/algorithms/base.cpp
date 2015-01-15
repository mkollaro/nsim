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

#include "algorithms/base.h"

#include <cassert>
#include <cmath>
#include "physics/simulationtime.h"
#include "physics/vector.h"
#include "exceptions.h"

namespace algorithms
{
/**
 * Computes the acceleration of a particle according to Newton's Law of
 * Gravity. The equation for a body of index _i_ in a system containing
 * \f$N=n+1\f$ bodies takes the form
 * \f[
 * \boldsymbol{\ddot{x}}_i = -G\sum_{j=0,\,j \neq i}^n m_j
 *      \frac{\boldsymbol{x}_i - \boldsymbol{x}_j}
 *           {|\boldsymbol{x}_i - \boldsymbol{x}_j|^3}
 *      \quad i = 0,1,2,\,\dots n\,.
 * \f]
 * The symbol
 *      \f$\boldsymbol{\ddot{x}}_i\f$
 * denotes the acceleration of the _i_-th particle, where
 *      \f$\boldsymbol{x}_i\f$
 * is the position vector,
 * _G_ is the universal gravitational constant and _m_ means mass.
 * The function is basically a rewrite of this equation, with
 * `bodyIndex == i`, returning \f$\boldsymbol{\ddot{x}}_i\f$
 *
 * @todo When `distance < MIN_DISTANCE`, tell the simulation that bodies have
 * crashed, so it can save it into the buffer.
 *
 */
void
Base::computeAcceleration(physics::UniverseModel *universe)
{
    for(auto& body : *universe) {
        body.acceleration = computeAcceleration(universe, body, body.position);
    }
}

physics::Vector
Base::computeAcceleration(const physics::UniverseModel *universe,
                          const physics::Body& body,
                          const physics::Vector& position)
{
    physics::Vector result;
    for(const auto& body2 : *universe) {
        if(&body == &body2) continue;

        physics::DOUBLE distance = physics::abs(position - body2.position);
        if(distance < 0.1) throw Exception("crash!");
        result += body2.mass * (position - body2.position) / pow(distance, 3);
    }
    return result * (-G);
}
}  // namespace
