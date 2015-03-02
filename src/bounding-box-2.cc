
#include "bounding-box-2.hh"

BoundingBox2D::BoundingBox2D() {
  reset();
}

void BoundingBox2D::reset() {
  m_min.set(  10000,  10000 );
  m_max.set( -10000, -10000 );
}

void BoundingBox2D::update( float tx, float ty ) { 

  if( tx < m_min.x ) m_min.x = tx;
  if( ty < m_min.y ) m_min.y = ty;

  if( tx > m_max.x ) m_max.x = tx;
  if( ty > m_max.y ) m_max.y = ty;
}

void BoundingBox2D::update( const Vector3 & o ) {
  update( o.x, o.z );
}

void BoundingBox2D::finish() {
  m_center = (m_min + m_max) * 0.5;

  m_radius = (m_center - m_min).magnitude();
}

bool BoundingBox2D::point_inside( float tx, float ty ) const {

  if( tx < m_min.x || tx > m_max.x ) return false;
  if( ty < m_min.y || ty > m_max.y ) return false;
  
  return true;
}

bool BoundingBox2D::point_inside( const Vector2 &o ) const {

  if( o.x < m_min.x || o.x > m_max.x ) return false;
  if( o.y < m_min.y || o.y > m_max.y ) return false;
  
  return true;
}

bool BoundingBox2D::does_intersect( const BoundingBox2D& bt ) const {

  if( bt.max().x < m_min.x ) return false;
  if( bt.max().y < m_min.y ) return false;
  
  if( bt.min().x > m_max.x ) return false;
  if( bt.min().y > m_max.y ) return false;

  return true;
}

float BoundingBox2D::distance_to( const BoundingBox2D& bt ) const {

  // this could be smarter
  return (m_center - bt.center()).magnitude();
}

const Vector2 & BoundingBox2D::min() const {
  return m_min;
}

const Vector2 & BoundingBox2D::max() const {
  return m_max;
}

const Vector2 & BoundingBox2D::center() const {
  return m_center;
}

float BoundingBox2D::radius() const {
  return m_radius;
}

int BoundingBox2D::classify_side( const Plane &pln ) const {

  // TODO: this needs to operate better on 2D.
  int i;

  Vector3 v;

  // left top back
  v.set( m_min.x, 0, m_min.y );
  i += pln.is_facing(v);

  // right top back
  v.set( m_max.x, 0, m_min.y );
  i += pln.is_facing(v);

  // left top front
  v.set( m_min.x, 0, m_max.y );
  i += pln.is_facing(v);

  // right top front
  v.set( m_max.x, 0, m_max.y );
  i += pln.is_facing(v);

  if( i == 4 ) return 1;
  if( i == 0 ) return -1;

  return 0;
}
