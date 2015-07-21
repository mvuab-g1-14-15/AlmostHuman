#include "Texture.h"
#include "Utils\Defines.h"
#include "GraphicsManager.h"

#include "Logger\Logger.h"
#include "EngineManagers.h"

static const D3DPOOL    sTexturePools[CTexture::eTexturePoolCount]    = { D3DPOOL_DEFAULT, D3DPOOL_SYSTEMMEM };
static const DWORD      sTextureUsage[CTexture::eTextureUsageCount]   = { D3DUSAGE_DYNAMIC, D3DUSAGE_RENDERTARGET  };
static const D3DFORMAT  sTextureFormat[CTexture::eTextureFormatCount] = { D3DFMT_A8R8G8B8, D3DFMT_R8G8B8, D3DFMT_X8R8G8B8, D3DFMT_R32F };

#define D3D_TEXTURE_ERROR_CASE( param ) case param: LOG_ERROR_APPLICATION( "%s creating the texture %s", ""#param"", m_FileName.c_str() ); break;

CTexture::CTexture()
    : m_Texture( 0 )
    , m_FileName( "" )
    , m_Width( 0 )
    , m_Height( 0 )
    , mMips( 0 )
    , m_OldRenderTarget( 0 )
    , m_DepthStencilRenderTargetTexture( 0 )
    , m_OldDepthStencilRenderTarget( 0 )
    , m_RenderTargetTexture( 0 )
    , m_CreateDepthStencilSurface(false)
    , mPoolType( CTexture::eDefaultPool )
    , mUsage( CTexture::eUsageDynamic )
    , mFormat( CTexture::eRGB8 )
{

}
CTexture::~CTexture()
{
    Unload();
}

bool CTexture::LoadFile()
{
   bool lOk = true;
   HRESULT lHR = D3DXCreateTextureFromFile( GraphicsInstance->GetDevice(), m_FileName.c_str(), &m_Texture );

   if( lHR != S_OK )
   {
      lOk = false;
      switch ( lHR )
      {
         D3D_TEXTURE_ERROR_CASE( D3DERR_NOTAVAILABLE )
         D3D_TEXTURE_ERROR_CASE( D3DERR_INVALIDCALL )
         D3D_TEXTURE_ERROR_CASE( D3DERR_NOTFOUND )
         D3D_TEXTURE_ERROR_CASE( D3DERR_DRIVERINVALIDCALL )
         D3D_TEXTURE_ERROR_CASE( E_OUTOFMEMORY )
         D3D_TEXTURE_ERROR_CASE( D3DERR_OUTOFVIDEOMEMORY )
         default: LOG_ERROR_APPLICATION("The file %s does not exist", m_FileName.c_str());
      }
   }

   if( lOk )
   {
      D3DXIMAGE_INFO lTextureInfo;
      HRESULT lTextureInfoHR= D3DXGetImageInfoFromFile(m_FileName.c_str(),&lTextureInfo);

      if( lTextureInfoHR == S_OK )
      {
         m_Width = lTextureInfo.Width;
         m_Height = lTextureInfo.Height;
      }
   }

    return lOk;
}

void CTexture::Unload()
{
    CHECKED_RELEASE( m_Texture );
}

const std::string& CTexture::GetFileName() const
{
    return m_FileName;
}

bool CTexture::Load( const std::string& FileName )
{
    SetName( FileName );
    m_FileName = FileName;
    return LoadFile();
}

bool CTexture::Reload()
{
    Unload();
    return LoadFile();
}

bool CTexture::Create( const std::string& Name, size_t Width, size_t Height,
                       size_t MipMaps, TUsageType UsageType, TPoolType PoolType,
                       TFormatType FormatType, bool CreateDepthStencilBuffer )
{
    ASSERT( Width != 0 && Height != 0, "The size of the texture is 0");

    SetName( Name );
    m_FileName = "dynamic_texture";

    D3DPOOL l_Pool = sTexturePools[ PoolType ];
    DWORD l_UsageType = sTextureUsage[UsageType];
    D3DFORMAT l_Format = sTextureFormat[FormatType];
    m_CreateDepthStencilSurface = (UsageType == eUsageRenderTarget) ? CreateDepthStencilBuffer : false;

    // Obtain the device from the graphics manager
    const LPDIRECT3DDEVICE9 l_Device = GraphicsInstance->GetDevice();
    HRESULT hr = l_Device->CreateTexture( Width, Height, MipMaps, l_UsageType, l_Format, l_Pool,
                                          &m_Texture, NULL );
    ASSERT( hr == D3D_OK && m_Texture, "Error creating the texture  %s", Name.c_str() );

    if ( m_CreateDepthStencilSurface )
    {
        l_Device->CreateDepthStencilSurface( Width, Height, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE,
                                             &m_DepthStencilRenderTargetTexture, NULL );
        ASSERT( m_DepthStencilRenderTargetTexture, "Error creating the depth stencil surface" );
    }

    m_Width  = Width;
    m_Height = Height;
    mMips    = MipMaps;
    mUsage   = UsageType;
    mPoolType = PoolType;
    mFormat = FormatType;
    return ( hr == D3D_OK );
}


