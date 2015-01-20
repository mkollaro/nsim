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

#include "algorithms/adams-bashforth.h"

#include <cassert>
#include <vector>
#include "algorithms/rk4.h"
#include "physics/simulationtime.h"


namespace algorithms
{
void
AdamsBashforth::reset()
{
    Base::reset();
    history.clear();
}

void
AdamsBashforth::computeStepImplementation(
    physics::UniverseModel *universe,
    physics::DOUBLE time_step)
{
    if(history.size() < order-1) {
        RungeKutta rk4(4, true);
        rk4.computeStep(universe, time_step);
        history.push_back(rk4.getLastStepData());
        return;
    }
    assert(history.size() == order-1);
    assert(history.back().size() == universe->size());

    physics::DOUBLE h = time_step;
    physics::Vector p_velocity, p_position;
    std::vector<HistoryItem> state;

    // foreach body in universe
    for(unsigned i = 0; i < universe->size(); ++i) {
        physics::Body &body = universe->at(i);

        HistoryItem item;
        item.acc = body.acceleration;
        item.velocity = body.velocity;
        state.push_back(item);

        // The Adams-Bashforth method.
        physics::Vector sum1, sum2;
        unsigned int indexH = 0;
        for(unsigned int indexB = order-1;
                indexB > 0 && indexH < order-1;
                --indexB, ++indexH) {
            sum1 += constants.bashforth(indexB) * history[indexH][i].velocity;
            sum2 += constants.bashforth(indexB) * history[indexH][i].acc;
        }
        sum1 += constants.bashforth(0) * body.velocity;
        sum2 += constants.bashforth(0) * body.acceleration;

        body.position += h * sum1 * constants.divisor();
        body.velocity += h * sum2 * constants.divisor();
    }
    history.push_back(state);
    history.pop_front();
    assert(history.size() == order-1);
}

Type
AdamsBashforth::getType()
{
    assert(order == 4 || order == 8);
    if(order == 4)
        return T_AB4;
    else
        return T_AB8;
}
}  // namespace
