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
 * Types of implemented algorithms and their names.
 *
 * @todo Rewrite all of this with some safer, modern version of enum.
 */

#ifndef __ALGORITHM_TYPES_H__
#define __ALGORITHM_TYPES_H__

#include <vector>
#include <QString>

namespace algorithms
{
/**
 * Types of numerical integration algorithms.
 * @note The order of the values has to correspond to algorithms::typeName.
 * @todo T_undef
 */
enum Type {
    T_EULER = 0,
    T_LEAPFROG,
    T_RK4,
    T_AB4,
    T_AB8,
    T_ABM4,
    T_ABM8
};

/// Default algorithm to use.
const Type DEFAULT_TYPE = T_RK4;

/**
 * Names of numerical integration algorithms used.
 * @note The order of the names has to correspond to the
 *      `enum` algorithms::Type.
 */
const std::vector<QString> typeName {
    "Euler Method",
    "Leapfrog",
    "Runge-Kutta 4",
    "Adams-Bashforth 4",
    "Adams-Bashforth 8",
    "Adams predictor-corrector 4",
    "Adams predictor-corrector 8"
};

/**
 * Short names of numerical integration algorithms, useful when parsing
 * command line arguments for example.
 * @note The order of the names has to correspond to the
 *      `enum` algorithms::Type.
 */
const std::vector<QString> shortTypeName {
    "euler",
    "leapfrog",
    "rk4",
    "ab4",
    "ab8",
    "abm4",
    "abm8"
};
}

#endif  // __ALGORITHM_TYPES_H__
