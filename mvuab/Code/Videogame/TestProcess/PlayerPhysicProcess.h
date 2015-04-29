#ifndef _PLAYER_PHYSIC_PROCESS_H
#define _PLAYER_PHYSIC_PROCESS_H
#pragma once

#include "Process.h"
#include "PhysicsDefs.h"
#include "Utils\Types.h"
#include "Math\Vector3.h"

#include <vector>

class CPhysicActor;
class CPhysicUserData;
class CPhysicController;
class CCharacter;
class CGrenade;
class CBlaster;
class CAStar;
//class CBillboard;

class CPlayerPhysicProcess : public CProcess
{
    private:
        Math::Vect3f          m_PointInicial;
        Math::Vect3f          m_PointFinal;
        std::vector<Math::Vect3f>   m_Path;
        std::vector<CPhysicActor*>    m_vPA;
        std::vector<CPhysicUserData*> m_vPUD;
        std::vector<CPhysicController*>      m_vController;
        CPhysicController*            m_PhysicController;
        CGrenade*                     m_Grenade;
        CBlaster*                     m_Blaster;
        //CBillboard*                   m_Billboard;
        //CCharacter*               m_Character;

    public:
        CPlayerPhysicProcess();
        virtual ~CPlayerPhysicProcess();

        virtual void Init();
        virtual void Update();
        virtual void Render();
        virtual void RenderDebugInfo();

        GET_SET( Math::Vect3f, PointInicial );
        GET_SET( Math::Vect3f, PointFinal );
        CPhysicUserData*    GetNewPUD( const std::string& Name );
        CPhysicActor*       GetNewPhysicActor( CPhysicUserData* PUD );
        CPhysicController*  GetNewController( float _fRadius, float _fHeight, float _fSlope, float _fSkinwidth,
                                              float _fStepOffset,
                                              CPhysicUserData* _pUserData, const Math::Vect3f& _vPos = Math::Vect3f( 0, 0, 0 ), float _fGravity = -10 );
        void                AddPudVector( CPhysicUserData* PUD );
        void                AddPhysicActorVector( CPhysicActor* PA );
        CPhysicUserData*    GetLastPUDInserted();
        void                DeleteController( CPhysicUserData* PUD );

        void InitSceneCharacterController();


};

#endif // _PLAYER_PHYSIC_PROCESS_H
