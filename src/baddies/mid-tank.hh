
#pragma

#include "../math/vector3.hh"
#include "../mesh-instance.hh"

class MidTank {
private:

  Vector3 m_position;
  float m_heading;
  bool m_active;
  bool m_has_moved;

  MeshInstance m_mesh_instance;
  

public:

  MidTank();

  void set_pos( float, float );
  void think_and_move();
  bool is_active();

  MeshInstance & mesh_instance();
};

