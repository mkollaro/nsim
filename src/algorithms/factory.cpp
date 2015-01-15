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

#include "algorithms/factory.h"

#include <memory>
#include "exceptions.h"
#include "physics/universemodel.h"
#include "physics/simulationtime.h"
#include "algorithms/euler.h"
#include "algorithms/rk4.h"
#include "algorithms/leapfrog.h"
#include "algorithms/adams-bashforth.h"
#include "algorithms/abm.h"


std::unique_ptr<algorithms::Base>
algorithms::factory(const algorithms::Type type)
{
    std::unique_ptr<algorithms::Base> alg;
    switch(type) {
    case algorithms::T_EULER:
        alg.reset(new algorithms::Euler());
        break;
    case algorithms::T_LEAPFROG:
        alg.reset(new algorithms::Leapfrog());
        break;
    case algorithms::T_RK4:
        alg.reset(new algorithms::RungeKutta(4));
        break;
    case algorithms::T_AB4:
        alg.reset(new algorithms::AdamsBashforth(4));
        break;
    case algorithms::T_AB8:
        alg.reset(new algorithms::AdamsBashforth(8));
        break;
    case algorithms::T_ABM4:
        alg.reset(new algorithms::AdamsBashforthMoulton(4));
        break;
    case algorithms::T_ABM8:
        alg.reset(new algorithms::AdamsBashforthMoulton(8));
        break;
    default:
        throw Exception("Unknown algorithm type");
    }
    return alg;
}
