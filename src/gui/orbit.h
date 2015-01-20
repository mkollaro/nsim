/*
 * Copyright 2015 Martina Kollarova
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
#ifndef __ORBIT_H__
#define __ORBIT_H__

#include <memory>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include "simulationhistory.h"


class QOpenGLShaderProgram;

/** Draw the trajectory of a body in the simulation history.
 *
 * Since it has a limited capacity, old values will be overwritten with new
 * ones when necessary (it implements a circular buffer), creating the effect
 * of a tail going behind the planet.
 *
 * It uses an OpenGL buffer and requires that some basic vertex and fragment
 * shader is available (ideally without any lighting).
 */
class Orbit : protected QOpenGLFunctions
{
public:
    /// Use the body with body_index in the history to draw its orbit.
    Orbit(const std::shared_ptr<SimulationHistory> history,
          const unsigned body_index);

    /** Call this in initializeGL() or the equivalent - it assumes that the
     * OpenGL context is bound, so it has to be separate from the constructor.
     */
    void initialize();

    /** Write positions from the history into the OpenGL buffer until the
     * `until_history_index` is reached.
     * @param history_index: ranges from 0 to SimulationHistory::historySize(),
     *      a higher value will be ignored
     */
    void updateData(unsigned until_history_index);

    /** Draw a line strip starting from the oldest values stored to the newest,
     * limited by vertex_count.
     *
     * @param shader_variable: name of the input variable to the vertex shader
     *      with positions of the vertices
     * @todo use until_history_index too, this is meaningless in a ring buffer
     */
    void draw(QOpenGLShaderProgram *program, unsigned vertex_count,
              const char* shader_variable = "vertex");

private:
    /// How many floating point numbers can the opengl buffer store. This will
    /// be allocated even if not in use.
    const unsigned MAX_BUFFER_LENGTH = 6000;

    QOpenGLBuffer vertex_buffer;
    std::shared_ptr<SimulationHistory> simulation_history;
    /// Index of the body in the SimulationHistory which will be shown.
    unsigned body_index;

    /// Index into the ring buffer (3x per vertex)
    unsigned start = 0;
    /// How many floats are already stored (3x per vertex)
    unsigned length = 0;

    /** Which item in the history was saved into the buffer last. Ranges from 0
     * to SimulationHistory::historySize.
     */
    unsigned history_index = 0;

    bool full() {
        return length == MAX_BUFFER_LENGTH;
    }

    bool empty() {
        return length == 0;
    }

    unsigned available() {
        return MAX_BUFFER_LENGTH - length;
    }
};

#endif  // __ORBIT_H__
