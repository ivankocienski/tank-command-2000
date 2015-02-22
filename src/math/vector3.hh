
#ifndef _IK_VECTOR3_INC_
#define _IK_VECTOR3_INC_

class Vector3 {
private:

public:

  float x;
  float y;
  float z;

  Vector3( float, float, float );
  Vector3();

  void set( float, float, float );
  void set( const Vector3& );

  void set_as_angle(float);

  void  normalize();
  float magnitude() const;
  float dot( const Vector3 & ) const;
  void  invert();

  Vector3 cross( const Vector3 & ) const;
  
  void operator*=( float );
  void operator+=( float );
  void operator*=( const Vector3& );
  void operator+=( const Vector3& );

  Vector3 operator-( ) const;
  Vector3 operator-( const Vector3 & ) const;
  Vector3 operator+( const Vector3 & ) const;
  Vector3 operator*( float ) const;

  Vector3 perpendicular() const;

  //Vector4 to_vector4() const;

  void dump() const;
};

#endif /* _IK_VECTOR3_INC_ */

