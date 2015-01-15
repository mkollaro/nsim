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
 * Definition of floating point precision and comparing difference.
 */
#ifndef __PRECISION_H__
#define __PRECISION_H__
#include <cmath>

namespace physics
{
/// Precision for floating point numbers in project.
typedef long double DOUBLE;

/** Maximum difference between two DOUBLE numbers for them to be considered
 *  equal.
 */
const DOUBLE EPSILON = 1e-8;

/**
 * Compare two DOUBLE numbers using EPSILON.
 * @return False if difference between the numbers is greater then EPSILON, true
 *      otherwise.
 */
inline bool equal(DOUBLE first, DOUBLE second)
{
    return !(std::fabs(first - second) > EPSILON);
}
} // namespace

#endif  // __PRECISION_H__
