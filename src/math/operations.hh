
#pragma once

#include "vector2.hh"
#include "vector3.hh"

#include "matrix3.hh"
#include "matrix4.hh"

/* stop with the circular dependencies between vectors and matrices */

void vec3_mat3_multiply( Vector3&, const Matrix3& );
void vec3_mat4_multiply( Vector3&, const Matrix4& );

void vec2_mat3_multiply( Vector2&, const Vector2&, const Matrix3& );

