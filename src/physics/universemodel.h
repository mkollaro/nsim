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
 * The Universe state in one moment - positions and velocities, etc.
 */

#ifndef __UNIVERSEMODEL_H__
#define __UNIVERSEMODEL_H__

#include <vector>
#include <QString>
#include "physics/vector.h"


namespace physics
{
/**
 * One celestial body, defined by its position and velocity and other info.
 * All that the simulation needs to know about the body.
 * @see UniverseModel
 */
struct Body {
    physics::Vector position;
    physics::Vector velocity;
    physics::Vector acceleration;
    physics::DOUBLE mass = 1;
    /// Used to compute the distance between bodies.
    physics::DOUBLE radius = 1;
    QString name = "<name>";

    /**
     * Visual multiplier of size. If equal to 1, the apparent size
     * of the celestial body will be normal, if equal to 2, it will be
     * shown 2x larger. Used to make a very small body visible.
     */
    int visibleSizeMultiplier = 1;
};

/**
 * Contains all the celestial bodies with their positions and velocities at
 * one point of time.
 */
typedef std::vector<Body> UniverseModel;

}  // namespace

#endif  // __UNIVERSEMODEL_H__
