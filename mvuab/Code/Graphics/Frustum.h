//----------------------------------------------------------------------------------
// CCamera class
// Author: Enric Vergara
//
// Description:
// Clase 
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_FRUSTUM_H_
#define INC_FRUSTUM_H_

#include <d3dx9math.h>
#include "Math/Vector3.h"

class CFrustum 
{
public:
    
  void    Update                            ( const D3DXMATRIX &viewproj );
  bool    SphereVisible                ( const D3DXVECTOR3 &p, float32 radius ) const;
  bool    BoxVisible                    ( const D3DXVECTOR3 &max, const D3DXVECTOR3 &min ) const;
  bool  BoxVisibleByVertexs    ( const Vect3f* points) const;
   
private:

  float32 m_proj[16];
  float32 m_modl[16];
  float32 m_clip[16];
  float32 m_frustum[6][4];
};

#endif //INC_FRUSTUM_H_
