#include "Fonts\FontManager.h"
#include "Language\LanguageManager.h"
#include "GraphicsManager.h"
#include "xml/XMLTreeNode.h"
#include "Logger\Logger.h"
#include "EngineConfig.h"

#include "EngineManagers.h"
#include "Utils/StringUtils.h"

CFontManager::CFontManager( CXMLTreeNode& atts )
    : CManager( atts )
{
}

void CFontManager::Init()
{
  m_pD3DDevice = GraphicsInstance->GetDevice();
  LoadTTFs( mConfigPath );
}
void CFontManager::Release()
{
  m_Fonts.Destroy();
  mTTFs.clear();
}

bool CFontManager::ReloadTTFs()
{
  return LoadTTFs( mConfigPath );
}

bool CFontManager::LoadTTFs( const std::string& pathFile )
{
  bool lLoadedOk = false;
  if( pathFile != "" )
  {
    //first of all we release the fonts
    Release();

    // Create a default font
    mTTFs["TimesNewRoman"] = AddFont( 17, true, false, "Times New Roman", "timesnewroman.ttf", true );

    CXMLTreeNode parser, m;
    if ( parser.LoadAndFindNode( pathFile.c_str(), "Fonts", m ) )
    {
        for ( uint32 i = 0, lCount = m.GetNumChildren(); i < lCount; ++i )
        {
            const CXMLTreeNode& lCurrentNode = m(i);
            const std::string& file = lCurrentNode.GetAttribute<std::string>( "file" , "no_file");
            if ( !m_Fonts.Exist( file ) )
            {
                if ( AddFontResource( file.c_str() ) == 1 )
                {
                    mTTFs[lCurrentNode.GetAttribute<std::string>( "id" , "no_id" )] = 
                      AddFont( 
                        lCurrentNode.GetAttribute<int32>( "size", 10 ),
                        lCurrentNode.GetAttribute<bool>( "bold", false ),
                        lCurrentNode.GetAttribute<bool>( "italica", false ),
                        lCurrentNode.GetAttribute<std::string>( "name" , "no_name"),
                        file,
                        lCurrentNode.GetAttribute<bool>( "default", false )
                      );
                }
                else
                {
                    LOG_ERROR_APPLICATION( "LoadFonts:: no se ha podido añadir el ttf file: %s", file.c_str() );
                }
            }
        }
    }
    lLoadedOk = true;
  }

  return lLoadedOk;
}

int32 CFontManager::GetTTF_Id( const std::string& name )
{
    std::map<std::string, uint32>::iterator it;
    it = mTTFs.find( name );

    if ( it != mTTFs.end() )
    {
        return it->second;
    }

    return -1;
}

uint32 CFontManager::AddFont( uint32 size, bool bold, bool italica, const std::string& fontName, const std::string& fontFile, bool replaceDefault )
{
    LPD3DXFONT font = 0;
    D3DXCreateFont
      (
        m_pD3DDevice,
        size,
        0,
        (bold) ? FW_BOLD : FW_NORMAL,
        1,
        italica,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        fontName.c_str(),
        &font
      );

    uint32 id = m_Fonts.GetResourcesCount();

    m_Fonts.AddResource( fontFile, font );

    if ( replaceDefault )
    {
      mDefaultFont = font;
    }

    return id;
}

uint32 CFontManager::DrawDefaultText( uint32 x, uint32 y, Math::CColor color,
                                      const char* format, ... )
{
    std::string lMsg;
    va_list args;
    va_start(args,format);
    StringUtils::Format( lMsg, format, args); 
    va_end(args);
    
    RECT rect;
    rect.left = x;
    rect.top = y;
    DWORD color_aux = color.GetUint32Argb();

    // Calcular el tamaño del rectangulo necesario para pintar ese texto
    mDefaultFont->DrawText( NULL, lMsg.c_str(), -1, &rect, DT_CALCRECT, color_aux );
    // pintarlo
    mDefaultFont->DrawText( NULL, lMsg.c_str(), -1, &rect, DT_CENTER, color_aux );

    return ( rect.bottom  - rect.top );
}

uint32 CFontManager::DrawText( uint32 x, uint32 y, Math::CColor color,
                               uint32 idFont, const char* format, ... )
{
    va_list args;
    char* buffer;
    va_start( args, format );
    int len = _vscprintf( format, args ) + 1;
    buffer = ( char* )malloc( len * sizeof( char ) );
    vsprintf_s( buffer, len, format, args );
    va_end(args);
    RECT rect;
    rect.left = x;
    rect.top = y;
    DWORD color_aux = color.GetUint32Argb();
    // Calcular el tamaño del rectangulo necesario para pintar ese texto
    m_Fonts.GetResourceById(idFont)->DrawText( NULL, buffer, -1, &rect, DT_CALCRECT, color_aux );
    // pintarlo
    m_Fonts.GetResourceById(idFont)->DrawText( NULL, buffer, -1, &rect, DT_LEFT, color_aux );
    free( buffer );
    return ( rect.bottom  - rect.top );
}

uint32 CFontManager::DrawLiteral( uint32 x, uint32 y, const std::string& s_lit )
{
    CLanguageManager* languageM = LanguageInstance;
    SLiteral lit;

    if ( languageM->GetLiteral( s_lit, lit ) )
    {
        int32 ttfId = GetTTF_Id( lit.m_sFontId );

        if ( ttfId >= 0 )
        {
            return DrawText( x, y, lit.m_cColor, ttfId, lit.m_value.c_str() );
        }
        else
            return DrawDefaultText( x, y, Math::colGREEN, "Font Error: %s",
                                    lit.m_sFontId.c_str() );
    }

    return DrawDefaultText( x, y, Math::colGREEN, "Literal Error: %s",
                            s_lit.c_str() );
}


uint32 CFontManager::SizeX( const char* format, uint32 idFont )
{
    va_list args;
    char* buffer;
    va_start( args, format );
    int len = _vscprintf( format, args ) + 1;
    buffer = ( char* )malloc( len * sizeof( char ) );
    vsprintf_s( buffer, len, format, args );
    va_end(args);
    RECT rect;
    // Calcular el tamaño del rectangulo necesario para pintar ese texto
    m_Fonts.GetResourceById(idFont)->DrawText( NULL, buffer, -1, &rect, DT_CALCRECT, 0xff000000 );
    free( buffer );
    return ( rect.right  - rect.left );
}

uint32 CFontManager::SizeY( const char* format, uint32 idFont )
{
    va_list args;
    char* buffer;
    va_start( args, format );
    int len = _vscprintf( format, args ) + 1;
    buffer = ( char* )malloc( len * sizeof( char ) );
    vsprintf_s( buffer, len, format, args );
    va_end(args);
    RECT rect;
    // Calcular el tamaño del rectangulo necesario para pintar ese texto
    m_Fonts.GetResourceById(idFont)->DrawText( NULL, buffer, -1, &rect, DT_CALCRECT, 0xff000000 );
    free( buffer );
    return ( rect.bottom  - rect.top );
}