#include "ParticleManager.h"
#include "Utils\Defines.h"

CParticleManager::CParticleManager()
{
}

CParticleManager::~CParticleManager()
{
  m_Emitters.clear();
}

bool CParticleManager::Init( const std::string& path )
{
  CXMLTreeNode l_XML;

  if ( !l_XML.LoadFile( path.c_str() ) )
  {
    LOG_ERROR_APPLICATION( "CParticleManager::Init Can't load XML file" );
    return false;
  }

  CXMLTreeNode l_Node = l_XML["emitters"];

  if ( !l_Node.Exists() )
  {
    LOG_ERROR_APPLICATION( "CParticleManager::Init Can't find node \"emitters\"" );
    l_XML.Done();
    return false;
  }

  for ( int i = 0; i < l_Node.GetNumChildren(); ++i )
  {
    CParticleEmitter l_Emitter;

    l_Emitter.SetTimeToLive( l_Node( i ).GetFloatProperty( "time", 0.0f ) );
    l_Emitter.SetMax( l_Node( i ).GetIntProperty( "max", 0 ) );
    l_Emitter.SetMin( l_Node( i ).GetIntProperty( "min", 0 ) );

    l_Emitter.SetTextureName( l_Node( i ).GetPszProperty( "texture", "" ) );

    l_Emitter.SetAcceleration( l_Node( i ).GetVect3fProperty( "acc", Math::Vect3f() ) );
    l_Emitter.SetPosition( l_Node( i ).GetVect3fProperty( "pos", Math::Vect3f() ) );
    l_Emitter.SetVelocity( l_Node( i ).GetVect3fProperty( "vel", Math::Vect3f() ) );
    l_Emitter.SetDirection( l_Node( i ).GetVect3fProperty( "dir", Math::Vect3f() ) );

    l_Emitter.Generate( l_Node( i ).GetIntProperty( "num", 0 ) );
    m_Emitters.push_back( l_Emitter );
  }

  l_XML.Done();
  return true;
}

void CParticleManager::Update( float dt )
{
  for ( std::vector<CParticleEmitter>::iterator it = m_Emitters.begin(); it != m_Emitters.end(); ++it )
    it->Update( dt );
}

void CParticleManager::Render()
{
  for ( std::vector<CParticleEmitter>::iterator it = m_Emitters.begin(); it != m_Emitters.end(); ++it )
    it->Render();
}
