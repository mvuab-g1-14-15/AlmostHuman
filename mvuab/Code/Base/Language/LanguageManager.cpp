#include "Language\LanguageManager.h"
#include "xml/XMLTreeNode.h"
#include "Logger/Logger.h"
#include "Utils\Defines.h"
#include "EngineConfig.h"

CLanguageManager::CLanguageManager( CXMLTreeNode& atts )
    : CManager( atts )
    , m_sCurrentLanguage( "" )
{
}

void CLanguageManager::Init()
{
    CXMLTreeNode l_File;
    const char* lFileStr = mConfigPath.c_str();

    if ( !l_File.LoadFile( lFileStr ) )
    {
        FATAL_ERROR( "Error reading the configuration file of the engine in %s", lFileStr );
    }

    CXMLTreeNode  TreeNode = l_File["languages"];

    if ( TreeNode.Exists() )
    {
        for ( uint32 lans = 0, lCountLan = TreeNode.GetNumChildren(); lans < lCountLan; ++lans )
        {
            const std::string& TagName = TreeNode( lans ).GetName();

            if ( TagName == "language" )
            {
                m_vXML_Files.push_back( std::string( TreeNode( lans ).GetAttribute<std::string>( "path", "" ) ) );
            }
        }
    }

    LoadXMLs();
    SetCurrentLanguage( l_File.GetAttribute<std::string>( "current_language", "" ) );
}
void CLanguageManager::LoadXMLs()
{
    for( std::map< std::string , TLanguage >::iterator lItb = m_Languages.begin(), lIte = m_Languages.end();
            lItb != lIte; ++lItb )
    {
        lItb->second.clear();
        ++lItb;
    }
    m_Languages.clear();

    for( std::vector<std::string>::iterator lItb = m_vXML_Files.begin(), lIte = m_vXML_Files.end();
            lItb != lIte; ++lItb )
    {
        LoadXML( *lItb );
    }
}

void CLanguageManager::LoadXML( const std::string& pathFile )
{
    CXMLTreeNode parser, m;
    if ( parser.LoadAndFindNode( pathFile.c_str(), "Language", m ) )
    {
        CXMLTreeNode  m = parser["Language"];
        const std::string & id_language    = m.GetAttribute<std::string>("id", "no_id");
        TLanguage language;

        if ( m.Exists() )
        {
            for ( uint32 i = 0, lCount = m.GetNumChildren(); i < lCount; ++i )
            {
                const CXMLTreeNode& lNode = m(i);
                const std::string& lId = lNode.GetAttribute<std::string>("id", "no_id");

                if( lId != "no_id")
                {
                    SLiteral l_literal =
                    {
                        lNode.GetAttribute<std::string>("font", "no_font"),
                        lNode.GetAttribute<Math::CColor>("color", Math::CColor() ),
                        lNode.GetAttribute<std::string>("value", "no_value"),
                    };
                    language.insert( std::pair<std::string, SLiteral>( lId, l_literal ) );
                }
            }
        }

        if ( m_Languages.find( id_language ) != m_Languages.end() )
        {
            //Ya está registrado el identificador id_language
            LOG_WARNING_APPLICATION( "LoadXML-> EYa se ha registrado un language con identificador %s",
                                     id_language.c_str() );
        }
        else
        {
            m_Languages.insert( std::pair<std::string, TLanguage>( id_language, language ) );
        }
    }
}

void CLanguageManager::SetXmlFile( const std::string & pathFile )
{
    typedef std::vector<std::string>::const_iterator const_vector_string_iterator;

    for ( const_vector_string_iterator lItb = m_vXML_Files.begin(),
            lIte = m_vXML_Files.end();
            lItb != lIte; ++lItb )
    {
        if ( *lItb == pathFile )
        {
            m_vXML_Files.push_back( pathFile );
            return;
        }
    }
}

bool CLanguageManager::GetLiteral( const std::string & id, SLiteral & sliteral ) const
{
    std::map<std::string, TLanguage>::const_iterator it = m_Languages.find( m_sCurrentLanguage );

    if ( it != m_Languages.end() )
    {
        TLanguage currentLanguage = it->second;
        TLanguage::const_iterator it_aux = currentLanguage.find( id );

        if ( it_aux != currentLanguage.end() )
        {
            sliteral = it_aux->second;
            return true;
        }
    }

    return false;
}
void CLanguageManager::SetCurrentLanguage( const std::string & id )
{
    std::map<std::string, TLanguage>::iterator it = m_Languages.find( id );

    if ( it != m_Languages.end() )
    {
        m_sCurrentLanguage = id;
        //LOG_INFO_APPLICATION( "SetCurrentLanguage-> Se ha seteado como current el language %s", id.c_str() );
    }
    else
    {
        LOG_WARNING_APPLICATION( "LanguageManager::SetCurrentLanguage-> El language %s no esta registrado", id.c_str() );
    }
}