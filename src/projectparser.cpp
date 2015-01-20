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
#include "projectparser.h"

#include <QDomDocument>
#include <QFile>
#include <QStringList>
#include <QDebug>
#include "exceptions.h"

namespace parser
{
physics::DOUBLE
convertUnits(physics::DOUBLE value, LengthUnit from, LengthUnit to) {
    return convertUnits(physics::Vector(value, 0, 0), from, to)[0];
}

physics::Vector
convertUnits(physics::Vector value, LengthUnit from, LengthUnit to) {
    // convert to meters first
    if(from == LengthUnit::KM)
        value *= 1000;
    else if(from == LengthUnit::AU)
        value *= ASTRON_UNITS;

    // convert to desired unit
    if(to == LengthUnit::KM)
        return value / 1000;
    else if(from == LengthUnit::AU)
        return value / ASTRON_UNITS;
    else
        return value;
}

physics::DOUBLE
convertUnits(physics::DOUBLE value, TimeUnit from, TimeUnit to) {
    return convertUnits(physics::Vector(value, 0, 0), from, to)[0];
}

physics::Vector
convertUnits(physics::Vector value, TimeUnit from, TimeUnit to) {
    // convert to seconds first
    if(from == TimeUnit::DAY)
        value *= 3600;

    // convert to desired unit
    if(to == TimeUnit::DAY)
        return value / 3600;
    else
        return value;
}

ProjectParser::ProjectParser(QFile *file)
{
    parse(file);
}

ProjectParser::ProjectParser(const QString& fileName)
{
    if (fileName.isEmpty())
        throw Exception("No file name given.");

    QFile file(fileName);
    parse(&file);
}

void
ProjectParser::parse(QFile *file)
{
    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        QString msg = "Can't read file " + file->fileName()
                      + ": " + file->errorString();
        throw Exception(msg);
    }
    QDomDocument doc("world");

    QString errorString;
    int errorLine;
    int errorColumn;
    if (!doc.setContent(file, false, &errorString, &errorLine,
                        &errorColumn)) {
        file->close();
        QString msg = "Parse error at line ";
        msg += errorLine + ", ";
        msg += "column " + errorColumn;
        msg += ": " + errorString;
        throw Exception(msg);
    }
    file->close();

    QDomElement root = doc.documentElement();
    if(root.tagName() != "nsim") {
        std::cerr << "root tag is not \"nsim\"\n";
        throw Exception("bad xml file");
    }

    QDomNode node = root.firstChild();
    while(!node.isNull()) {
        QString tag = node.toElement().tagName();
        if(tag == "settings") {
            parseSettings(node.toElement());
        } else if(tag == "universe") {
            parseUniverse(node.toElement());
        } else {
            throw Exception("unknown tag " + tag);
        }
        node = node.nextSibling();
    }

    // always use meters, seconds and kilogram in internal representation
    for(auto& body : universe) {
        body.position = convertUnits(body.position,
                                     settings.length_unit, LengthUnit::METER);
        body.velocity = convertUnits(body.velocity,
                                     settings.length_unit, LengthUnit::METER);
        body.velocity = convertUnits(body.velocity,
                                     settings.time_unit, TimeUnit::SEC);
        body.radius = convertUnits(body.radius,
                                   settings.length_unit, LengthUnit::METER);
    }
}

void
ProjectParser::parseUnits(const QDomElement& element)
{
    QDomNode node = element.firstChild();
    QDomElement e;

    while(!node.isNull()) {
        e = node.toElement();
        QString tag = e.tagName();
        QString text;
        if(tag == "mass") {
        } else if(tag == "length" || tag == "distance") {
            text = e.text().trimmed();
            qDebug() << "parser: length unit is" << text;
            if(text == "m" || text == "meter") {
                settings.length_unit = LengthUnit::METER;
            } else if(text == "km") {
                settings.length_unit = LengthUnit::KM;
            } else if(text == "AU") {
                settings.length_unit = LengthUnit::AU;
            } else {
                throw Exception("Unknown length unit " + text);
            }
        } else if(tag == "time") {
            text = e.text().trimmed();
            qDebug() << "parser: time unit is" << text;
            if(text == "s" || text == "sec" || text == "seconds") {
                settings.time_unit = TimeUnit::SEC;
            } else if(text == "day" || text == "days") {
                settings.time_unit = TimeUnit::DAY;
            } else {
                throw Exception("Unknown time unit " + text);
            }
        }
        node = node.nextSibling();
    }
}

void
ProjectParser::parseSettings(const QDomElement& element)
{
    QDomNode node = element.firstChild();
    QDomElement e;

    while(!node.isNull()) {
        e = node.toElement();
        QString tag = e.tagName();
        if(tag == "start-date-time") {
        } else if(tag == "units") {
            parseUnits(e);
        } else if(tag == "body-color") {
            throw Exception("Unimplemented tag 'body-color'");
        } else if(tag == "trajectory-color") {
            throw Exception("Unimplemented tag 'trajectory-color'");
        } else if(tag == "visible-size-multiplier") {
            settings.visible_size_multiplier = e.text().toInt();
        } else if(tag == "visual-center") {
            throw Exception("Unimplemented tag 'visual-center'");
        } else {
            throw Exception("Unknown tag in settings - " + tag);
        }
        node = node.nextSibling();
    }
}

void
ProjectParser::parseUniverse(const QDomElement& element)
{
    QDomNode node = element.firstChild();
    QDomElement e;

    while(!node.isNull()) {
        e = node.toElement();
        if(e.tagName() != "body") {
            throw Exception("Unknown tag in <universe>.");
        }
        physics::Body body;
        body.name = e.attribute("name", "body");
        body.visible_size_multiplier = 1;
        body.acceleration = physics::Vector(0, 0, 0);
        QDomNode param = e.firstChild();
        QString tag;
        physics::Vector *v = 0;
        while(!param.isNull()) {
            e = param.toElement();
            tag = e.tagName();
            if(tag == "radius") {
                body.radius = e.text().toDouble();
            } else if(tag == "visible-size-multiplier") {
                body.visible_size_multiplier = e.text().toInt();
            } else if(tag == "mass") {
                body.mass = e.text().toDouble();
            } else if(tag == "position") {
                v = &body.position;
            } else if(tag == "velocity") {
                v = &body.velocity;
            } else {
                throw Exception("unknown tag");
            }

            if(tag == "position" || tag == "velocity") {
                // @bug weird when the user uses something different for a
                // separator
                QStringList list =  e.text().split(",");
                if(list.size() > 3) {
                    throw Exception("vector is max 3D");
                }
                for(int i = 0; i < list.size(); ++i) {
                    (*v)[i] = list[i].toDouble();
                }
            }
            param = param.nextSibling();
        }

        universe.push_back(body);
        node = node.nextSibling();
    }
}
}  // namespace
