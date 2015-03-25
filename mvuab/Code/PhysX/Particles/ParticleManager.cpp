#include "ParticleManager.h"
#include "SphereEmitter.h"
#include "Utils\Defines.h"

CParticleManager::CParticleManager()
{
}

CParticleManager::~CParticleManager()
{
    for(unsigned int i = 0; i < m_Emitters.size(); ++i)
    {
        delete m_Emitters[i];
        m_Emitters[i] = 0;
    }


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
        CParticleEmitter *l_Emitter = NULL;

        std::string l_EmitterType = l_Node(i).GetPszProperty("EmitterType", "Sphere");
        if(l_EmitterType == "Sphere") l_Emitter =  new CSphereEmitter();

        Math::Vect2f l_TimeToLive = l_Node(i).GetVect2fProperty("LifeTime", Math::Vect2f());
        l_Emitter->SetTimeToLive(l_TimeToLive.x, l_TimeToLive.y);
       
        Math::Vect3f l_Acceleration = l_Node(i).GetVect3fProperty("Acceleration", Math::Vect3f());
        l_Emitter->SetAcceleration(l_Acceleration);

        Math::Vect3f l_Position = l_Node(i).GetVect3fProperty("Position", Math::Vect3f());
        l_Emitter->SetPosition(l_Position);
        
        Math::Vect3f l_Velocity = l_Node(i).GetVect3fProperty("Velocity", Math::Vect3f());
        l_Emitter->SetVelocity(l_Velocity);

        Math::Vect3f l_Direction = l_Node(i).GetVect3fProperty("Direction", Math::Vect3f());
        l_Emitter->SetDirection(l_Direction.Normalize());

        if(l_EmitterType == "Sphere")
        {
            Math::Vect2f l_Radius = l_Node(i).GetVect2fProperty("Radius", Math::Vect2f());
            ((CSphereEmitter *) l_Emitter)->SetRadius(l_Radius.x, l_Radius.y);

            Math::Vect2f l_Yaw = l_Node(i).GetVect2fProperty("Yaw", Math::Vect2f());
            ((CSphereEmitter *) l_Emitter)->SetYaw(l_Yaw.x, l_Yaw.y);

            Math::Vect2f l_Pitch = l_Node(i).GetVect2fProperty("Pitch", Math::Vect2f());
            ((CSphereEmitter *) l_Emitter)->SetPitch(l_Pitch.x, l_Pitch.y);
        }

        l_Emitter->Generate(l_Node(i).GetIntProperty("NumParticles", 0));
        m_Emitters.push_back(l_Emitter);
    }

    l_XML.Done();
    return true;
}

void CParticleManager::Update(float dt)
{
    for(std::vector<CParticleEmitter *>::iterator it = m_Emitters.begin(); it != m_Emitters.end(); ++it)
    {
        (*it)->Update(dt);
    }
}

void CParticleManager::Render()
{
    for(std::vector<CParticleEmitter *>::iterator it = m_Emitters.begin(); it != m_Emitters.end(); ++it)
    {
        (*it)->Render();
    }
}
