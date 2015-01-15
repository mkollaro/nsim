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
 * Defines Vector as in physics [x, y, z] and its operators.
 */
#ifndef __PHYSICS_VECTOR_H__
#define __PHYSICS_VECTOR_H__

#include <cmath>
#include <iostream>
#include <stdexcept>
#include "physics/precision.h"

namespace physics
{
/**
 * A vector as in physics, [x, y, z] where x, y, and z are floating-point, with
 * the precision of physics::DOUBLE.
 * Supports some of the standard vector operations, like the addition of
 * vectors or the dot product.
 *
 * @note This class is very similar to QVector3D from Qt. QVector3D wasn't used
 * because it only supports `float` precision, which is not enough to get
 * precise simulation results. It would also make it necessary to link the CLI
 * version of NSim against the QtGui library.
 */
class Vector
{
    physics::DOUBLE xx, yy, zz;
public:
    /// Construct vector [x, y, z].
    Vector(const physics::DOUBLE x = 0,
           const physics::DOUBLE y = 0,
           const physics::DOUBLE z = 0)
        :xx(x), yy(y), zz(z) {}
    /// Copy vector v.
    Vector(const Vector& v)
        :xx(v.x()), yy(v.y()), zz(v.z()) {}

    /// Set new values [x, y, z].
    void set(const physics::DOUBLE x,
             const physics::DOUBLE y,
             const physics::DOUBLE z) {
        xx = x;
        yy = y;
        zz = z;
    }
    /// Copy values from v.
    void set(const Vector& v) {
        xx = v.x();
        yy = v.y();
        zz = v.z();
    }

    /// @{

    /// Equivalent to myVector[0].
    physics::DOUBLE x() const {
        return xx;
    }
    /// Equivalent to myVector[1].
    physics::DOUBLE y() const {
        return yy;
    }
    /// Equivalent to myVector[2].
    physics::DOUBLE z() const {
        return zz;
    }

    /// Read-only access to data.
    physics::DOUBLE const& operator[](const int op) const {
        if(op == 0) return xx;
        else if(op == 1) return yy;
        else if(op == 2) return zz;
        else
            throw std::out_of_range("Index out of range in Vector");
    }

    /// Read-write access to data.
    physics::DOUBLE& operator[](const int op) {
        if(op == 0) return xx;
        else if(op == 1) return yy;
        else if(op == 2) return zz;
        else
            throw std::out_of_range("Index out of range in Vector");
    }
    /// @}

    /// Return a stream of the vector values separated by space, "x y z".
    /// @code
    ///     cout << myVector;
    /// @endcode
    friend std::ostream& operator<<(std::ostream& output, const Vector& v) {
        return output << v[0] << " " << v[1] << " " << v[2];
    }

    /// They are equal if the difference between their items is less
    /// than EPSILON.
    bool operator==(const Vector& v) {
        return physics::equal(xx, v.xx)
               && physics::equal(yy, v.yy)
               && physics::equal(zz, v.zz);
    }

    /// The exact opposite result of operator== .
    bool operator!=(const Vector& op) {
        return !(*this == op);
    }

    /// Copy the values, but do nothing if used on identical objects.
    Vector& operator=(const Vector& op) {
        if(this != &op) {
            xx = op.x();
            yy = op.y();
            zz = op.z();
        }
        return *this;
    }

    /*************************************************************************/
    /// Add scalar to vector.
    /// @return \f$[x + op, y + op, z + op]\f$
    Vector& operator+=(const physics::DOUBLE op) {
        xx += op;
        yy += op;
        zz += op;
        return *this;
    }

    /// Add two vectors together.
    /// @return \f$[x_1 + x_2, y_1 + y_2, z_1 + z_2]\f$
    Vector& operator+=(const Vector& op) {
        xx += op.x();
        yy += op.y();
        zz += op.z();
        return *this;
    }

    Vector operator+(const physics::DOUBLE op) const {
        return Vector(*this) += op;
    }

    friend Vector operator+(const physics::DOUBLE op, Vector v) {
        return v += op;
    }

    const Vector operator+(Vector v) const {
        return v += *this;
    }

    Vector& operator-=(const physics::DOUBLE op) {
        xx -= op;
        yy -= op;
        zz -= op;
        return *this;
    }

    Vector& operator-=(const Vector& v) {
        xx -= v.xx;
        yy -= v.yy;
        zz -= v.zz;
        return *this;
    }

    const Vector operator-(const physics::DOUBLE op) const {
        return Vector(*this) -= op;
    }

    const Vector operator-(const Vector& op) const {
        return Vector(*this) -= op;
    }

    /// Divide vector by a scalar value.
    /// @return \f$[x/op, y/op, z/op]\f$
    Vector& operator/=(const physics::DOUBLE op) {
        if(op == 0)
            throw std::overflow_error("Divide by zero exception");
        xx /= op;
        yy /= op;
        zz /= op;
        return *this;
    }

    const Vector operator/(const physics::DOUBLE op) const {
        return Vector(*this) /= op;
    }

    /// Multiply vector by a scalar value.
    /// @return \f$[x \cdot op, y \cdot op, z \cdot op]\f$
    Vector& operator*=(const physics::DOUBLE op) {
        xx *= op;
        yy *= op;
        zz *= op;
        return *this;
    }

    const Vector operator*(const physics::DOUBLE op) const {
        return Vector(*this) *= op;
    }

    friend const Vector operator*(const physics::DOUBLE op, Vector v) {
        return v *= op;
    }
};

/// Get the magnitude of the vector.
/// @return \f$\sqrt{x^2 + y^2 + z^2}\f$
inline physics::DOUBLE
abs(const Vector& op)
{
    return std::sqrt(op[0]*op[0] + op[1]*op[1] + op[2]*op[2]);
}

/// Return standard dot product of vectors.
inline physics::DOUBLE
dotproduct(const Vector& one, const Vector& two)
{
    return one[0]*two[0] + one[1]*two[1] + one[2]*two[2];
}
}  // namespace

#endif  // __PHYSICS_VECTOR_H__
