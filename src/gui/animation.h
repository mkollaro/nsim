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
#ifndef __ANIMATION_H__
#define __ANIMATION_H__


#include <memory>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLDebugLogger>
#include <QBasicTimer>

#include "physics/universemodel.h"
#include "simulationhistory.h"
#include "projectparser.h"
#include "gui/animationstate.h"
#include "gui/sphere.h"
#include "gui/orbit.h"

/**
 * OpenGL animation of the bodies movement.
 * Updates itself using a timer every few miliseconds.
 */
class Animation : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    /// @param history Simulation results that will be animated.
    Animation(QWidget *parent,
              std::shared_ptr<SimulationHistory> const history);
    ~Animation();

    /// Load new universe informations and call Animation::reset.
    void loadUniverse(physics::UniverseModel u, parser::ProjectSettings p);

    /// Get the position in the SimulationHistory that is currently being drawn.
    unsigned drawnSimulationHistoryIndex()
        { return history_index; }


signals:
    /** Emited when buffering has started or stopped. Buffering happens when
     * the animation reaches the end of the simulation history and has to wait
     * for further results.
     */
    void stateChanged(AnimationState state);

public slots:
    /// Start the animation if `run` is set to true, stop otherwise.
    void startOrStop(bool start);

    /// Reads simulation history from beginning, resets opengl camera position
    void reset();

    /// Move trough the simulation history faster.
    void increaseSpeed();
    /// Move trough the simulation history slower.
    /// @todo 0.5 speed? interpolation? speed = 0?
    void decreaseSpeed();

    void onMessageLogged(QOpenGLDebugMessage message);

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void timerEvent(QTimerEvent *e) override;

    void initializeLogging();
    void initializeShaders();
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    // helpers
    bool hasData()
        { return !simulation_history->empty(); }
    float computeUniverseRadius();
    void printGlErrors();
    QString glErrorToString(GLenum error_code);
private:
    QBasicTimer timer;
    QOpenGLDebugLogger *logger;
    QOpenGLShaderProgram program;
    QMatrix4x4 projection;

    /// Data to be animated.
    const std::shared_ptr<SimulationHistory> simulation_history;
    /// Index in the simulation_history that is currently being drawn
    unsigned history_index = 0;

    /// Local copy of the universe info (used to get the names, colors, etc.)
    physics::UniverseModel universe;

    parser::ProjectSettings settings;

    /// How fast should it animate (i.e. read the simulation history).
    int speed = 1;
    /// How much should the view matrix be scaled.
    float view_scale = 1.0;
    QVector2D mouse_press_position;
    QQuaternion rotation;

    // models
    Sphere sphere;
    std::vector<Orbit> orbits;
};

#endif  // __ANIMATION_H__
