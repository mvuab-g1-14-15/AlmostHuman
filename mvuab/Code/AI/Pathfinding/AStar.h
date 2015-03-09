#ifndef _A_STAR_H
#define _A_STAR_H

#include "Math\Vector3.h"

#include <vector>

class CGraph;

class CAStar
{
public:
  //std::vector<Node*> nodos;
  CGraph* m_Graph;

  CAStar();
  ~CAStar();

  void Init();
  void Render();

  std::vector<Math::Vect3f> GetPath( Math::Vect3f posicion_inicial, Math::Vect3f posicion_final );
};

#endif //_A_STAR_H