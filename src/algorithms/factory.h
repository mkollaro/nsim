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
 * Factory function to create an algorithm object of a specified type.
 */

#ifndef __ALGORITHMSFACTORY_H__
#define __ALGORITHMSFACTORY_H__

#include <memory>
#include "algorithms/base.h"

/**
 * @namespace algorithms Numerical Integration Algorithms
 *
 * A set of numeric integration methods used to compute positions and
 * velocities of the particles in the universe. The algorithms are used by the
 * Simulation to get the next state of the universe in `time + time
 * step`. Since the algorithm should be able to change during the simulation,
 * the strategy design pattern was used.
 *
 * @dotfile strategy.dot
 */
namespace algorithms
{
/// Shortcut for getting the algorithm instance based on its type
std::unique_ptr<algorithms::Base> factory(const algorithms::Type type);
}

#endif  // __ALGORITHMSFACTORY_H__
