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
#ifndef __SPHERE_H__
#define __SPHERE_H__

#include <QOpenGLFunctions>
#include <QOpenGLBuffer>


class QOpenGLShaderProgram;

/** Draw a sphere in OpenGL.
 *
 * Parts of the code originate from http://stackoverflow.com/a/5989676/770335
 */
class Sphere : protected QOpenGLFunctions
{
public:
    Sphere();

    /** Call this in initializeGL() or the equivalent - it assumes that the
     * OpenGL context is bound, so it has to be separate from the constructor.
     */
    void initialize(float radius,
                    unsigned int rings = 20, unsigned int sectors = 20);

    /** Draw the sphere at [0, 0, 0]. To move it around, pass some model-view
     * matrix to the vertex shader.
     * @param shader_variable_vertex: name of the input variable to the vertex
     *      shader with positions of the vertices
     * @param shader_variable_normal: name of the input variable to the vertex
     *      shader with normals of the vertices
     */
    void draw(QOpenGLShaderProgram *program,
              const char* shader_variable_vertex = "vertex",
              const char* shader_variable_normal = "normal");

private:
    QOpenGLBuffer vertex_buffer;
    QOpenGLBuffer index_buffer;
    QOpenGLBuffer normal_buffer;
    /// number of indices in Sphere::index_buffer
    int index_size = 0;

    void generate(float radius, unsigned int rings, unsigned int sectors);
};

#endif  // __SPHERE_H__
