#include "RenderableObjectsManager.h"
#include "RenderableObject.h"

#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"

#include "StaticMeshes\InstanceMesh.h"
#include "AnimatedModels\AnimatedInstanceModel.h"

#include "Math\MathTypes.h"
#include "Cinematics\Cinematic.h"

#include "Memory\FreeListAllocator.h"
#include "Memory\AllocatorManager.h"
#include "Memory\LinearAllocator.h"

#include "EngineConfig.h"
#include "EngineManagers.h"

CRenderableObjectsManager::CRenderableObjectsManager()
{
}

CRenderableObjectsManager::~CRenderableObjectsManager()
{
    CleanUp();
}

bool CRenderableObjectsManager::Load( const std::string& FileName )
{
    CXMLTreeNode newFile;
    CXMLTreeNode m;

    if ( !newFile.LoadFile( FileName.c_str() ) )
    {
        LOG_ERROR_APPLICATION( "CRenderableObjectsManager::Load --> Error loading XML %s.", FileName.c_str() );
        return false;
    }

    m = newFile["RenderableObjects"];
    if ( !m.Exists() )
    {
        LOG_ERROR_APPLICATION( "CRenderableObjectsManager::Load --> RenderableObjects tag not found in the file %s", FileName.c_str());
        return false;
    }

    for ( int i = 0; i < m.GetNumChildren(); ++i )
    {
        const std::string& l_TagName = m( i ).GetName();
        const std::string& l_Name = m( i ).GetPszProperty( "name", "" );

        if ( l_TagName == "MeshInstance" )
        {
            CInstanceMesh* l_InstanceMesh = new CInstanceMesh( m( i ) );
            if ( !AddResource( l_Name, l_InstanceMesh ) ) CHECKED_DELETE( l_InstanceMesh );
        }
        else if ( l_TagName == "AnimatedInstance" )
        {
            CAnimatedInstanceModel* l_AnimatedInstance = new CAnimatedInstanceModel( m( i ) );
            if ( !AddResource( l_Name, l_AnimatedInstance ) ) CHECKED_DELETE( l_AnimatedInstance );
        }
    }

    //CCinematic* l_CinematicInstance = new CCinematic("Data/cinematic.xml");
    //l_CinematicInstance->Play(true);
    //AddResource(l_CinematicInstance->GetName(), l_CinematicInstance);
    return true;
}

void CRenderableObjectsManager::CleanUp()
{
    CAllocatorManager *l_AllocatorManager = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();
    for(unsigned int i =  0; i < m_ResourcesVector.size(); ++i)
    {
        l_AllocatorManager->m_pFreeListAllocator->MakeDelete(m_ResourcesVector[i]);
    }

    m_ResourcesMap.clear();
    m_ResourcesVector.clear();
}

void CRenderableObjectsManager::Render()
{
    for ( unsigned int i = 0; i < m_ResourcesVector.size(); ++i )
    {
        std::string name = m_ResourcesVector[i]->GetName();
        m_ResourcesVector[i]->Render();
    }
}

void CRenderableObjectsManager::Update()
{
    for (unsigned int i = 0; i < m_ResourcesVector.size(); ++i) m_ResourcesVector[i]->Update();
}

void CRenderableObjectsManager::RemoveResource(const std::string &l_Name)
{
    TMapResources::iterator it = m_ResourcesMap.find(l_Name);
    if ( it == m_ResourcesMap.end() ) return;

    CAllocatorManager *l_AllocatorManager = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();
    l_AllocatorManager->m_pFreeListAllocator->MakeDelete(it->second.m_Value);
    unsigned int l_ID = it->second.m_Id;

    m_ResourcesMap.erase(it);
    m_ResourcesVector.erase(m_ResourcesVector.begin() + l_ID);

    for (size_t i =  l_ID; i < m_ResourcesVector.size();  ++i)
    {
        TMapResources::iterator l_ItMap = m_ResourcesMap.begin();

        while (l_ItMap->second.m_Value != m_ResourcesVector[i]) ++l_ItMap;
        l_ItMap->second.m_Id = i;
    }
}

//CRenderableObject* CRenderableObjectsManager::AddMeshInstance( const std::string& CoreMeshName,
//    const std::string& InstanceName, const Math::Vect3f& Position )
//{
//  return NULL; //TODO RAUl
//}
//
//CRenderableObject* CRenderableObjectsManager::AddAnimatedInstanceModel(
//  const std::string& CoreModelName, const std::string& InstanceModelName,
//  const Math::Vect3f& Position )
//{
//  return NULL; //TODO RAUl
//}

CCinematic* CRenderableObjectsManager::CreateCinematic( const std::string& FileName )
{
    CAllocatorManager *l_AllocatorManager = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();

    CCinematic *l_Cinematic = (CCinematic *) l_AllocatorManager->m_pFreeListAllocator->Allocate(sizeof(CCinematic), __alignof(CCinematic));
    new (l_Cinematic) CCinematic(FileName);

    AddResource(l_Cinematic->GetName(), l_Cinematic);
    return l_Cinematic;
}