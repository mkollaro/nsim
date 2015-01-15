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

#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

#include <stdexcept>
#include <QString>

/**
 *  Basic runtime exception with nothing special.
 */
class Exception : public std::runtime_error
{
public:
    explicit Exception(const QString& msg="")
        : runtime_error(msg.toStdString()) {}
};

#endif  // __EXCEPTIONS_H__
