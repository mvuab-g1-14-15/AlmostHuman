#pragma once
#ifndef INC_TEXTURE_H_
#define INC_TEXTURE_H_

#include <d3dx9.h>
#include <string>
#include "Utils\Name.h"

class CTexture: public CName
{
public:
  enum TPoolType
  {
    DEFAULT = 0,
    SYSTEMMEM
  };
  enum TUsageType
  {
    DYNAMIC = 0,
    RENDERTARGET
  };
  enum TTextureType
  {
    TGA = 0,
    JPG,
    BMP
  };
  enum TFormatType
  {
    A8R8G8B8 = 0,
    R8G8B8,
    X8R8G8B8,
    R32F
  };
protected:
  //Members
  LPDIRECT3DTEXTURE9 m_Texture;
  std::string m_FileName;
  //TODO ALEX No estoy seguro de si va en protected o private (pag 26 Advanced Shaders) y de como se trabaja con esto...
  size_t m_Width;
  size_t m_Height;
  IDirect3DSurface9* m_OldRenderTarget;
  IDirect3DSurface9* m_DepthStencilRenderTargetTexture;
  IDirect3DSurface9* m_OldDepthStencilRenderTarget;
  IDirect3DSurface9* m_RenderTargetTexture;
  //FIN TODO
  //Methods
  virtual bool LoadFile();
  void Unload();

public:
  CTexture();
  virtual ~CTexture();

  const std::string& GetFileName() const;
  bool Load( const std::string& FileName );
  bool Save( const std::string& FileName );

  bool Reload();
  virtual void Activate( size_t StageId );
  bool Create( const std::string& Name, size_t Width, size_t Height,
               size_t MipMaps, TUsageType UsageType, TPoolType PoolType,
               TFormatType FormatType );

  void Deactivate( size_t Stage );
  bool SetAsRenderTarget( size_t IdStage = 0 );
  void UnsetAsRenderTarget( size_t IdStage = 0 );
  void CaptureFrameBuffer( size_t IdStage );
  CTexture::TFormatType GetFormatTypeFromString( const std::string& FormatType );
};

#endif //INC_TEXTURE_H_