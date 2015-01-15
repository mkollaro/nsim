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
 * Algorithms T_ABM4 and T_ABM8.
 */
#ifndef __ABMALGORITHM_H__
#define __ABMALGORITHM_H__

#include "algorithms/base.h"
#include "algorithms/constants.h"


namespace algorithms
{
/**
 * The Adams-Bashforth-Moulton predictor-corrector integration method.
 * Currently available as 4th and 8th order.
 * @see http://mymathlib.webtrellis.net/diffeq/adams/
 * @see http://en.wikipedia.org/wiki/Linear_multistep_method
 */
class AdamsBashforthMoulton : public Base
{
public:
    AdamsBashforthMoulton(unsigned order)
        : Base(order), constants(order) {}

    void reset();

    Type getType();
protected:
    void computeStepImplementation(physics::UniverseModel *universe,
                                   physics::DOUBLE time_step) override;
private:
    /// History of previous results, initially computed using the 4th order
    /// Runge-Kutta method.
    History history;
    AdamsConstants constants;
};

}  // namespace

#endif  // __ABMALGORITHM_H__
