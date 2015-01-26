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
 * Entry point for the GUI.
 */

#include <QtWidgets/QApplication>
#include <QSurfaceFormat>
#include "gui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setOption(QSurfaceFormat::DebugContext);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setSamples(4);
    QSurfaceFormat::setDefaultFormat(format);

    MainWindow w;
    w.show();
    return a.exec();
}
