
#pragma once

#include "vector3.hh"
#include "matrix4.hh"
#include "matrix3.hh"

/* stop with the circular dependencies between vectors and matrices */

void vec3_mat3_multiply( Vector3&, const Matrix3& );
//void vec4_mat4_multiply( Vector4&, const Matrix4& );
