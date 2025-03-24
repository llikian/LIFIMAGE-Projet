/***************************************************************************************************
 * @file  transforms.hpp
 * @brief Declaration of functions for 3D transforms
 **************************************************************************************************/

#pragma once

#include "mat4.hpp"
#include "vec.h"

/**
 * @brief Calculates the scaling matrix that scales by the same factor in all 3 directions.
 * @param factor The scaling factor.
 * @return The scaling matrix.
 */
mat4 scale(float factor);

/**
 * @brief Calculates the scaling matrix that scales by a specific factor in each direction.
 * @param x The scaling factor in the x direction.
 * @param y The scaling factor in the y direction.
 * @param z The scaling factor in the z direction.
 * @return The scaling matrix.
 */
mat4 scale(float x, float y, float z);

/**
 * @brief Calculates the scaling matrix that scales by a specific factor in each direction.
 * @param factors The scaling factors.
 * @return The scaling matrix.
 */
mat4 scale(const vec3& factors);

/**
 * @brief Calculates the scaling matrix that only scales in the x direction.
 * @param factor The scaling factor.
 * @return The scaling matrix.
 */
mat4 scaleX(float factor);

/**
 * @brief Calculates the scaling matrix that only scales in the y direction.
 * @param factor The scaling factor.
 * @return The scaling matrix.
 */
mat4 scaleY(float factor);

/**
 * @brief Calculates the scaling matrix that only scales in the z direction.
 * @param factor The scaling factor.
 * @return The scaling matrix.
 */
mat4 scaleZ(float factor);

/**
 * @brief Calculates the translation matrix that displaces by a specific vector.
 * @param vector The translation vector.
 * @return The translation matrix.
 */
mat4 translate(const Vector& vector);

/**
 * @brief Calculates the translation matrix that displaces by a specific amount in each direction.
 * @param x The displacement in the x direction.
 * @param y The displacement in the y direction.
 * @param z The displacement in the z direction.
 * @return The translation matrix.
 */
mat4 translate(float x, float y, float z);

/**
 * @brief Calculates the translation matrix that only displaces in the x direction.
 * @param scalar The displacement amount.
 * @return The translation matrix.
 */
mat4 translateX(float scalar);

/**
 * @brief Calculates the translation matrix that only displaces in the y direction.
 * @param scalar The displacement amount.
 * @return The translation matrix.
 */
mat4 translateY(float scalar);

/**
 * @brief Calculates the translation matrix that only displaces in the z direction.
 * @param scalar The displacement amount.
 * @return The translation matrix.
 */
mat4 translateZ(float scalar);

/**
 * @brief Calculates the rotation matrix that rotates around an axis by a certain angle.
 * @param angle The rotation angle in degrees.
 * @param axis The rotation axis.
 * @return The rotation matrix.
 */
mat4 rotate(float angle, const Vector& axis);

/**
 * @brief Calculates the rotation matrix that rotates around the x axis by a certain angle.
 * @param angle The rotation angle in degrees.
 * @return The rotation matrix.
 */
mat4 rotateX(float angle);

/**
 * @brief Calculates the rotation matrix that rotates around the y axis by a certain angle.
 * @param angle The rotation angle in degrees.
 * @return The rotation matrix.
 */
mat4 rotateY(float angle);

/**
 * @brief Calculates the rotation matrix that rotates around the z axis by a certain angle.
 * @param angle The rotation angle in degrees.
 * @return The rotation matrix.
 */
mat4 rotateZ(float angle);
