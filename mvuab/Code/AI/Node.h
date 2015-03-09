#ifndef _NODE_H
#define _NODE_H

#include "Math/Vector3.h"

#include <vector>

class Node
{
public:
  Node(Math::Vect3f pos) {posicion = pos;};
  ~Node() {};

  void Render();

  Math::Vect3f posicion;
  std::vector<Node*> vecinos;

  float h;
  float g;

  bool visitado;
};

#endif //_NODE_H