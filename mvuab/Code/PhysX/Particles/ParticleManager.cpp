#include "ParticleManager.h"
#include "SphereEmitter.h"
#include "CubeEmitter.h"

CParticleManager::CParticleManager()
{
}

CParticleManager::~CParticleManager()
{
  for ( unsigned int i = 0; i < m_Emitters.size(); ++i )
  {
    delete m_Emitters[i];
    m_Emitters[i] = 0;
  }


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
    CParticleEmitter* l_Emitter = NULL;

    std::string l_EmitterType = l_Node( i ).GetPszProperty( "EmitterType", "Sphere" );

    if ( l_EmitterType == "Sphere" ) l_Emitter =  new CSphereEmitter();
    else if ( l_EmitterType == "Cube" ) l_Emitter =  new CCubeEmitter();

    l_Emitter->SetActive( true );
    l_Emitter->SetEmitterLifeTime( -1 );

    Math::Vect2f l_TimeToLive = l_Node( i ).GetVect2fProperty( "LifeTime", Math::Vect2f() );
    l_Emitter->SetLifeTime( l_TimeToLive.x, l_TimeToLive.y );

    Math::Vect3f l_Acceleration = l_Node( i ).GetVect3fProperty( "Acceleration", Math::Vect3f() );
    l_Emitter->SetAcceleration( l_Acceleration );

    Math::Vect3f l_Position = l_Node( i ).GetVect3fProperty( "Position", Math::Vect3f() );
    l_Emitter->SetPosition( l_Position );

    Math::Vect3f l_Velocity = l_Node( i ).GetVect3fProperty( "Velocity", Math::Vect3f() );
    l_Emitter->SetVelocity( l_Velocity );

    std::string l_TextureName = l_Node( i ).GetPszProperty( "Texture", "" );
    l_Emitter->SetTextureName( l_TextureName );

    size_t l_Min = l_Node( i ).GetIntProperty( "Min", 0 );
    l_Emitter->SetMin( l_Min );

    size_t l_Max = l_Node( i ).GetIntProperty( "Max", 0 );
    l_Emitter->SetMin( l_Max );

    if ( l_EmitterType == "Sphere" )
    {
      Math::Vect2f l_Radius = l_Node( i ).GetVect2fProperty( "Radius", Math::Vect2f() );
      ( ( CSphereEmitter* ) l_Emitter )->SetRadius( l_Radius.x, l_Radius.y );

      Math::Vect2f l_Yaw = l_Node( i ).GetVect2fProperty( "Yaw", Math::Vect2f() );
      ( ( CSphereEmitter* ) l_Emitter )->SetYaw( l_Yaw.x, l_Yaw.y );

      Math::Vect2f l_Pitch = l_Node( i ).GetVect2fProperty( "Pitch", Math::Vect2f() );
      ( ( CSphereEmitter* ) l_Emitter )->SetPitch( l_Pitch.x, l_Pitch.y );
    }
    else if ( l_EmitterType == "Cube" )
    {
      Math::Vect2f l_Depth = l_Node( i ).GetVect2fProperty( "Depth", Math::Vect2f() );
      ( ( CCubeEmitter* ) l_Emitter )->SetDepth( l_Depth.x, l_Depth.y );

      Math::Vect2f l_Width = l_Node( i ).GetVect2fProperty( "Width", Math::Vect2f() );
      ( ( CCubeEmitter* ) l_Emitter )->SetWidth( l_Width.x, l_Width.y );

      Math::Vect2f l_Height = l_Node( i ).GetVect2fProperty( "Height", Math::Vect2f() );
      ( ( CCubeEmitter* ) l_Emitter )->SetHeight( l_Height.x, l_Height.y );

      Math::Vect2f l_Radius = l_Node( i ).GetVect2fProperty( "Radius", Math::Vect2f() );
      ( ( CCubeEmitter* ) l_Emitter )->SetRadius( l_Radius.x, l_Radius.y );
    }

    l_Emitter->Generate( l_Node( i ).GetIntProperty( "NumParticles", 0 ) );
    m_Emitters.push_back( l_Emitter );

  }

  l_XML.Done();
  return true;
}

void CParticleManager::Update( float dt )
{
  for ( int i = 0; i < m_Emitters.size();  ++i )
  {
    if ( m_Emitters[i]->GetEmitterLifeTime() != -1 )
    {
      if ( m_Emitters[i]->GetTime() >= m_Emitters[i]->GetEmitterLifeTime() )
      {
        m_Emitters.erase( m_Emitters.begin() + i );
        continue;
      }
    }

    if ( m_Emitters[i]->GetActive() )
      m_Emitters[i]->Update( dt );
  }
}

void CParticleManager::Render()
{
  for ( std::vector<CParticleEmitter*>::iterator it = m_Emitters.begin(); it != m_Emitters.end(); ++it )
  {
    if ( ( *it )->GetActive() )
      ( *it )->Render();
  }
}

void CParticleManager::AddEmitter( CParticleEmitter* Emitter )
{
  m_Emitters.push_back( Emitter );
}
