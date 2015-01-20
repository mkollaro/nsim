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
 * Fourth order Runge-Kutta (T_RK4).
 */
#ifndef __RK4ALGORITHM_H__
#define __RK4ALGORITHM_H__

#include <vector>
#include "algorithms/base.h"

namespace algorithms
{
/**
 * The common fourth-order Runge-Kutta numerical integration method.
 * @see http://en.wikipedia.org/wiki/Runge-Kutta_methods
 */
class RungeKutta : public Base
{
public:
    explicit RungeKutta(unsigned order, bool save_step = false)
        : Base(order), save_step_(save_step) {}

    Type getType();

    std::vector<HistoryItem> getLastStepData() {
        return stepData;
    }
protected:
    void computeStepImplementation(physics::UniverseModel *universe,
                                   physics::DOUBLE time_step) override;
    bool save_step_;

    std::vector<HistoryItem> stepData;
};
}  // namespace

#endif  // __RK4ALGORITHM_H__
