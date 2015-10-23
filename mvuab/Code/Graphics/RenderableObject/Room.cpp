#include "Room.h"
#include "Utils\Defines.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"
#include "Lights\LightProbe.h"
#include "EngineManagers.h"
#include "Lights/LightManager.h"
#include "StaticMeshes/StaticMeshManager.h"
#include "PhysicsManager.h"
#include "Utils/Defines.h"
#include <algorithm>
#include "CookingMesh/PhysicCookingMesh.h"
#include "Utils/PhysicUserData.h"
#include "Actor/PhysicActor.h"
#include "Billboard/BillboardManager.h"
#include "Particles/ParticleSystemManager.h"
#include "WWSoundManager.h"
#include "Joints\PhysicRevoluteJoint.h"
#include "Cinematics/CinematicManager.h"

CRoom::CRoom( const CXMLTreeNode& aNode )
    : CName( aNode.GetAttribute<std::string>("level", "no_level") )
    , m_BasePath( aNode.GetAttribute<std::string>( "path", "no_path" ) )
    , m_RenderableObjectsPath( aNode.GetAttribute<std::string>( "renderable_objects_file", "no_file" ) )
    , m_StaticMeshesPath( aNode.GetAttribute<std::string>( "static_meshes_file", "no_file" ) )
    , m_pLayers( 0 )
    , m_Active( false )
    , mIsLoaded( false )
{
    m_BasePath                += "/";
    m_RenderableObjectsPath = m_BasePath + m_RenderableObjectsPath;
    m_StaticMeshesPath      = m_BasePath + m_StaticMeshesPath;
    LoadLightProbe();
}

CRoom::~CRoom() 
{ 
    CHECKED_DELETE(m_pLayers);

    for(unsigned int i = 0; i < mLightProbes.size(); i++)
    {
        CHECKED_DELETE(mLightProbes[i]);
    }
}

void CRoom::LoadLightProbe()
{
    bool lOk = false;
    if ( m_BasePath != "" )
    {
        std::string lPath = m_BasePath + "light_probe.xml";
        CXMLTreeNode l_Root, l_Node;
        if ( l_Root.LoadAndFindNode( lPath.c_str(), "light_probe_room", l_Node ) )
        {
            for ( int i = 0, l_NumChilds = l_Node.GetNumChildren(); i < l_NumChilds; ++i )
            {
                CXMLTreeNode& l_CurrentNode = l_Node( i );

                CLightProbe* lLightProbe = new CLightProbe( l_CurrentNode );
                if (lLightProbe)
                    mLightProbes.push_back( lLightProbe );
            }
        }
    }
}

struct SPointDist
{
    CLightProbe* lightprobe;
    float distance;
};

bool SPointDistComparison( SPointDist a, SPointDist b)
{
    return a.distance < b.distance;
};

std::vector<CLightProbe*> CRoom::GetClosedLightProbes( Math::Vect3f aPos )
{
  std::vector<CLightProbe*> lLightProbes;

  if (!mLightProbes.empty())
  {
      std::vector<SPointDist> lDistances;
      for( unsigned int i = 0; i < mLightProbes.size(); ++i)
      {
          Math::Vect3f lPos = mLightProbes[i]->GetPosition();
          float lDist = lPos.SqDistance( aPos );
          SPointDist s = { mLightProbes[i], lDist };
          lDistances.push_back( s );
      }

      std::sort( lDistances.begin(), lDistances.end(), SPointDistComparison);

      for (int i = 0; i < 4; ++i)
          lLightProbes.push_back(lDistances[i].lightprobe);
  }

    return lLightProbes;
}

void CRoom::RenderLayer( const std::string& aLayerName )
{
  BROFILER_CATEGORY( GetName().c_str(), Profiler::Color::Orchid )
    if( m_pLayers ) m_pLayers->Render(aLayerName);
}

void CRoom::Load()
{
    LoadMeshes();
    LoadInstances();
    const std::string& lName = GetName();
    // The core room must not load some elements
    if( lName != "core" )
    {
        CPhysicsManager *lPM = PhysXMInstance;
        if (lPM->GetLoadASE())
        {
            if (PhysXMInstance->GetCookingMesh()->CreateMeshFromASE(m_BasePath + "" + GetName() + ".ase", GetName(), GetName()))
            {
                CPhysicUserData* l_pPhysicUserDataASEMesh = new CPhysicUserData( lName + "Escenario" );
                l_pPhysicUserDataASEMesh->SetColor( Math::colBLACK );
                CPhysicActor* l_AseMeshActor = new CPhysicActor( l_pPhysicUserDataASEMesh );

                VecMeshes l_CookMeshes = PhysXMInstance->GetCookingMesh()->GetMeshes(GetName());

                for ( VecMeshes::iterator it = l_CookMeshes.begin(); it != l_CookMeshes.end(); it++ )
                    l_AseMeshActor->AddMeshShape( it->second, Vect3f( 0, 0, 0 ) );

                lPM->AddPhysicActor( l_AseMeshActor );
            }
        }

		if (lPM->GetLoadASE())
        {
            if (PhysXMInstance->GetCookingMesh()->CreateMeshFromASE(m_BasePath + "" + GetName() + "_door.ase", GetName()+"_door", GetName() + "_door"))
            {
                CPhysicUserData* l_pPhysicUserDataASEMesh = new CPhysicUserData( lName + "DoorEscenario" );
                l_pPhysicUserDataASEMesh->SetColor( Math::colBLACK );
                CPhysicActor* l_AseMeshActor = new CPhysicActor( l_pPhysicUserDataASEMesh );

                VecMeshes l_CookMeshes = PhysXMInstance->GetCookingMesh()->GetMeshes(GetName()+"_door");

                for ( VecMeshes::iterator it = l_CookMeshes.begin(); it != l_CookMeshes.end(); it++ )
                    l_AseMeshActor->AddMeshShape( it->second, Vect3f( 0, 0, 0 ) );
				
                lPM->AddPhysicActor( l_AseMeshActor );
            }
        }

        BillboardMan->LoadInstances         ( m_BasePath + "billboards.xml");
        PSManager->LoadInstances            ( m_BasePath + "particles.xml");
        SoundInstance->LoadParticlePosSound ( m_BasePath + "particles.xml");
        CinematicMInstance->LoadCinematics  ( m_BasePath + "cinematics.xml");
        LoadLights();
    }

    mIsLoaded = true;
}

void CRoom::Unload()
{

}

void CRoom::LoadMeshes()
{
    SMeshMInstance->Load( m_StaticMeshesPath, m_BasePath );
}

void CRoom::LoadInstances()
{
    m_pLayers = new CRenderableObjectsLayersManager();
    m_pLayers->LoadLayers( m_RenderableObjectsPath, GetName() );
}

void CRoom::LoadLights()
{
    LightMInstance->Load( m_BasePath + "lights.xml" );
}

CRenderableObjectsManager* CRoom::GetLayer( const std::string& aLayer ) const 
{
    ASSERT( m_pLayers, "Null Layers for room %s", GetName().c_str() );
    return m_pLayers->GetResource(aLayer);
}

void CRoom::Update()
{
  BROFILER_CATEGORY( GetName().c_str(), Profiler::Color::Orchid )
  m_pLayers->Update();
}