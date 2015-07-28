#include "Cinematic.h"
#include "XML\XMLTreeNode.h"
#include "Math\Vector3.h"
#include "Logger\Logger.h"
#include "CinematicObject.h"

#include "Memory\FreeListAllocator.h"
#include "Memory\AllocatorManager.h"
#include "Memory\LinearAllocator.h"

#include "EngineConfig.h"
#include "EngineManagers.h"

CCinematic::CCinematic(const std::string &FileName) : CRenderableObject()  , CCinematicPlayer()
{
    CXMLTreeNode newFile;

    if (!newFile.LoadFile(FileName.c_str()))
    {
        LOG_ERROR_APPLICATION("CCinematic::Constructor --> Error loading XML %s.", FileName.c_str());
        return;
    }

    CXMLTreeNode cinematic = newFile["cinematic"];
    if (!cinematic.Exists())
    {
        LOG_ERROR_APPLICATION("CCinematic::Constructor --> Error reading %s, cinematic no existeix.", FileName.c_str());
        return;
    }

    m_Name = cinematic.GetAttribute<std::string>("name", "");
    m_Duration = cinematic.GetAttribute<float>("duration", 0.0f);

    CCinematicObject* l_CinematicObject = 0;
    CAllocatorManager *l_AllocatorManger = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();

    for( uint32 i = 0, lCount = cinematic.GetNumChildren(); i < lCount ; ++i)
    {
        l_CinematicObject = (CCinematicObject *) l_AllocatorManger->m_pFreeListAllocator->Allocate(sizeof(CCinematicObject), __alignof(CCinematicObject));
        new (l_CinematicObject) CCinematicObject(cinematic(i));

        l_CinematicObject->Init(m_Duration);
        m_CinematicObjects.push_back(l_CinematicObject);
    }
}

CCinematic::~CCinematic()
{
    CAllocatorManager *l_AllocatorManger = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();

    for(std::vector<CCinematicObject *>::iterator it = m_CinematicObjects.begin(); it != m_CinematicObjects.end(); ++it)
    {
        l_AllocatorManger->m_pFreeListAllocator->MakeDelete(*it);
        *it = 0;
    }

    m_CinematicObjects.clear();
}

void CCinematic::LoadXML(const std::string &FileName)
{
    CXMLTreeNode newFile;

    if (!newFile.LoadFile(FileName.c_str()))
    {
        LOG_ERROR_APPLICATION( "CCinematic::Constructor --> Error loading XML %s.", FileName.c_str());
        return;
    }

    CXMLTreeNode cinematic = newFile["cinematic"];
    if (!cinematic.Exists())
    {
        LOG_ERROR_APPLICATION("CCinematic::Constructor --> Error reading %s, cinematic no existeix.", FileName.c_str());
        return;
    }

    m_Name = cinematic.GetAttribute<std::string>("name", "");
    m_Duration = cinematic.GetAttribute<float>("duration", 0.0f);

    CAllocatorManager *l_AllocatorManger = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();
    for(uint32 i = 0, lCount = cinematic.GetNumChildren(); i < lCount; ++i)
    {
        std::string resource = cinematic(i).GetAttribute<std::string>("resource", "");

        CCinematicObject *l_CinematicObject = (CCinematicObject *) l_AllocatorManger->m_pFreeListAllocator->Allocate(sizeof(CCinematicObject), __alignof(CCinematicObject));
        new (l_CinematicObject) CCinematicObject(cinematic(i));

        m_CinematicObjects.push_back(l_CinematicObject);
    }
}

void CCinematic::AddCinematicObject(CCinematicObject *CinematicObject)
{
    m_CinematicObjects.push_back(CinematicObject);
}

void CCinematic::Update()
{
    std::vector<CCinematicObject *>::iterator it = m_CinematicObjects.begin();
    std::vector<CCinematicObject *>::iterator it_end = m_CinematicObjects.end();

    for(; it != it_end; ++it)
    {
        (*it)->Update();
    }
}

void CCinematic::Stop()
{
    std::vector<CCinematicObject *>::iterator it = m_CinematicObjects.begin();
    std::vector<CCinematicObject *>::iterator it_end = m_CinematicObjects.end();

    for(; it != it_end; ++it)
    {
        (*it)->Stop();
    }
}

void CCinematic::Play(bool Cycle)
{
    std::vector<CCinematicObject *>::iterator it = m_CinematicObjects.begin();
    std::vector<CCinematicObject *>::iterator it_end = m_CinematicObjects.end();

    for(; it != it_end; ++it)
    {
        (*it)->Play(Cycle);
    }
}

void CCinematic::Pause()
{
    std::vector<CCinematicObject *>::iterator it = m_CinematicObjects.begin();
    std::vector<CCinematicObject *>::iterator it_end = m_CinematicObjects.end();

    for(; it != it_end; ++it)
    {
        (*it)->Pause();
    }
}

void CCinematic::Render()
{
    std::vector<CCinematicObject *>::iterator it = m_CinematicObjects.begin();
    std::vector<CCinematicObject *>::iterator it_end = m_CinematicObjects.end();

    for(; it != it_end; ++it)
    {
        (*it)->Render();
    }
}