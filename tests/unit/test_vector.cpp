#include "catch.h"
#include <cmath>
#include "physics/vector.h"


TEST_CASE("Constructor creates zero vector", "[vector]")
{
    physics::Vector vector;
    REQUIRE(vector.x() == 0);
    REQUIRE(vector.y() == 0);
    REQUIRE(vector.z() == 0);
}

TEST_CASE("Vector value initialization", "[vector]")
{
    physics::Vector vector(1,-2,3);
    REQUIRE(vector.x() == 1.0);
    REQUIRE(vector.y() == -2.0);
    REQUIRE(vector.z() == 3.0);
}

TEST_CASE("Initialize vector with other vector", "[vector]")
{
    physics::Vector vector1(1,-2,3);
    physics::Vector vector2(vector1);
    REQUIRE(vector2.x() == 1.0);
    REQUIRE(vector2.y() == -2.0);
    REQUIRE(vector2.z() == 3.0);
}

TEST_CASE("Set values", "[vector]")
{
    physics::Vector vector;
    vector.set(4,5,6);
    REQUIRE(vector[0] == 4.0);
    REQUIRE(vector[1] == 5.0);
    REQUIRE(vector[2] == 6.0);
}

TEST_CASE("Set values by other vector", "[vector]")
{
    physics::Vector vector1(1,-2,3);
    physics::Vector vector2(4,5,6);
    vector1.set(vector2);
    REQUIRE(vector1.x() == vector2.x());
    REQUIRE(vector1.y() == vector2.y());
    REQUIRE(vector1.z() == vector2.z());
}

TEST_CASE("Compare vectors", "[vector]")
{
    physics::Vector vector1(1,-2,3);
    physics::Vector vector2(1.0,-2.0,3.0);
    REQUIRE(vector1 == vector2);
    vector2[0] = 5;
    REQUIRE(vector1 != vector2);
}

TEST_CASE("Vector addition", "[vector]")
{
    physics::Vector vector1(6,-2,10.0);
    physics::Vector vector2(1,2,3);
    auto result = vector1 + vector2;
    REQUIRE(result == physics::Vector(7, 0, 13));
    vector1 += vector2;
    REQUIRE(vector1 == physics::Vector(7, 0, 13));
}

TEST_CASE("Scalar addition", "[vector]")
{
    physics::Vector vector(6,-2,10.0);
    auto result = vector + 1;
    REQUIRE(result == physics::Vector(7, -1, 11));
    result = (physics::DOUBLE)2.5 + vector;
    REQUIRE(result == physics::Vector(8.5, 0.5, 12.5));
    vector += 3.0;
    REQUIRE(vector == physics::Vector(9, 1, 13));
}

TEST_CASE("Vector subtraction", "[vector]")
{
    physics::Vector vector1(-1, -5, 8.5);
    physics::Vector vector2(1,2,3);
    auto result = vector1 - vector2;
    REQUIRE(result == physics::Vector(-2, -7, 5.5));
    result = vector2 - vector1;
    REQUIRE(result == physics::Vector(2, 7, -5.5));
    vector1 -= vector2;
    REQUIRE(vector1 == physics::Vector(-2, -7, 5.5));
}

TEST_CASE("Scalar subtraction", "[vector]")
{
    physics::Vector vector(6,-2,10.0);
    auto result = vector - 1.0;
    REQUIRE(result == physics::Vector(5, -3, 9));
    vector -= 3.0;
    REQUIRE(vector == physics::Vector(3, -5, 7));
}

TEST_CASE("Scalar multiplication", "[vector]")
{
    physics::Vector vector(6,-2,10.0);
    auto result = vector * 0;
    REQUIRE(result == physics::Vector(0,0,0));
    result = vector * 1;
    REQUIRE(result == vector);
    result = vector * 2;
    REQUIRE(result == physics::Vector(12,-4,20));
    result = 2 * vector;
    REQUIRE(result == physics::Vector(12,-4,20));
    vector *= 3.0;
    REQUIRE(vector == physics::Vector(18, -6, 30));
}

TEST_CASE("Scalar division", "[vector]")
{
    physics::Vector vector(6,-2,9.0);
    auto result = vector / 1;
    REQUIRE(result == vector);
    result = vector / 2;
    REQUIRE(result == physics::Vector(3,-1,4.5));
    vector /= 3.0;
    REQUIRE(vector == physics::Vector(2, (physics::DOUBLE)-2.0/(physics::DOUBLE)3.0, 3));

    REQUIRE_THROWS(vector / 0);
}

TEST_CASE("Check abs (magnitude)", "[vector]")
{
    REQUIRE(physics::abs(physics::Vector()) == 0);
    REQUIRE(physics::abs(physics::Vector(-1,0,0)) == 1);
    REQUIRE(physics::abs(physics::Vector(0,1,0)) == 1);
    REQUIRE(physics::abs(physics::Vector(0,0,1)) == 1);
    REQUIRE(physics::equal(physics::abs(physics::Vector(-3,2,5)),
                           std::sqrt(38)));
}

TEST_CASE("Dot product", "[vector]")
{
    physics::Vector vector1(1,2,3);
    physics::Vector vector2(4,-5,6);
    REQUIRE(physics::equal(physics::dotproduct(vector1, vector2), 12));
}
