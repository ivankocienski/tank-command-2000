
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

bool BoundingBox2D::point_inside( float tx, float ty ) {

  if( tx < m_min.x || tx > m_max.x ) return false;
  if( ty < m_min.y || ty > m_max.y ) return false;
  
  return true;
}

bool BoundingBox2D::does_intersect( const BoundingBox2D& bt ) {

  if( bt.max().x < m_min.x ) return false;
  if( bt.max().y < m_min.y ) return false;
  
  if( bt.min().x > m_max.x ) return false;
  if( bt.min().y > m_max.y ) return false;

  return true;
}

float BoundingBox2D::distance_to( const BoundingBox2D& bt ) {

  return 0; 
}


const Vector2 & BoundingBox2D::min() const {
  return m_min;
}

const Vector2 & BoundingBox2D::max() const {
  return m_max;
}
