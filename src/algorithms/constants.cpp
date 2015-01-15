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

#include "algorithms/constants.h"

#include <cassert>
#include "exceptions.h"

using physics::DOUBLE;

namespace algorithms
{
// Constants for the 8th order Adams-Bashforth and Adamds-Moulton methods.
const DOUBLE b8[] = { 434241.0, -1152169.0, 2183877.0, -2664477.0, 2102243.0,
                      -1041723.0, 295767.0, -36799.0
                    };
const DOUBLE m8[] = { 36799.0, 139849.0, -121797.0, 123133.0, -88547.0,
                      41499.0, -11351.0, 1375.0
                    };
const DOUBLE d8 = 1.0 / 120960.0;

// 4th order
const DOUBLE b4[] = {  55.0, -59.0, 37.0, -9.0 };
const DOUBLE m4[] = {  9.0, 19.0, -5.0, 1.0 };
const DOUBLE d4 = 1.0 / 24.0;


AdamsConstants::AdamsConstants(unsigned order)
{
    assert(order == 4 || order == 8);
    this->order = order;

    if(order == 8) {
        b = &b8[0];
        m = &m8[0];
        d = &d8;
    } else if (order == 4) {
        b = &b4[0];
        m = &m4[0];
        d = &d4;
    }
}

DOUBLE
AdamsConstants::bashforth(const unsigned index) const
{
    if(index >= order)
        throw Exception("Bad use of adams constants, index out of range");
    return b[index];
}

DOUBLE
AdamsConstants::moulton(const unsigned index) const
{
    if(index >= order)
        throw Exception("Bad use of adams constants, index out of range");
    return m[index];
}

DOUBLE
AdamsConstants::divisor() const
{
    return *d;
}
}  // namespace
