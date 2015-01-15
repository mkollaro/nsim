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

#include "simulation.h"

#include <QTimer>
#include <QFile>
#include <QDebug>
#include <iostream>
#include <cassert>
#include "algorithms/factory.h"
#include "physics/simulationtime.h"


Simulation::Simulation(QObject *parent,
                       std::shared_ptr<SimulationHistory> simulation_history)
    :QObject(parent), simulation_history(simulation_history), counter(0)
{
    assert(simulation_history != nullptr);

    saveStateStep = SAVE_STATE_INTERVAL / time.timeStep();
    if(saveStateStep < 1) saveStateStep = 1;

    algorithm = algorithms::factory(algorithms::DEFAULT_TYPE);

    stepsInTick = DEFAULT_COMPUTE_STEPS_IN_TICK;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(compute()));
}

void
Simulation::changeComputationIntensity(AnimationState state)
{
    if(state == PLAYING) {
        stepsInTick = DEFAULT_COMPUTE_STEPS_IN_TICK;
        timer->start(0);
        qDebug() << "Simulation running at normal speed";
    } else if(state == PAUSED) {
        stepsInTick = DEFAULT_COMPUTE_STEPS_IN_TICK/2;
        timer->start(100);
        qDebug() << "Simulation running slower";
    } else if(state == BUFFERING) {
        stepsInTick = DEFAULT_COMPUTE_STEPS_IN_TICK*2;
        timer->start(0);
        qDebug() << "Simulation running at full speed, buffering";
    }
    // ignore other
}
void
Simulation::compute()
{
    assert(universe.size() > 0);
    while(true) {
        algorithm->computeStep(&universe, time.timeStep());
        time.updateTime();

        counter++;
        if(counter % saveStateStep == 0) {
            simulation_history->save(universe, time);
        }
        if(counter % stepsInTick == 0)
            return;
    }
}

void
Simulation::loadUniverse(physics::UniverseModel universe,
                         parser::ProjectSettings settings)
{
    this->universe = universe;
    this->settings = settings;
    time.setTime(0);
    simulation_history->clear();
    algorithm->reset();
    counter = 0;

    // save initial positions into buffer
    simulation_history->save(universe, time);
    changeComputationIntensity(PAUSED);
}

void
Simulation::startOrStop(bool action)
{
    if(action == 1)
        timer->start(0);
    else
        timer->stop();
}

void
Simulation::setAlgorithm(algorithms::Type type,
                         physics::DOUBLE timeStep,
                         unsigned history_index)
{
    bool load_history = false;
    assert(timeStep > 0);

    if(type != algorithm->getType()) {
        algorithm = std::move(algorithms::factory(type));
        load_history = true;
        qDebug() << "algorithm changed to " << algorithms::typeName[type];
    }
    if(!physics::equal(timeStep, time.timeStep())) {
        time.setTimeStep(timeStep);
        saveStateStep = SAVE_STATE_INTERVAL / time.timeStep();
        if(saveStateStep == 0) saveStateStep = 1;
        load_history = true;
    }
    if(load_history && simulation_history->historySize() > 0) {
        simulation_history->load(history_index, &universe, &time);
        simulation_history->clear(history_index);
    }
    time.timeStep();
}
