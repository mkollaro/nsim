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
 * Contains constans for the numerical integrators, for example the Adams
 * methods.
 */
#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#include "physics/precision.h"

namespace algorithms
{
/**
 * Provides access to constants used by the Adams-Bashforth and
 * Adams-Moulton integration methods of a given order.
 * @see AdamsBashforth
 * @see AdamsBashforthMoulton
 * @see http://mymathlib.webtrellis.net/diffeq/adams/ the constants source
 */
class AdamsConstants
{
public:
    explicit AdamsConstants(unsigned order);

    physics::DOUBLE bashforth(const unsigned index) const;
    physics::DOUBLE moulton(const unsigned index) const;
    physics::DOUBLE divisor() const;

private:
    physics::DOUBLE const *b;
    physics::DOUBLE const *m;
    physics::DOUBLE const *d;
    unsigned order;
};
}  // namespace

#endif  // __CONSTANTS_H__
