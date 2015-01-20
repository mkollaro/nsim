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

#ifndef __SIMULATIONHISTORY_H__
#define __SIMULATIONHISTORY_H__

#include <vector>
#include <QVector3D>
#include <QOpenGLFunctions>
#include "physics/universemodel.h"
#include "physics/simulationtime.h"
#include "physics/precision.h"


/** Save history of simulation results, so that they can be animated.
 *
 * It should be possible to jump back into some position in history, change the
 * algorithm and overwrite the history (from that position) with the new
 * values. Therefore, some values that aren't necessary for the animation
 * itself have to be stored, e.g. velocity.
 */
class SimulationHistory
{
public:
    SimulationHistory() {}

    /** Save the positions and velocities of all bodies in the universe. The
     * time has to be equal or greater than the previously saved time.
     */
    void save(const physics::UniverseModel& universe,
              const physics::SimulationTime& time);

    /** Restore the positions and velocities of all bodies from the history at
     * the specified index into the physics::UniverseModel.
     */
    void load(const unsigned index,
              physics::UniverseModel *universe,
              physics::SimulationTime *time) const;

    /** Remove all history
     */
    void clear();

    /** Remove history starting from (and including) from_index.
     */
    void clear(unsigned from_index);

    bool empty() {
        return N == 0;
    }

    /** How many universe bodies are saved in the history.
     */
    unsigned universeSize() const {
        return N;
    }

    /** How many simulation states were saved. There is one time stamp for each
     * state, and a position and velocity vector for each universe body.
     */
    unsigned historySize() const {
        return times.size();
    }

    /** Get the position vector of the universe body at body_index (ranging
     * from 0 to SimulationHistory::universeSize) at the Nth saved simulation
     * state, where N is equal to index (ranging from 0 to
     * SimulationHistory::historySize).
     */
    QVector3D bodyPosition(const unsigned body_index,
                           const unsigned index) const;

    /** Get the raw position history of body with body_index (ranging from 0 to
     * SimulationHistory::universeSize). This format is supposed to be suitable
     * to be copied by OpenGL into a buffer - it is possible because
     * std::vector guarantees continuity of data and it can be used as a raw C
     * array. The size of result will be SimulationHistory::historySize() * 3,
     * since each position is a vector of 3 floating point numbers.
     */
    const std::vector<GLfloat>& bodyPositions(const unsigned body_index) const;

private:
    /// Stored in a format usable by an OpenGL buffer.
    std::vector<std::vector<GLfloat> > positions;
    /// Stored in the same format as positions for convenience.
    std::vector<std::vector<GLfloat> > velocities;
    /** Save time information about the simulation, in seconds. The length
     * should always be `positions[x].size()/3`, since 3 numbers are stored for
     * each vertex. Should be always sorted from lowers to highest.
     */
    std::vector<physics::DOUBLE> times;
    /// Number of bodies saved, i.e. number of items in positions.
    unsigned N = 0;
};

#endif  // __SIMULATIONHISTORY_H__
