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

#ifndef __PROJECTPARSER_H__
#define __PROJECTPARSER_H__

#include <QDateTime>
#include <QColor>
#include "physics/universemodel.h"

class QDateTime;
class QDomElement;
class QFile;
class QString;

/**
 * @namespace parser Parsers of different types, like the CLI arguments or the
 * XML files with data.
 */
namespace parser
{
enum class MassUnit { KG };
enum class LengthUnit { METER, KM, AU };
enum class TimeUnit { SEC, DAY };

// AU in meters
const physics::DOUBLE ASTRON_UNITS = 1.495978707e11;

physics::DOUBLE convertUnits(physics::DOUBLE value,
                             LengthUnit from, LengthUnit to);

physics::Vector convertUnits(physics::Vector value,
                             LengthUnit from, LengthUnit to);

physics::DOUBLE convertUnits(physics::DOUBLE value,
                             TimeUnit from, TimeUnit to);

physics::Vector convertUnits(physics::Vector value,
                             TimeUnit from, TimeUnit to);

/**
 * Parameters given to the simulation and animation.
 * @see Simulation
 */
struct ProjectSettings {
    /// Start time of the simulation. Will be invalid if not set.
    QDateTime startDateTime;

    /// Which units to use when getting input or printing output (internally,
    /// kilograms are always used)
    MassUnit massUnit = MassUnit::KG;

    /// Which units to use when getting input or printing output (internally,
    /// meters are always used)
    LengthUnit lengthUnit = LengthUnit::METER;

    /// Which units to use when getting input or printing output (internally,
    /// seconds are always used)
    TimeUnit timeUnit = TimeUnit::SEC;

    /// Index of a body in UniverseModel to which the animation will be
    /// centered. Default is 0 (the first body in the project file).
    unsigned visualCenter = 0;

    /// Default color of all the celestial bodies.
    QColor bodyColor = Qt::gray;

    /// Default color of all the trajectories. Will be Qt::green if not set.
    QColor orbitColor = Qt::red;

    /// Default vizual multiplier of size. If equal to 1, the apparent sizes
    /// of celestial bodies will be normal, if equal to 2, they will be
    /// shown 2x larger. Used to make very small bodies visible.
    int visibleSizeMultiplier = 1;
};

/**
 * Parser of the input XML file with the model specifications.
 */
class ProjectParser
{
public:
    explicit ProjectParser(QFile *file);
    explicit ProjectParser(const QString& fileName);

    physics::UniverseModel getUniverseModel() const {
        return universe;
    }
    ProjectSettings getSettings() const {
        return settings;
    }

private:
    physics::UniverseModel universe;
    ProjectSettings settings;

    void parse(QFile *file);
    void parseUniverse(const QDomElement& element);
    void parseSettings(const QDomElement& element);
    void parseUnits(const QDomElement& element);
};
}  // namespace

#endif  // __PROJECTPARSER_H__
