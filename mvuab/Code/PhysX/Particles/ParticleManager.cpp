#include "ParticleManager.h"
#include "Utils\Defines.h"

CParticleManager::CParticleManager()
{
}

CParticleManager::~CParticleManager()
{
    m_Emitters.clear();
}

bool CParticleManager::Init(const std::string &path)
{
    CXMLTreeNode l_XML;
    if(!l_XML.LoadFile(path.c_str()))
    {
        LOG_ERROR_APPLICATION("CParticleManager::Init Can't load XML file");
        return false;
    }

    CXMLTreeNode l_Node = l_XML["emitters"];
    if(!l_Node.Exists())
    {
        LOG_ERROR_APPLICATION("CParticleManager::Init Can't find node \"emitters\"");
        l_XML.Done(); return false;
    }

    for(int i = 0; i < l_Node.GetNumChildren(); ++i)
    {
        CParticleEmitter l_Emitter;


        l_Emitter.SetTimeToLive(l_Node.GetFloatProperty("time", 0.0f));
        l_Emitter.SetPosition(l_Node.GetVect3fProperty("pos", Math::Vect3f()));
        l_Emitter.SetVelocity(l_Node.GetVect3fProperty("vel", Math::Vect3f()));
        l_Emitter.SetDirection(l_Node.GetVect3fProperty("dir", Math::Vect3f()));

        l_Emitter.Generate();
        m_Emitters.push_back(std::move(l_Emitter));
        
    }

    l_XML.Done();
    return true;
}