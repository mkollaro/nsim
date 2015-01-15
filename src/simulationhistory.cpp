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

#include "simulationhistory.h"

#include <algorithm>
#include <QDebug>
#include "exceptions.h"


void
SimulationHistory::save(const physics::UniverseModel& universe,
                        const physics::SimulationTime& time)
{
    if (universe.size() == 0)
        throw Exception("The universe model is empty");
    if (N > 0 && universe.size() != N)
        throw Exception("The universe model size has changed");
    if (times.size() > 0 && time.time() < times.back()) {
        QString msg = "Attempted to save old simulation state to history";
        msg += " (time = " + (int) time.time();
        msg += ")";
        throw Exception(msg);
    }
    // first save
    if (N == 0) {
        N = universe.size();
        positions.resize(N);
        velocities.resize(N);
    }

    for(unsigned i = 0; i < N; i++) {
        positions[i].push_back(universe[i].position.x());
        positions[i].push_back(universe[i].position.y());
        positions[i].push_back(universe[i].position.z());
        velocities[i].push_back(universe[i].velocity.x());
        velocities[i].push_back(universe[i].velocity.y());
        velocities[i].push_back(universe[i].velocity.z());
    }
    times.push_back(time.time());
    Q_ASSERT(positions[0].size() == times.size() * 3);
    Q_ASSERT(velocities[0].size() == times.size() * 3);
}

void
SimulationHistory::load(const unsigned index,
                        physics::UniverseModel *universe,
                        physics::SimulationTime *time) const
{
    if (universe == nullptr || time == nullptr)
        throw Exception("Null pointer exception");
    if (times.size() == 0)
        throw Exception("No data were saved yet into the history");
    if (times.size() <= index)
        throw Exception("Simulation history is smaller than requested index");

    for(unsigned i = 0; i < N; i++) {
        universe->at(i).position.set(positions[i][index * 3],
                                     positions[i][index * 3 + 1],
                                     positions[i][index * 3 + 2]);
        universe->at(i).velocity.set(velocities[i][index * 3],
                                     velocities[i][index * 3 + 1],
                                     velocities[i][index * 3 + 2]);
    }
    time->setTime(times[index]);
}

void
SimulationHistory::clear()
{
    positions.clear();
    velocities.clear();
    times.clear();
    N = 0;
}

void
SimulationHistory::clear(unsigned from_index)
{
    if (from_index >= times.size()) return;
    if (empty()) return;
    if (from_index == 0) {
        clear();
        return;
    }
    for (auto& p: positions) {
        Q_ASSERT(p.size() > from_index * 3);
        p.erase(p.begin() + (from_index * 3), p.end());
    }
    for (auto& v: velocities) {
        Q_ASSERT(v.size() > from_index * 3);
        v.erase(v.begin() + (from_index * 3), v.end());
    }
    times.erase(times.begin() + from_index, times.end());
    Q_ASSERT(positions[0].size() == times.size() * 3);
    Q_ASSERT(velocities[0].size() == times.size() * 3);
}

QVector3D
SimulationHistory::bodyPosition(const unsigned body_index,
                                const unsigned index) const
{
    if (body_index >= N)
        throw Exception("Requested body index out of range in history");
    if (index >= times.size())
        throw Exception("Requested index out of range in history");
    return QVector3D(positions[body_index][index * 3],
                     positions[body_index][index * 3 + 1],
                     positions[body_index][index * 3 + 2]);
}

const std::vector<GLfloat>&
SimulationHistory::bodyPositions(const unsigned body_index) const
{
    if (body_index >= N)
        throw Exception("Index out of range");
    return positions[body_index];
}
