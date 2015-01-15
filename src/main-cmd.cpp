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
 * Entry point for the CLI.
 */

#include <iostream>
#include <iomanip>
#include <QCoreApplication>
#include <QString>

#include "arguments.h"
#include "projectparser.h"
#include "physics/simulationtime.h"
#include "algorithms/factory.h"
#include "exceptions.h"

using std::cout;
using std::cerr;
using std::endl;

void printComment(physics::UniverseModel& universe);
void printStep(const physics::DOUBLE time,
               const physics::UniverseModel& universe,
               const parser::ProjectSettings& settings,
               const physics::Vector& center);

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("NSim");
    QCoreApplication::setApplicationVersion("1.0");

    try {
        parser::ArgumentsParser arguments(app);
        parser::ProjectParser project(arguments.fileName);
        auto settings = project.getSettings();
        auto universe = project.getUniverseModel();
        physics::SimulationTime time;
        time.setTimeStep(arguments.timeStep);

        auto algorithm = algorithms::factory(arguments.algorithm);
        unsigned saveStateStep = arguments.printInterval / arguments.timeStep;
        if(saveStateStep < 1) saveStateStep = 1;
        const unsigned steps = arguments.simulationTime / arguments.timeStep;

        physics::DOUBLE TOTAL_MASS = 0;
        for(const auto& body : universe) TOTAL_MASS += body.mass;

        printComment(universe);
        // main computation
        for(unsigned i = 0; i <= steps; i++) {
            // print simulation state
            if(i % saveStateStep == 0) {
                if(!arguments.centerToBarycenter) {
                    printStep(time.time(), universe, settings,
                              universe[arguments.centerBodyIndex].position);
                } else {
                    physics::Vector sum;
                    for(const auto& body : universe) {
                        sum += body.position * body.mass;
                    }
                    printStep(time.time(), universe, settings, sum/TOTAL_MASS);
                }
            }
            algorithm->computeStep(&universe, time.timeStep());
            time.updateTime();
        }
    } catch(const Exception& e) {
        cerr << "ERROR: " << e.what() << endl;
        return EXIT_FAILURE;
    } catch(...) {
        cerr << "Unknown error." << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void printComment(physics::UniverseModel& universe)
{
    cout << "# time";
    for(const auto& body : universe) {
        cout << " " << qPrintable(body.name) << "_x";
        cout << " " << qPrintable(body.name) << "_y";
        cout << " " << qPrintable(body.name) << "_z";
    }
    cout << endl;
}

void printStep(const physics::DOUBLE time,
               const physics::UniverseModel& universe,
               const parser::ProjectSettings& settings,
               const physics::Vector& center)
{
    cout << convertUnits(time, parser::TimeUnit::SEC, settings.timeUnit);
    for(const auto& body : universe) {
        cout << std::setprecision(15) << " "
             << parser::convertUnits(body.position - center,
                                     parser::LengthUnit::METER,
                                     settings.lengthUnit);
    }
    cout << endl;
}
