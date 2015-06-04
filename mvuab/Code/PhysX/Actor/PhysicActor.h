//----------------------------------------------------------------------------------
// CPhysicActor class
// Author: Enric Vergara
//
// Description:
// Esta clase representa un actor físico.
//----------------------------------------------------------------------------------
#pragma once
#ifndef __PHYSIC_ACTOR_CLASS_H__
#define __PHYSIC_ACTOR_CLASS_H__

#include <vector>
////#include "Utils\Base.h"
#include "Math\Vector3.h"
#include "Math\Matrix44.h"
#include "PhysicsDefs.h"

#undef min
#undef max

#include "NxMaterial.h"
#include "Utils\Defines.h"

using namespace Math;

//---Forward Declarations---
class NxActor;
class NxActorDesc;
class NxBodyDesc;
class NxCCDSkeleton;
class NxTriangleMesh;
class NxBoxShapeDesc;
class NxBoxShapeDesc;
class NxTriangleMeshShapeDesc;
class NxCapsuleShapeDesc;
class NxSphereShapeDesc;
class NxPlaneShapeDesc;
class CPhysicUserData;

enum NxForceMode;

class CPhysicUserData;
//--------------------------

class CPhysicActor
{
public:
  //--- Init and End protocols------------------------------------------
  CPhysicActor::CPhysicActor( CPhysicUserData* _pUserData );
  //CPhysicActor::CPhysicActor  ( NxActor * _pActor );
  CPhysicActor::~CPhysicActor();

  //---- Main Functions ---------------------------------------
  void        CreateBody( float _Density, float _AngularDamping = 0.5f, float _LinearDamping = 0.5f );

  //---Trigger Function---
  void        CreateBoxTrigger( const Math::Vect3f& _vGlobalPos, const Math::Vect3f& _vSize,
                                uint32 _uiGroup = 0 );
  void        CreateSphereTrigger( const Math::Vect3f& _vGlobalPos, const float _fRadius,
                                   uint32 _uiGroup );
  void        ActivateAllTriggers( void );

  //---Get PhsX Info---
  void        CreateActor( NxActor* actor );

  //---- Functions ---------------------------------------
  void        AddImpulseAtPos( const Math::Vect3f& _vDirection, const Math::Vect3f& _vPos,
                               float _fPower, bool _bLocal = true );
  void        AddVelocityAtPos( const Math::Vect3f& _vDirection, const Math::Vect3f& _vPos,
                                float _fPower, bool _bLocal = true );
  void        AddAcelerationAtPos( const Math::Vect3f& _vDirection, const Math::Vect3f& _vPos,
                                   float _fPower, bool _bLocal = true );
  void        AddForceAtPos( const Math::Vect3f& _vDirection, const Math::Vect3f& _vPos,
                             float _fPower, bool _bLocal = true );
  void        AddTorque( const Math::Vect3f  _vTorque );

  //---AddShape Functions-----
  void        AddSphereShape( float radius, const Math::Vect3f& _vGlobalPos = v3fZERO,
                              const Math::Vect3f& _vLocalPos = v3fZERO, NxCCDSkeleton* _pSkeleton = 0, uint32 _uiGroup = 0 );
  void        AddBoxShape( const Math::Vect3f& _vSize, const Math::Vect3f& _vGlobalPos = v3fZERO,
                           const Math::Vect3f& _vLocalPos = v3fZERO, const Math::Vect3f& rotation = v3fZERO,
                           NxCCDSkeleton* _pSkeleton = 0, uint32 _uiGroup = 0 );
  void        AddCapsuleShape( float _Radius, float _Height,
                               const Math::Vect3f& _vGlobalPos = v3fZERO, const Math::Vect3f& _vLocalPos = v3fZERO,
                               NxCCDSkeleton* _pSkeleton = 0, uint32 _uiGroup = 0 );
  void        AddMeshShape( NxTriangleMesh* mesh, const Math::Vect3f& _vGlobalPos = v3fZERO,
                            const Math::Vect3f& _vLocalPos = v3fZERO, NxCCDSkeleton* _pSkeleton = 0, uint32 _uiGroup = 0 );
  void        AddPlaneShape( const Math::Vect3f& _vNormal, float _Distance, uint32 _uiGroup = 0 );

