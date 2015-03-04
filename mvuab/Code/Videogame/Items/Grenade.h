#ifndef Grenade_H
#define Grenade_H

#include <string>
#include <vector>
class CPhysicActor;
class CPhysicUserData;
//class CStaticMesh;

class CGrenade
{
  private:
    float m_TimeAux;
    float m_ForceMin;
    float m_ForceMax;
    float m_Force;
    std::vector<CPhysicUserData*> m_vPhysicUserData;
    CPhysicUserData* m_PhysicUserData;
    std::vector<CPhysicActor*> m_vActor;
    CPhysicActor* m_Actor;
    //CStaticMesh* m_StaticMesh;
    std::string m_MeshName;
    bool m_GrenadeThrown;
    float m_ElapsedTime;

  public:
    CGrenade(float a_TimeAux, float a_ForceMin, float a_ForceMax, float a_Force, std::string a_MeshName);
    virtual ~CGrenade();

    void Start();
    void Update();
    void Render();
};

#endif // Grenade_H
