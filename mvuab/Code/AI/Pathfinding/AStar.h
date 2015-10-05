#ifndef _A_STAR_H
#define _A_STAR_H

#include "Math\Vector3.h"

#include <vector>

class CGraph;

class CAStar
{
public:
  CAStar(std::string aRoomName);
  ~CAStar();

  void Init();
  void Render();

  CGraph* m_Graph;
  std::string mRoomName;

  CGraph* GetGraph() const
  {
      return m_Graph;
  }

  std::vector<Math::Vect3f> GetPath( Math::Vect3f posicion_inicial, Math::Vect3f posicion_final );
};

#endif //_A_STAR_H