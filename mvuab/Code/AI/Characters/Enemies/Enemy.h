#ifndef _ENEMY_H
#define _ENEMY_H

#pragma once

#include "Math\Vector3.h"
#include "Utils\Defines.h"
#include "XML\XMLTreeNode.h"
#include "Characters\Character.h"

#include <string>

class CStateMachine;
class CRenderableObject;
class CAnimatedInstanceModel;
class CTrigger;
class CShoot;

class CEnemy : public CCharacter
{
public:
    typedef enum { eEasy = 0, ePatroll, eBoss } EEnemyType;
    CEnemy( CXMLTreeNode& Node, CStateMachine *aStateMachine );

    virtual ~CEnemy();
    virtual void Init();
    virtual void Update();
    virtual void Render();

    GET_SET_PTR( CStateMachine, StateMachine );
    GET_SET_PTR( CRenderableObject, RenderableObject );

    GET_SET( std::vector<std::string>, NameStates );
    GET_SET( std::string, CurrentState );
    GET_SET( bool, OnExit );

    void AddMesh( std::string );
    void ChangeState( std::string NewState );
    CAnimatedInstanceModel *GetAnimationModel();

    void MakeShoot( Math::Vect3f aDirection );
    void MoveAStar( Math::Vect3f aTargetPos );

protected:
    EEnemyType m_Type;
    bool m_OnEnter;
    bool m_OnExit;

    // State Machine instance
    CStateMachine *m_pStateMachine;
    std::string m_CurrentState;
    std::string m_NextState;

    std::vector<std::string> m_NameStates;
    CRenderableObject *m_pRenderableObject;

    std::vector<CShoot *> mShoots;
};

#endif

