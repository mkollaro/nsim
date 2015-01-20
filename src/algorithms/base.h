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
 * Abstract base class for numerical algorithms.
 * @see Simulation
 */

#ifndef __BASEALGORITHM_H__
#define __BASEALGORITHM_H__

#include <deque>
#include <vector>
#include "physics/precision.h"
#include "physics/universemodel.h"
#include "algorithms/types.h"


namespace algorithms
{
/// Gravitational constant. In \f$m^3 kg^{-1} s^{-2}\f$.
const physics::DOUBLE G = 6.67428e-11;

/** An item in the history of computation results. Used by multi-step
 * methods like the AdamsBashforth.
 * @see History.
 */
struct HistoryItem {
    physics::Vector velocity;
    physics::Vector acc;
};
/** History of computation results, used by multi-step methods.
 * @see AdamsBashforth
 * @see AdamsBashforthMoulton
 */
typedef std::deque<std::vector<HistoryItem> > History;

/** %Base class and interface for all numeric integration algorithms.
 *
 * Example of use:
 * @code
 *      physics::UniverseModel universe;
 *      universe.load(file);
 *      algorithms::Base *algorithm =
 *          new algorithms::RungeKutta(4);
 *      while(counter < 100) {
 *          algorithm->computeStep(&universe, time.timeStep);
 *          time.updateTime();
 *          cout << time.time() << universe;
 *      }
 *      // algorithm change
 *      delete algorithm;
 *      algorithm = new algorithms::Euler();
 *      // continue as if nothing happened
 * @endcode
 */
class Base
{
public:
    /**
     * @param[in] order Numerical integration method order. Default is
     *      zero, which means that it will be ignored and the default order
     *      will be used. Has no use in the Euler method, but should be set
     *      in the Adams methods, since they are available in various
     *      orders.
     */
    explicit Base(unsigned order = 0) : order(order) {}

    virtual ~Base() {}

    /** Compute new positions and velocities in `time + timeStep` and save
     * the new state of Base::universe. Uses the _Template method_ pattern.
     */
    void computeStep(physics::UniverseModel *universe,
                     physics::DOUBLE time_step) {
        if(universe->size() < 1)
            return;
        computeAcceleration(universe);
        computeStepImplementation(universe, time_step);
    }

    /** Reset variables that depend on the model structure, e.g. history in the
     * Adams algorithms. Call it when you want to use a new model.
     */
    virtual void reset() {}

    /// Get the type of algorithm.
    virtual Type getType() = 0;

protected:
    /// The order of the numeric integrator, used only by algorithms that
    /// are available in more orders. Is equal to zero if unused.
    unsigned order;

    virtual void computeStepImplementation(physics::UniverseModel *universe,
                                           physics::DOUBLE time_step) = 0;

    /** Compute acceleration for all bodies in the `universe` and save
     * the results into the acceleration field of each body in the `universe`.
     *
     * @warning Call this method only once per step, right in the beginning. If
     * positions of planets are already modified, the results might be
     * different.
     */
    void computeAcceleration(physics::UniverseModel *universe);

    /** Compute the acceleration of the body in a different position than where
     * it is located at the current simulation time.
     *
     * @warning Be careful when some of the other planets already have their new
     * positions saved in the Base::universe and this is called - it will count
     * with them in the new position and this might not be what you want.
     */
    physics::Vector computeAcceleration(const physics::UniverseModel *universe,
                                        const physics::Body& body,
                                        const physics::Vector& position);
};
}  // namespace

#endif  // __BASEALGORITHM_H__
