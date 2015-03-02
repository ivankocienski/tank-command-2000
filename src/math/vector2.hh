
#ifndef _IK_VECTOR2_INC_
#define _IK_VECTOR2_INC_

#include "vector3.hh"

class Vector2 {
private:

public:

  float x;
  float y;

  Vector2( float, float );
  Vector2();

  void set( float, float );
  void set( const Vector2& );

  void set_as_angle(float);

  void  normalize();
  float magnitude() const;
  float dot( const Vector2 & ) const;
  void  invert();

  void operator*=( float );
  void operator+=( float );
  void operator*=( const Vector2& );
  void operator+=( const Vector2& );

  Vector2 operator-( ) const;
  Vector2 operator-( const Vector2 & ) const;
  Vector2 operator+( const Vector2 & ) const;
  Vector2 operator*( float ) const;

  Vector2 perpendicular() const;

  Vector3 to_vector3() const;
  Vector3 to_vector3(float) const;

  void dump() const;
};

#endif /* _IK_VECTOR2_INC_ */

