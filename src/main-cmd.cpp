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

void printComment(const physics::UniverseModel& universe);
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
        parser::ProjectParser project(arguments.file_name);
        auto settings = project.getSettings();
        auto universe = project.getUniverseModel();
        physics::SimulationTime time;
        time.setTimeStep(arguments.time_step);

        auto algorithm = algorithms::factory(arguments.algorithm);
        unsigned save_state_step = arguments.print_interval/arguments.time_step;
        if(save_state_step < 1) save_state_step = 1;
        const unsigned steps = arguments.simulation_time/arguments.time_step;

        physics::DOUBLE TOTAL_MASS = 0;
        for(const auto& body : universe) TOTAL_MASS += body.mass;

        printComment(universe);
        // main computation
        for(unsigned i = 0; i <= steps; i++) {
            // print simulation state
            if(i % save_state_step == 0) {
                if(!arguments.center_to_barycenter) {
                    printStep(time.time(), universe, settings,
                              universe[arguments.center_body_index].position);
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

void printComment(const physics::UniverseModel& universe)
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
    cout << convertUnits(time, parser::TimeUnit::SEC, settings.time_unit);
    for(const auto& body : universe) {
        cout << std::setprecision(15) << " "
             << parser::convertUnits(body.position - center,
                                     parser::LengthUnit::METER,
                                     settings.length_unit);
    }
    cout << endl;
}
