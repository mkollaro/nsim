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
#include "gui/animation.h"

#include <QMouseEvent>
#include <QDebug>
#include "exceptions.h"

const int TIMER_TIMEOUT = 12;


Animation::Animation(QWidget *parent,
                     std::shared_ptr<SimulationHistory> const history)
    : QOpenGLWidget(parent), simulation_history(history)
{
    logger = new QOpenGLDebugLogger(this);
}

Animation::~Animation()
{
    makeCurrent();
    // Make sure the context is current when deleting the texture
    // and the buffers.
    logger->stopLogging();
    doneCurrent();
}

void
Animation::loadUniverse(physics::UniverseModel u, parser::ProjectSettings p)
{
    universe = u;
    settings = p;
    history_index = 0;
    view_scale = 1.0/computeUniverseRadius();
    orbits.clear();
    for (unsigned i = 0; i < universe.size(); i++) {
        orbits.push_back(Orbit(simulation_history, i));
        orbits[i].initialize();
    }
    update();
}

////////////////////////////////////////////////////////////////////////////////
void
Animation::reset()
{
    history_index = 0;
    rotation = QQuaternion();
    speed = 1;
    update();
}

void
Animation::startOrStop(bool start)
{
    if(!hasData()) return;

    if (start) {
        emit stateChanged(PLAYING);
        timer.start(TIMER_TIMEOUT, this);
    } else {
        emit stateChanged(PAUSED);
        timer.stop();
    }
}

void
Animation::increaseSpeed()
{
    speed++;
    if(speed > 10000) speed = 10000;
    update();
}

void
Animation::decreaseSpeed()
{
    speed--;
    if(speed < 1) speed = 1;
    update();
}

void
Animation::onMessageLogged(QOpenGLDebugMessage message)
{
    qDebug() << message;
}

////////////////////////////////////////////////////////////////////////////////
void Animation::mousePressEvent(QMouseEvent *e)
{
    mouse_press_position = QVector2D(e->localPos());
}

void Animation::mouseMoveEvent(QMouseEvent *e)
{
    if (!hasData()) return;

    // mouse release position - mouse press position
    QVector2D diff = QVector2D(e->localPos()) - mouse_press_position;
    // rotation axis is perpendicular to the mouse position difference vector
    QVector3D rotationAxis = QVector3D(diff.y(), diff.x(), 0.0).normalized();
    // calculate new rotation axis as weighted sum
    rotation *= QQuaternion::fromAxisAndAngle(rotationAxis, diff.length()/100);
    update();
}

void Animation::timerEvent(QTimerEvent *)
{
    if (!hasData()) return;
    Q_ASSERT(speed >= 1); /// @todo slower?
    history_index += speed;
    if (history_index >= simulation_history->historySize()) {
        history_index = simulation_history->historySize() - 1;
        // startBuffering();
    }
    for (auto& orbit: orbits)
        orbit.updateData(history_index);
    update();
}

void Animation::initializeLogging()
{
    connect(logger, SIGNAL(messageLogged(QOpenGLDebugMessage)),
            this, SLOT(onMessageLogged(QOpenGLDebugMessage)),
            Qt::DirectConnection );
    if (logger->initialize()) {
        logger->startLogging(QOpenGLDebugLogger::SynchronousLogging);
        logger->enableMessages();
        qDebug("OpenGL debug logging initialized");
    } else {
        qDebug("Failed to initialize OpenGL debug logging"
               " - do you have the KHR_debug extension?");
    }
}

void Animation::initializeShaders()
{
    try {
        if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                             ":/shaders/vshader.glsl"))
            throw Exception("Could not compile vertex shader");
        if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                             ":/shaders/fshader.glsl"))
            throw Exception("Could not compile fragment shader");
        if (!program.link())
            throw Exception("Could not link shader pipeline");
        if (!program.bind())
            throw Exception("Could not bind shader pipeline");
    } catch(Exception&) {
        qDebug() << program.log();
        throw;
    }
}

void Animation::initializeGL()
{
    initializeLogging();
    initializeOpenGLFunctions();
    initializeShaders();
    sphere.initialize(1.0);

    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    printGlErrors();
}

void Animation::resizeGL(int w, int h)
{
    qreal aspect = qreal(w) / qreal(h ? h : 1);
    const qreal zNear = 1.0, zFar = 1024.0, fov = 45.0;
    projection.setToIdentity();
    projection.perspective(fov, aspect, zNear, zFar);
}

void Animation::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (!hasData()) return;

    QMatrix4x4 view;
    view.translate(0.0, 0.0, -3.0);
    view.rotate(rotation);
    view.scale(view_scale);

    Q_ASSERT(universe.size() == simulation_history->universeSize());
    program.setUniformValue("color", settings.bodyColor);
    program.setUniformValue("use_lights", true);
    for(unsigned i = 0; i < universe.size(); i++) {
        const auto& body = universe[i];
        QMatrix4x4 model;
        model.translate(simulation_history->bodyPosition(i, history_index));
        model.scale(body.radius
                    * body.visibleSizeMultiplier
                    * settings.visibleSizeMultiplier);
        program.setUniformValue("mvp_matrix", projection * view * model);
        sphere.draw(&program);
    }
    program.setUniformValue("color", settings.orbitColor);
    program.setUniformValue("use_lights", false);
    for(unsigned i = 0; i < universe.size(); i++) {
        QMatrix4x4 model;
        program.setUniformValue("mvp_matrix", projection * view * model);
        orbits[i].draw(&program, history_index);
    }
    printGlErrors();
}

////////////////////////////////////////////////////////////////////////////////
float
Animation::computeUniverseRadius()
{
    float max = 0;
    for (const auto& body: universe) {
        float distance = abs(body.position);
        if(distance > max) max = distance;
    }
    Q_ASSERT(max > 0);
    return max;
}


void Animation::printGlErrors()
{
    GLenum error = GL_NO_ERROR;
    do {
        error = glGetError();
        if (error != GL_NO_ERROR) {
            QString s = glErrorToString(error);
            qDebug() << "OpenGL Error:" << s;
        }
    } while (error != GL_NO_ERROR);
}

QString Animation::glErrorToString(GLenum error_code)
{
    switch(error_code) {
        case GL_NO_ERROR: return "No error";
        case GL_INVALID_ENUM: return "Invalid enumerant";
        case GL_INVALID_VALUE: return "Invalid value";
        case GL_INVALID_OPERATION: return "Invalid operation";
        case GL_STACK_OVERFLOW: return "Stack overflow";
        case GL_STACK_UNDERFLOW: return "Stack underflow";
        case GL_OUT_OF_MEMORY: return "Out of memory";
        default: return "Unknown error";
    }
}
