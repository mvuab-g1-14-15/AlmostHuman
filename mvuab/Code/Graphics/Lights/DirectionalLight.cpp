#include "DirectionalLight.h"
#include "Math\Vector3.h"
#include "GraphicsManager.h"

#include "XML\XMLTreeNode.h"

#define D3DFVF_CUSTOMVERTEXLIGHT (D3DFVF_XYZ|D3DFVF_DIFFUSE)
typedef struct CUSTOMVERTEXLIGHT
{
  float x, y, z;
  DWORD color;

  static unsigned int getFlags()
  {
    return(D3DFVF_CUSTOMVERTEXLIGHT);
  }
} CUSTOMVERTEXLIGHT;

CDirectionalLight::CDirectionalLight(CXMLTreeNode node) : CLight()
{
  m_Direction = node.GetVect3fProperty("dir",Math::Vect3f(0,0,0));
  m_Name = node.GetPszProperty("name","");
  m_Color = Math::colWHITE;
  m_Position = node.GetVect3fProperty("pos",Math::Vect3f(0,0,0));
  m_Intensity = node.GetFloatProperty("intensity", 0);
  m_fYaw = node.GetFloatProperty("yaw", 0);
  m_fPitch = node.GetFloatProperty("pitch", 0);
  m_fRoll = node.GetFloatProperty("roll", 0);
  m_Scale = node.GetVect3fProperty("scale",Math::Vect3f(0,0,0));
  m_StartRangeAttenuation = node.GetFloatProperty("att_start_range", 0);
  m_EndRangeAttenuation = node.GetFloatProperty("att_end_range", 0);
  SetType(CLight::DIRECTIONAL);
}

void CDirectionalLight::SetDirection(const Math::Vect3f &Direction)
{
  m_Direction = Direction;
}

Math::Vect3f CDirectionalLight::GetDirection() const
{
  return m_Direction;
}

void CDirectionalLight::Render()
{
  Math::Vect3f l_Direction = m_Direction.GetNormalized();
  LPDIRECT3DDEVICE9 l_Device = GraphicsInstance->GetDevice();
  CUSTOMVERTEXLIGHT v[2] =
  {
    {
      m_Position.x,
      m_Position.y,
      m_Position.z,
      0xff000000
    },
    {
      m_Position.x + l_Direction.x,
      m_Position.y + l_Direction.y,
      m_Position.z + l_Direction.z,
      0xff000000
    },
  };

  l_Device->DrawPrimitiveUP( D3DPT_LINELIST,1, v,sizeof(CUSTOMVERTEXLIGHT));

  D3DXMATRIX matrix;
  D3DXMATRIX translation;

  D3DXMatrixTranslation( &translation, m_Position.x ,m_Position.y ,m_Position.z );
  l_Device->SetTransform( D3DTS_WORLD, &translation );
  GraphicsInstance->DrawCube(0.5f, Math::colRED);

  D3DXMatrixTranslation( &matrix, 0, 0, 0 );
  l_Device->SetTransform( D3DTS_WORLD, &matrix );
}
