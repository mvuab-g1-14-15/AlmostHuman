#include "Texture.h"
#include "Utils\Defines.h"
#include "GraphicsManager.h"

#include "Logger\Logger.h"
#include "EngineManagers.h"

/*

    IDirect3DSurface9* m_OldRenderTarget;
    IDirect3DSurface9* m_DepthStencilRenderTargetTexture;
    IDirect3DSurface9* m_OldDepthStencilRenderTarget;
    IDirect3DSurface9* m_RenderTargetTexture;
*/

CTexture::CTexture()
    : m_Texture( 0 )
    , m_FileName( "" )
    , m_Width( 0 )
    , m_Height( 0 )
    , m_OldRenderTarget( 0 )
    , m_DepthStencilRenderTargetTexture( 0 )
    , m_OldDepthStencilRenderTarget( 0 )
    , m_RenderTargetTexture( 0 )
	, m_CreateDepthStencilSurface(false)
{
}

CTexture::~CTexture()
{
    Unload();
}

bool CTexture::LoadFile()
{
    HRESULT l_Res = D3DXCreateTextureFromFile(
                        GraphicsInstance->GetDevice(),
                        m_FileName.c_str(), &m_Texture );
    return ( l_Res == S_OK );
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

void CTexture::Activate( size_t StageId )
{
    GraphicsInstance->GetDevice()->SetTexture( StageId,
            m_Texture );
}

bool CTexture::Create( const std::string& Name, size_t Width, size_t Height,
                       size_t MipMaps, TUsageType UsageType, TPoolType PoolType,
                       TFormatType FormatType, bool CreateDepthStencilBuffer )
{
    SetName( Name );
    D3DPOOL l_Pool = D3DPOOL_DEFAULT;
    DWORD l_UsageType = D3DUSAGE_DYNAMIC;
    D3DFORMAT l_Format = D3DFMT_A8R8G8B8;
    bool l_CreateDepthStencilSurface = false;

    switch ( UsageType )
    {
        case DYNAMIC:
            l_UsageType = D3DUSAGE_DYNAMIC;
            break;

        case RENDERTARGET:
            l_CreateDepthStencilSurface = CreateDepthStencilBuffer;
            l_UsageType = D3DUSAGE_RENDERTARGET;
            break;
    }

    switch ( PoolType )
    {
        case DEFAULT:
            l_Pool = D3DPOOL_DEFAULT;
            break;

        case SYSTEMMEM:
            l_Pool = D3DPOOL_SYSTEMMEM;
            break;
    }

    switch ( FormatType )
    {
        case A8R8G8B8:
            l_Format = D3DFMT_A8R8G8B8;
            break;

        case R8G8B8:
            l_Format = D3DFMT_R8G8B8;
            break;

        case X8R8G8B8:
            l_Format = D3DFMT_X8R8G8B8;
            break;

        case R32F:
            l_Format = D3DFMT_R32F;
            break;
    }

	m_CreateDepthStencilSurface=l_CreateDepthStencilSurface;

    // Obtain the device from the graphics manager
    const LPDIRECT3DDEVICE9 l_Device = GraphicsInstance->GetDevice();
    HRESULT hr = l_Device->CreateTexture( Width, Height, MipMaps, l_UsageType, l_Format, l_Pool,
                                          &m_Texture, NULL );
    assert( hr == D3D_OK );
    assert( m_Texture != NULL );

    if ( l_CreateDepthStencilSurface )
    {
        l_Device->CreateDepthStencilSurface( Width, Height, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE,
                                             &m_DepthStencilRenderTargetTexture, NULL );
        assert( m_DepthStencilRenderTargetTexture != NULL );
    }

    m_Width = Width;
    m_Height = Height;
    return ( hr != D3D_OK );
}

void CTexture::Deactivate( size_t Stage )
{
    GraphicsInstance->GetDevice()->SetTexture( ( DWORD )Stage,
            NULL );
}

bool CTexture::SetAsRenderTarget( size_t IdStage )
{
    LPDIRECT3DDEVICE9 l_Device = GraphicsInstance->GetDevice();
    l_Device->GetRenderTarget( ( DWORD )IdStage, &m_OldRenderTarget );

    if ( FAILED( m_Texture->GetSurfaceLevel( 0, &m_RenderTargetTexture ) ) )
    { return false; }

    l_Device->SetRenderTarget( ( DWORD )IdStage, m_RenderTargetTexture );
    CHECKED_RELEASE( m_RenderTargetTexture );

	if(m_CreateDepthStencilSurface)
	{
		if ( FAILED( l_Device->GetDepthStencilSurface( &m_OldDepthStencilRenderTarget ) ) )
		{ return false; }

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
			l_Device->SetDepthStencilSurface( m_OldDepthStencilRenderTarget );
    
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
    { return R32F; }
    else if ( FormatType == "A8R8G8B8" )
    { return A8R8G8B8; }
    else if ( FormatType == "R8G8B8" )
    { return R8G8B8; }
    else if ( FormatType == "X8R8G8B8" )
    { return X8R8G8B8; }
    else
        LOG_ERROR_APPLICATION( "Format Type '%s' not recognized",
                               FormatType.c_str() );

    return A8R8G8B8;
}

bool CTexture::Save( const std::string& FileName )
{
	bool lOk = true;
	ASSERT( m_Texture, "Null d3dx texture to save" );
    if( m_Texture )
	{
		HRESULT hr = D3DXSaveTextureToFile( ( "./Data/textures/debug/" + FileName + ".png" ).c_str(), D3DXIFF_PNG, m_Texture, 0 );

		if( hr != D3D_OK )
		{
			lOk = false;
			LOG_ERROR_APPLICATION("Error while saving the texture");
		}
	}
    
	return lOk;
}