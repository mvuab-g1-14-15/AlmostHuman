#include "TriggerManager.h"
#include "XML\XMLTreeNode.h"
#include "PhysicsManager.h"
#include "Actor\PhysicActor.h"
#include "Utils\PhysicUserData.h"
#include "Logger\Logger.h"
#include "Core.h"
#include "Trigger.h"

CTriggerManager::CTriggerManager()
{
}

CTriggerManager::~CTriggerManager()
{
  Clear();
}

bool CTriggerManager::LoadXML( const std::string& FileName )
{
  m_FileName = FileName;
  CXMLTreeNode newFile;

  if ( !newFile.LoadFile( FileName.c_str() ) )
  {
    CLogger::GetSingletonPtr()->AddNewLog( ELL_ERROR,
                                           "CTriggerManager::LoadXML=> ERROR loading the file %s.\n", FileName.c_str() );
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
  if ( m_FileName != "" )
  {
    Clear();
    return LoadXML( m_FileName );
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
  // TODO RAUL RELEASE ALL THE TRIGGERS
  for ( TVectorResources::iterator it = m_ResourcesVector.begin();
        it < m_ResourcesVector.end(); ++it )
    ( *it )->Release();
}