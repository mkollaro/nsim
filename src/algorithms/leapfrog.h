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
 * Second order Leapfrog integration method (T_LEAPFROG).
 */
#ifndef __LEAPFROGALGORITHM_H__
#define __LEAPFROGALGORITHM_H__

#include "algorithms/base.h"

namespace algorithms
{
/**
 * %Leapfrog Method - second order numerical integration method, imprecise.
 * @see http://en.wikipedia.org/wiki/Leapfrog_integration
 */
class Leapfrog : public Base
{
public:
    Leapfrog(unsigned order = 0)
        : Base(order) {}

    Type getType() {
        return T_LEAPFROG;
    }
protected:
    void computeStepImplementation(physics::UniverseModel *universe,
                                   physics::DOUBLE time_step) override;
};
}  // namespace

#endif  // __LEAPFROGALGORITHM_H__
