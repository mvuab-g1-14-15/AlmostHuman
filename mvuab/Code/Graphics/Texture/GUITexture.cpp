#include "GUITexture.h"
#include "Utils\Defines.h"
#include "Logger\Logger.h"
#include "EngineManagers.h"
#include "GraphicsManager.h"
#include <assert.h>

CGUITexture::CGUITexture()
{
}

CGUITexture::~CGUITexture()
{
}

bool CGUITexture::LoadFile()
{
  HRESULT l_HR = D3DXCreateTextureFromFile( GraphicsInstance->GetDevice(), m_FileName.c_str(), &m_Texture );

  if ( l_HR != D3D_OK )
  {
    LOG_ERROR_APPLICATION( "CGUITexture::LoadFile error al cargar %s\n",
                           m_FileName );
    return false;
  }

  return true;
}