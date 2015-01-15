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
#include "sphere.h"

#include <cmath>
#include <vector>
#include <QOpenGLShaderProgram>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Sphere::Sphere()
    : index_buffer(QOpenGLBuffer::IndexBuffer)
{
}

void
Sphere::initialize(float radius, unsigned int rings, unsigned int sectors)
{
    initializeOpenGLFunctions();
    generate(radius, rings, sectors);
}

void
Sphere::draw(QOpenGLShaderProgram *program, const char* shader_variable_vertex,
             const char* shader_variable_normal)
{
    vertex_buffer.bind();
    program->enableAttributeArray(shader_variable_vertex);
    program->setAttributeBuffer(shader_variable_vertex, GL_FLOAT, 0, 3);
    vertex_buffer.release();

    normal_buffer.bind();
    program->enableAttributeArray(shader_variable_normal);
    program->setAttributeBuffer(shader_variable_normal, GL_FLOAT, 0, 3);
    normal_buffer.release();

    index_buffer.bind();
    glDrawElements(GL_QUADS, index_size, GL_UNSIGNED_SHORT, 0);
    index_buffer.release();
}

void
Sphere::generate(float radius, unsigned int rings, unsigned int sectors)
{
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texcoords;
    std::vector<GLushort> indices;

    float const R = 1.0/(float)(rings - 1);
    float const S = 1.0/(float)(sectors - 1);

    vertices.resize(rings * sectors * 3);
    normals.resize(rings * sectors * 3);
    texcoords.resize(rings * sectors * 2);
    std::vector<GLfloat>::iterator v = vertices.begin();
    std::vector<GLfloat>::iterator n = normals.begin();
    std::vector<GLfloat>::iterator t = texcoords.begin();
    for(unsigned int r = 0; r < rings; r++) {
        for(unsigned int s = 0; s < sectors; s++) {
            float y = std::sin( -M_PI_2 + M_PI * r * R );
            float x = std::cos(2*M_PI * s * S) * std::sin(M_PI * r * R);
            float z = std::sin(2*M_PI * s * S) * std::sin(M_PI * r * R);

            *t++ = s*S;
            *t++ = r*R;

            *v++ = x * radius;
            *v++ = y * radius;
            *v++ = z * radius;

            *n++ = x;
            *n++ = y;
            *n++ = z;
        }
    }

    indices.resize(rings * sectors * 4);
    std::vector<GLushort>::iterator i = indices.begin();
    for(unsigned int r = 0; r < rings-1; r++) {
        for(unsigned int s = 0; s < sectors-1; s++) {
            *i++ = r * sectors + s;
            *i++ = r * sectors + (s+1);
            *i++ = (r+1) * sectors + (s+1);
            *i++ = (r+1) * sectors + s;
        }
    }

    vertex_buffer.create();
    vertex_buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vertex_buffer.bind();
    vertex_buffer.allocate(&(vertices[0]), vertices.size() * sizeof(GLfloat));
    vertex_buffer.release();

    index_buffer.create();
    index_buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    index_buffer.bind();
    index_buffer.allocate(&(indices[0]), indices.size() * sizeof(GLfloat));
    index_buffer.release();
    index_size = indices.size();

    normal_buffer.create();
    normal_buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    normal_buffer.bind();
    normal_buffer.allocate(&(normals[0]), normals.size() * sizeof(GLfloat));
    normal_buffer.release();
}
