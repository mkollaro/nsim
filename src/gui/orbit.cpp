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
#include "gui/orbit.h"

#include <algorithm>
#include <QOpenGLShaderProgram>

const unsigned VERTEX_SIZE = 3;


Orbit::Orbit(const std::shared_ptr<SimulationHistory> history,
             const unsigned body_index)
    : simulation_history(history), body_index(body_index)
{
    Q_ASSERT(history != nullptr);
    Q_ASSERT(body_index < history->universeSize());
    Q_ASSERT(MAX_BUFFER_LENGTH % VERTEX_SIZE == 0);
}

void
Orbit::initialize()
{
    initializeOpenGLFunctions();
    vertex_buffer.create();
    vertex_buffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    vertex_buffer.bind();
    vertex_buffer.allocate(MAX_BUFFER_LENGTH * sizeof(GLfloat));
    vertex_buffer.release();
}

void
Orbit::updateData(unsigned until_history_index)
{
    if (history_index > until_history_index) return;
    const auto data = simulation_history->bodyPositions(body_index);
    if (history_index * VERTEX_SIZE > data.size()) return;

    unsigned end = (start + length) % MAX_BUFFER_LENGTH;
    Q_ASSERT(end + VERTEX_SIZE <= MAX_BUFFER_LENGTH);

    vertex_buffer.bind();
    vertex_buffer.write(end * sizeof(GLfloat),
                        &data[history_index * VERTEX_SIZE],
                        VERTEX_SIZE * sizeof(GLfloat));
    vertex_buffer.release();

    if (full()) {
        start = (start + VERTEX_SIZE) % MAX_BUFFER_LENGTH;
        // repeat the ending vertex at the beginning, so that the line strip is
        // continuos (OpenGL doesn't know it's a ring)
        if (start == 0) history_index--;
    } else {
        length += VERTEX_SIZE;
    }
    history_index++;
    if (history_index <= until_history_index)
        updateData(until_history_index);
}

void
Orbit::draw(QOpenGLShaderProgram *program, unsigned vertex_count,
            const char* shader_variable)
{
    if (vertex_count == 0) return;
    vertex_buffer.bind();
    program->enableAttributeArray(shader_variable);
    program->setAttributeBuffer(shader_variable, GL_FLOAT, 0, VERTEX_SIZE);
    vertex_buffer.release();
    unsigned count = std::min((length - start)/VERTEX_SIZE, vertex_count);
    Q_ASSERT(count < length / VERTEX_SIZE);
    glDrawArrays(GL_LINE_STRIP, start / VERTEX_SIZE, count);

    if (full()) {
        // OpenGL doesn't know that the buffer is supposed to be a ring, so we
        // have to wrap it around ourselves
        count = vertex_count - count;
        unsigned end = (start + length) % MAX_BUFFER_LENGTH;
        count = std::min((unsigned)end / VERTEX_SIZE, count);
        glDrawArrays(GL_LINE_STRIP, 0, count);
        // the connection point between the last buffer vertex and the first
        // one will be drawn because we have saved the last buffer vertex at
        // the beginning too (at the cost of some data duplication...)
    }
}
