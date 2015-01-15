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
 * Command line argument parser.
 */
#ifndef __ARGUMENTS_H__
#define __ARGUMENTS_H__

#include <QString>
#include "algorithms/types.h"

class QCoreApplication;

namespace parser
{
/** Default value for ArgumentsParser::simulationTime */
static const unsigned DEFAULT_TIME = 800;

/// Default value ArgumentsParser::timeStep
static const double DEFAULT_STEP = 10;

/// Default value for ArgumentsParser::printInterval
static const unsigned DEFAULT_PRINT_INTERVAL = 200;

struct ArgumentsParser {
    ArgumentsParser(QCoreApplication& app);

    /// Required input file name set with `-f` with universe definition
    /// at `t = 0`.
    QString fileName;

    /// Requested duration of simulation in seconds.
    /// @exception ParserException if smaller than
    ///     ArgumentsParser::timeStep.
    double simulationTime = DEFAULT_TIME;

    /// Time step of the algorithm.
    /// @exception ParserException if negative or zero.
    double timeStep = DEFAULT_STEP;

    /// Approx. time interval between printing out the simulation state.
    /// @exception ParserException if smaller than
    ///     ArgumentsParser::timeStep.
    double printInterval = DEFAULT_PRINT_INTERVAL;

    /// Which numerical integration algorithm to use.
    /// @exception ParserException if not part of
    ///     algorithms::shortTypeName.
    algorithms::Type algorithm = algorithms::DEFAULT_TYPE;

    /// Should output coordinates be centered to the barycenter (center of
    /// mass)?
    /// @exception ParserException if centerBodyIndex was given too.
    bool centerToBarycenter = false;

    /// Index of the body to which the output coordinates should be
    /// centered. The index is the number of the body given in the project
    /// input file and starts from 0. Default is 0 - centered to the first
    /// body in the input file.
    /// @exception ParserException if centerToBarycenter was given too.
    unsigned centerBodyIndex = 0;
};
}  // namespace

#endif  // __ARGUMENTS_H__