  //AddShape Functions hardcoded
  void        AddBoxShapeHardcoded( const Math::Vect3f& _vSize, const Math::Vect3f& _vGlobalPos = v3fZERO,
                             const Math::Vect3f& localPos = v3fZERO, const Math::Vect3f& rotation = v3fZERO );
  void        AddSphereShapeHardcoded( float radius, const Math::Vect3f& _vGlobalPos = v3fZERO, const Math::Vect3f& _vLocalPos = v3fZERO );
  //---Activate---
  void        Activate( bool _bActivate );
  void        SetKinematic( bool _bValue );

  //---- Properties ( get & Set )---------------------------------------
  CPhysicUserData*    GetUserData()
  {
    return m_pUserData;
  }

  void                SetLinearVelocity( const Math::Vect3f& _vVelocity );
  Math::Vect3f        GetLinearVelocity();

  Math::Vect3f        GetPosition();
  Math::Vect3f        GetRotation();
  void                SetRotation( const Math::Vect3f& _vRot );
  void                SetRotation( const Mat33f& _mRot );

  void                SetAngularVelocity( const Math::Vect3f  _vVelocity );
  Math::Vect3f        GetAngularVelocity();

  void                SetGlobalPosition( const Math::Vect3f& _vPos = Math::Vect3f( 0.f, 0.f, 0.f ) );
  void                MoveGlobalPosition( const Math::Vect3f& _vPos );

  Math::Vect3f        GetAngularMomentum();

  Mat33f              GetInertiaTensor();

  void                SetCollisionGroup( uint32 _uiGroup );
  uint32              GetColisionGroup( void ) const
  {
    return m_uCollisionGroups;
  }
  ECollisionGroup     GetPhXColisionGroup( void ) const
  {
    return m_uCollisionGroups;
  }

  void                SetTriggerGroup( ECollisionGroup _uiGroup )
  {
    m_TriggerGroup = _uiGroup;
  }
  ECollisionGroup     GetTriggerGroup( void ) const
  {
    return m_TriggerGroup;
  }

  GET_SET( NxMaterialIndex, MaterialIndex );

  //---Get Info-------
  void        GetMat44( Mat44f& _mMatrix ) const;
  void        SetMat44( const Mat44f& _mMatrix );
  void        MoveGlobalPoseMat44( const Mat44f& _mMatrix );

  //---Get PhsX Info---
  NxActor*        GetPhXActor()
  {
    return m_pPhXActor;
  }
  NxActorDesc*    GetActorDesc()
  {
    return m_pPhXActorDesc;
  }

  //---Get Info-------
  void        SetActorSolverIterationCount( int _iCount );
  void        SetContactReportFlags( unsigned int _uiFlags );
  void        SetContactReportThreshold( float _fThreshold );

  void        Destroy();
private:
  //  void        Destroy           ();
  void        AddForceAtPos( const Math::Vect3f& _vDirection, const Math::Vect3f& _vPos,
                             float _fPower, NxForceMode _sForceMode, bool _bLocal );

  //---- Members ---------------------------------------
private:

  CPhysicUserData*            m_pUserData;
  NxActor*                    m_pPhXActor;
  NxActorDesc*                m_pPhXActorDesc;
  NxBodyDesc*                 m_pPhXBodyDesc;

  std::vector<NxBoxShapeDesc*>            m_vBoxDesc;
  std::vector<NxTriangleMeshShapeDesc*>   m_vMeshDesc;
  std::vector<NxCapsuleShapeDesc*>        m_vCapsuleDesc;
  std::vector<NxSphereShapeDesc*>         m_vSphereDesc;
  std::vector<NxPlaneShapeDesc*>          m_vPlaneDesc;

  ECollisionGroup               m_uCollisionGroups;
  ECollisionGroup               m_myCollisionGroups;
  ECollisionGroup               m_TriggerGroup;

  NxMaterialIndex               m_MaterialIndex;
};

#endif __PHYSIC_ACTOR_CLASS_H__