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
 * Main simulation class, used to control the simulation, change algorithms and
 * to write into the SimulationHistory.
 */
#ifndef __SIMULATION_H__
#define __SIMULATION_H__

#include <QObject>
#include <memory>
#include "physics/precision.h"
#include "physics/universemodel.h"
#include "physics/simulationtime.h"
#include "algorithms/types.h"
#include "algorithms/base.h"
#include "gui/animationstate.h"
#include "projectparser.h"
#include "simulationhistory.h"
class QTimer;


/// Save universe state into SimulationHistory every X seconds
/// of simulation time.
const unsigned SAVE_STATE_INTERVAL = 60*60*2;  // seconds

/// How many algorithm steps will be computed with one timer tick.
const unsigned DEFAULT_COMPUTE_STEPS_IN_TICK = 200;

/**
 * The simulation runner of the whole application. Calls itself using a
 * timer.
 */
class Simulation : public QObject
{
    Q_OBJECT

public:
    Simulation(QObject *parent,
               std::shared_ptr<SimulationHistory> simulation_history);

    /// Load new universe from specification, reset simulation and its history.
    /// The type of algorithm and time step are not changed.
    void loadUniverse(physics::UniverseModel universe,
                      parser::ProjectSettings settings);

public slots:
    /// Start or stop the simulation according to action.
    void startOrStop(bool action);

    void changeComputationIntensity(AnimationState state);

    /// If the type or step of algorithm is different, change them, load
    /// simulation state from bufferPosition clean the simulation history.
    void setAlgorithm(algorithms::Type type,
                      physics::DOUBLE timeStep,
                      unsigned history_index);

private slots:
    /// Main computation called by timer will run N steps of algorithm and
    /// save the state of universe in every SAVE_STATE_INTERVAL seconds of
    /// simulation time.
    void compute();

private:
    unsigned steps_in_tick;
    std::shared_ptr<SimulationHistory> simulation_history;
    physics::SimulationTime time;
    physics::UniverseModel universe;
    std::unique_ptr<algorithms::Base> algorithm;
    QTimer *timer;
    parser::ProjectSettings settings;


    /// How many steps to take so that approximatly SAVE_STATE_INTERVAL
    /// seconds pass of the the simulation.
    unsigned save_state_step;

    /// Counter of simulation algorithm steps.
    unsigned counter;
};

#endif  // __SIMULATION_H__
