#include "CubedTexture.h"
#include "Utils\Defines.h"
#include "Logger\Logger.h"
#include "Core.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"

CCubedTexture::CCubedTexture()
  : CTexture()
{
}

CCubedTexture::~CCubedTexture()
{
}

bool CCubedTexture::LoadFile()
{
  HRESULT l_HR = D3DXCreateCubeTextureFromFile( GraphicsInstance->GetDevice(),
                 ( "./Data/textures/" + m_FileName ).c_str(), &m_CubeTexture );

  if ( l_HR != D3D_OK )
  {
    LOG_ERROR_APPLICATION( "CCubedTexture::LoadFile error al cargar %s\n",
                                           m_FileName );
    return false;
  }

  return true;
}

void CCubedTexture::Activate( size_t StageId )
{
  GraphicsInstance->GetDevice()->SetTexture( StageId, m_CubeTexture );
}