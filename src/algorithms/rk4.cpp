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

#include "algorithms/rk4.h"

#include <cassert>
#include "physics/simulationtime.h"

namespace algorithms
{
void
RungeKutta::computeStepImplementation(physics::UniverseModel *universe,
                                      physics::DOUBLE time_step)
{
    if(save_step_) stepData.clear();
    physics::DOUBLE h = time_step;
    physics::Vector k1, k2, k3, k4, l1, l2, l3, l4;
    physics::Vector k, l;
    for(auto& body : *universe) {
        k1 = body.velocity;
        l1 = body.acceleration;

        k2 = body.velocity + h*l1/2;
        l2 = computeAcceleration(universe, body, body.position + h*k1/2);

        k3 = body.velocity + h*l2/2;
        l3 = computeAcceleration(universe, body, body.position + h*k2/2);

        k4 = body.velocity + h*l3;
        l4 = computeAcceleration(universe, body, body.position + h*k3);

        k = (k1 + 2*k2 + 2*k3 + k4)/6;
        l = (l1 + 2*l2 + 2*l3 + l4)/6;
        body.position += h*k;
        body.velocity += h*l;
        if(save_step_) {
            HistoryItem item;
            item.velocity = k1;
            item.acc = l1;
            stepData.push_back(item);
        }
    }
}

Type
RungeKutta::getType()
{
    // don't forget to implement this when you add available orders of method
    assert(order == 4);
    return T_RK4;
}
}  // namespace
