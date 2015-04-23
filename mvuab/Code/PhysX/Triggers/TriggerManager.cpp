#include "TriggerManager.h"
#include "XML\XMLTreeNode.h"
#include "PhysicsManager.h"
#include "Actor\PhysicActor.h"
#include "Utils\PhysicUserData.h"
#include "Logger\Logger.h"

#include "Trigger.h"
#include "EngineConfig.h"

CTriggerManager::CTriggerManager()
    : CManager()
{
}

CTriggerManager::CTriggerManager(CXMLTreeNode& atts)
    : CManager(atts)
{
}
CTriggerManager::~CTriggerManager()
{
    Clear();
}

void CTriggerManager::Init()
{
    LoadXML(mConfigPath);
}
bool CTriggerManager::LoadXML( const std::string& FileName )
{
    mConfigPath = FileName;
    CXMLTreeNode newFile;

    if ( !newFile.LoadFile( FileName.c_str() ) )
    {
        LOG_ERROR_APPLICATION( "CTriggerManager::LoadXML=> ERROR loading the file %s.\n", FileName.c_str() );
        return false;
    }

    CXMLTreeNode  m = newFile["triggers"];

    if ( m.Exists() )
    {
        int count = m.GetNumChildren();

        for ( int i = 0; i < count; ++i )
        {
            const std::string l_TagName = m( i ).GetName();

            if ( l_TagName == "trigger" )
            {
                CTrigger* l_Trigger = new CTrigger( m( i ) );
                AddResource( l_Trigger->GetName(), l_Trigger );
            }
        }
    }

    return true;
}

bool CTriggerManager::Reload()
{
    if ( mConfigPath != "" )
    {
        Clear();
        return LoadXML( mConfigPath );
    }

    return false;
}

bool CTriggerManager::ReloadXML( const std::string& FileName )
{
    Clear();
    return LoadXML( FileName );
}

void CTriggerManager::Clear()
{
    // TODO JAUME
    Destroy();
}


void CTriggerManager::Release()
{
    TVectorResources::iterator itb = m_ResourcesVector.begin();

    for ( ; itb != m_ResourcesVector.end(); ++itb )
    { ( *itb )->Release(); }
}

std::vector<CTrigger*> CTriggerManager::GetTriggersVector()
{
    return m_ResourcesVector;
}
CTrigger* CTriggerManager::GetTriggerByName( std::string Name )
{
    return GetResource( Name );
}