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
 * @file mainwindow.cpp
 */

#include "gui/mainwindow.h"

#include <QtWidgets>
#include <QWidget>
#include <QTimer>
#include "gui/animation.h"
#include "gui/animationstate.h"
#include "algorithms/types.h"
#include "exceptions.h"
#include "simulation.h"
#include "projectparser.h"

/// How long to display a message in the status bar
const unsigned STATUS_MSG_TIMEOUT = 10000;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      simulation_history(new SimulationHistory)
{
    ui->setupUi(this);
    hideDocksTitleBar();
    setWindowTitle(tr(WINDOW_TITLE));
    setUnifiedTitleAndToolBarOnMac(true);

    simulation = new Simulation(this, simulation_history);
    animation = new Animation(this, simulation_history);
    setCentralWidget(animation);
    animation->setFocusPolicy(Qt::StrongFocus);
    animation->setFocus();

    ui->playButton->setEnabled(false);
    populateAlgorithmComboBox();
    connectActions();
}

void
MainWindow::open()
{
    try {
        animation->startOrStop(false);
        /// @todo What about when the program is installed to /usr/..?
        QDir dir("examples");
        QString fileName =
            QFileDialog::getOpenFileName(this, tr("Open Project"), dir.path(),
                                         tr("XML Files (*.xml)"));
        if (fileName.isEmpty())
            return;

        parser::ProjectParser project(fileName);
        simulation->loadUniverse(project.getUniverseModel(),
                                 project.getSettings());
        animation->loadUniverse(project.getUniverseModel(),
                                project.getSettings());

        ui->playButton->setEnabled(true);
        statusBar()->showMessage(tr("File loaded, new universe created"),
                                 STATUS_MSG_TIMEOUT);
    } catch(const Exception& e) {
        QMessageBox::warning(this, tr("Failed to open project"), e.what());
    }
}

void
MainWindow::populateAlgorithmComboBox()
{
    for(unsigned i = 0; i < algorithms::typeName.size(); ++i) {
        ui->algorithmComboBox->insertItem(i, algorithms::typeName[i]);
    }
    ui->algorithmComboBox->setCurrentIndex(algorithms::DEFAULT_TYPE);
}

void
MainWindow::connectActions()
{
    connect(ui->playButton, SIGNAL(clicked(bool)),
            animation, SLOT(startOrStop(bool)));
    connect(animation, SIGNAL(stateChanged(AnimationState)),
            simulation, SLOT(changeComputationIntensity(AnimationState)));

    connect(ui->forwardButton, SIGNAL(clicked()),
            animation, SLOT(increaseSpeed()));
    connect(ui->backwardButton, SIGNAL(clicked()),
            animation, SLOT(decreaseSpeed()));

    // menu
    connect(ui->actionOpenProject, SIGNAL(triggered()),
            this, SLOT(open()));

    // algorithm changes
    connect(ui->setAlgorithmButton, SIGNAL(clicked()),
            this, SLOT(changeAlgorithm()));
}

void
MainWindow::changeAlgorithm()
{
    algorithms::Type type = (algorithms::Type)
                            ui->algorithmComboBox->currentIndex();

    QTime step = ui->timeStepBox->time();
    long double time_step = step.hour()*60*60
                            + step.minute()*60
                            + step.second()
                            + (long double)step.msec()/1000.0;
    // compute from the position that was just being drawn in the animation
    simulation->setAlgorithm(type, time_step,
                             animation->drawnSimulationHistoryIndex());
}

/**
 * Hides the title bar on the algorithm selection dock and the play/pause dock.
 * Looks nicer.
 */
void
MainWindow::hideDocksTitleBar()
{
    QWidget* titleWidget= new QWidget(this);
    ui->algorithmDock->setTitleBarWidget(titleWidget);
}