void CTexture::Activate( size_t StageId )
{
    GraphicsInstance->GetDevice()->SetTexture( StageId, m_Texture );
}

void CTexture::Deactivate( size_t Stage )
{
    GraphicsInstance->GetDevice()->SetTexture( ( DWORD )Stage, NULL );
}

bool CTexture::SetAsRenderTarget( size_t IdStage )
{
    LPDIRECT3DDEVICE9 l_Device = GraphicsInstance->GetDevice();
    l_Device->GetRenderTarget( ( DWORD )IdStage, &m_OldRenderTarget );

    if ( FAILED( m_Texture->GetSurfaceLevel( 0, &m_RenderTargetTexture ) ) )
    {
        return false;
    }

    l_Device->SetRenderTarget( ( DWORD )IdStage, m_RenderTargetTexture );
    CHECKED_RELEASE( m_RenderTargetTexture );

    if(m_CreateDepthStencilSurface)
    {
        if ( FAILED( l_Device->GetDepthStencilSurface( &m_OldDepthStencilRenderTarget ) ) )
        {
            return false;
        }

        l_Device->SetDepthStencilSurface( m_DepthStencilRenderTargetTexture );
    }
    return true;
}

void CTexture::UnsetAsRenderTarget( size_t IdStage, bool UnsetDepthStencilBuffer )
{
    LPDIRECT3DDEVICE9 l_Device = GraphicsInstance->GetDevice();
    if(m_CreateDepthStencilSurface)
    {
        if(UnsetDepthStencilBuffer)
        {
            l_Device->SetDepthStencilSurface( m_OldDepthStencilRenderTarget );
        }

        CHECKED_RELEASE( m_OldDepthStencilRenderTarget );
    }

    l_Device->SetRenderTarget( IdStage, m_OldRenderTarget );
    CHECKED_RELEASE( m_OldRenderTarget );
}

void CTexture::CaptureFrameBuffer( size_t IdStage )
{
    LPDIRECT3DDEVICE9 l_Device = GraphicsInstance->GetDevice();
    LPDIRECT3DSURFACE9 l_RenderTarget, l_Surface;
    m_Texture->GetSurfaceLevel( 0, &l_Surface );

    if ( D3D_OK == l_Device->GetRenderTarget( IdStage, &l_RenderTarget ) )
    {
        l_Device->StretchRect( l_RenderTarget, NULL, l_Surface, NULL, D3DTEXF_NONE );
        l_RenderTarget->Release();
    }
    else
    {
        LOG_ERROR_APPLICATION(
            "Texture::CaptureFrameBuffer: Error capturing the frame buffer" );
    }
}

CTexture::TFormatType CTexture::GetFormatTypeFromString( const std::string& FormatType )
{
    if ( FormatType == "R32F" )
    {
        return eR32F;
    }
    else if ( FormatType == "A8R8G8B8" )
    {
        return eRGBA8;
    }
    else if ( FormatType == "R8G8B8" )
    {
        return eRGB8;
    }
    else if ( FormatType == "X8R8G8B8" )
    {
        return eRGBX8;
    }
    else
        LOG_ERROR_APPLICATION( "Format Type '%s' not recognized",
                               FormatType.c_str() );

    return eRGBA8;
}

bool CTexture::Save( const std::string& FileName )
{
    bool lOk = true;
    ASSERT( m_Texture, "Null d3dx texture to save" );
    if( m_Texture )
    {
        HRESULT hr = D3DXSaveTextureToFile( ( "./Data/textures/debug/" + FileName + ".png" ).c_str(), D3DXIFF_PNG, m_Texture,
                                            0 );

        if( hr != D3D_OK )
        {
            lOk = false;
            LOG_ERROR_APPLICATION("Error while saving the texture");
        }
    }

    return lOk;
}

bool CTexture::Clone( CTexture** aTexture )
{
    if( !aTexture )
    {
        *aTexture = new CTexture();
    }

    return (*aTexture)->Create( GetName(), m_Width, m_Height, mMips, mUsage, mPoolType, mFormat,
                                m_CreateDepthStencilSurface );
}