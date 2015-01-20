/*
 * Copyright 2014 Martina Kollarova
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

#include "arguments.h"

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QString>
#include <QDebug>
#include "exceptions.h"

namespace parser
{

QString algorithmNames() {
    QString result;
    for(unsigned i = 0; i < algorithms::shortTypeName.size(); ++i) {
        result += algorithms::shortTypeName[i]
                + "\t(" + algorithms::typeName[i] + ")\n";
    }
    return result;
}

ArgumentsParser::ArgumentsParser(QCoreApplication& app)
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Compute paths of celestial bodies and"
                                     " print their positions in CSV format.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOptions({
        {{"f", "file"},
            QCoreApplication::translate("main",
                                        "Input XML file with universe model."),
            QCoreApplication::translate("main", "file")},
        {{"a", "algorithm"},
            QCoreApplication::translate("main",
                "Numeric integration algorithm. Default is 'rk4',"
                " possible options are:\n") + algorithmNames(),
            QCoreApplication::translate("main", "algorithm")},
        {{"t", "time"},
            QCoreApplication::translate("main",
                "Desired length of the simulation in seconds. Default is ")
                + QString::number(DEFAULT_TIME) + ".",
            QCoreApplication::translate("main", "seconds")},
        {{"s", "step"},
            QCoreApplication::translate("main",
                "Time step of the algorithm in seconds (floating-point)."
                " Default is ") + QString::number(DEFAULT_STEP) + ".",
            QCoreApplication::translate("main", "seconds")},
        {{"p", "print-step"},
            QCoreApplication::translate("main",
                "Approximate time interval between printing out the simulation"
                " state, in seconds. Should be larger than the time step."
                " Default is ") + QString::number(DEFAULT_PRINT_INTERVAL) + ".",
            QCoreApplication::translate("main", "seconds")},
        {{"c", "center-to"},
            QCoreApplication::translate("main",
                "Index of the celestial body in the project file to which the"
                " output should be centered - that body will always have the"
                " coordinates [0, 0, 0]. Default is 0 (the first body)."),
            QCoreApplication::translate("main", "index")},
        {{"b", "center-to-barycenter"},
            QCoreApplication::translate("main",
                "Center the output so that the barycenter (center of mass) has"
                " coordinates [0, 0, 0]. Off by default.")}
    });
    parser.process(app);
    file_name = parser.value("file");
    if(file_name.isEmpty())
        throw Exception("No input file name specified.");

    QString algorithmStr = parser.value("algorithm");
    if(!algorithmStr.isEmpty()) {
        bool valid = false;
        for(unsigned i = 0; i < algorithms::shortTypeName.size(); ++i) {
            if(algorithmStr == algorithms::shortTypeName[i]) {
                valid = true;
                algorithm = (algorithms::Type) i;
                break;
            }
        }
        if(valid == false)
            throw Exception("Invalid algorithm type.");
    }

    if(parser.isSet("time"))
        simulation_time = parser.value("time").toDouble();
    if(parser.isSet("step"))
        time_step = parser.value("step").toDouble();
    if(parser.isSet("print-step"))
        print_interval = parser.value("print-step").toDouble();

    if(simulation_time < time_step)
        throw Exception("The simulation time has to be greater then the"
                        " time step.");
    if(time_step > print_interval)
        throw Exception("The time step should be smaller or equal to the"
                        " print interval.");

    if(parser.isSet("center-to"))
        center_body_index = parser.value("center-to").toUInt();
    center_to_barycenter = parser.isSet("center-to-barycenter");

    if(center_to_barycenter && center_body_index > 0)
        throw Exception("The coordinates can be eighter centered to a"
                        " body or to the barycenter, not both.");

    qDebug() << "file: " << file_name;
    qDebug() << "time: " << simulation_time;
    qDebug() << "time step: " << time_step;
    qDebug() << "print interval: " << print_interval;
    qDebug() << "algorithm: " << algorithms::typeName[algorithm];
}
}  // namespace
