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
 * @file mainwindow.h
 * @brief The main application window.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QtWidgets/QMainWindow>

#include "ui_mainwindow.h"
#include "simulationhistory.h"

class Simulation;
class Animation;

#define WINDOW_TITLE "NSim N-body Simulator"
#define SETTINGS_FILE "settings.xml"

namespace Ui
{
class MainWindow;
}

/**
 * Main application window.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow() {}

private slots:
    void open();

    void changeAlgorithm();

private:
    Q_DISABLE_COPY(MainWindow)

    /// connects menu actions Qt signals to slots
    void connectActions();

    /// hides the title bar on the dock widgets
    void hideDocksTitleBar();

    void populateAlgorithmComboBox();

    /// user interface created with Qt Designer
    Ui::MainWindow *ui;

    QWidget *window;

    Simulation *simulation;

    Animation *animation;

    std::shared_ptr<SimulationHistory> simulation_history;
};

#endif  // MAINWINDOW_H
