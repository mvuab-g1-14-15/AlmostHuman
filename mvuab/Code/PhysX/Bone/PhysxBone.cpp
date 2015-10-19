#define __DONT_INCLUDE_MEM_LEAKS__
#include "Bone/PhysxBone.h"
#include <cal3d/cal3d.h>
#include "PhysicsManager.h"
#include "Actor/PhysicActor.h"
#include "Actor/PhysicController.h"
#include "Joints/PhysicFixedJoint.h"
#include "Joints/PhysicSphericalJoint.h"
#include "Utils/PhysicUserData.h"
#include "Math\Matrix44.h"

#include "AnimatedModels\AnimatedModelsManager.h"
#include "GraphicsManager.h"
#include <XML/XMLTreeNode.h>
#include "EngineManagers.h"
#include "Utils/Defines.h"

//#include "base.h"

//---PhysX Includes---//
#undef min
#undef max
#include "NxPhysics.h"
//---------------------//

using namespace Math;

bool CPhysxBone::Init(CalBone* _pBone, Math::Mat44f _vMat44, int _iColisionGroup)
{
    SetCalBone(_pBone);
    CalCoreBone* l_pCoreBone = m_pCalBone->getCoreBone();
    m_iParentID = l_pCoreBone->getParentId();
    m_vMatAnimatedModel = _vMat44;
    m_iCollisionGroup = _iColisionGroup;

    if (m_iParentID != -1)
    {
        m_bRoot = false;
    }

    // Afegir Child ID.
    std::list<int>::iterator iteratorChildId;
    for(iteratorChildId = l_pCoreBone->getListChildId().begin(); iteratorChildId != l_pCoreBone->getListChildId().end();
            ++iteratorChildId)
    {
        m_vChildListID.push_back(*iteratorChildId);
    }

    //SetOk(true);
    //return IsOk();
    return true;
}

void CPhysxBone::Release()
{
    CHECKED_DELETE(m_pActor);
    CHECKED_DELETE(m_pBoneUserData);
    m_vChildListID.clear();

}

Math::Mat44f CPhysxBone::GetBoneLeftHandedAbsoluteTransformation(CalBone* _pBone)
{
    //rotacio i translacio del bone (absoluta)
    CalVector l_vTranslation = _pBone->getTranslationAbsolute();
    CalQuaternion l_RotationQuaternion = _pBone->getRotationAbsolute();

    //passem el quaternion a left handed
    l_RotationQuaternion.x = -l_RotationQuaternion.x;
    l_RotationQuaternion.y = -l_RotationQuaternion.y;
    l_RotationQuaternion.z = -l_RotationQuaternion.z;

    //creem la matriu de transformacio Cal3d (absolute) -> Math::Mat44f
    CalMatrix l_RotationMatrix(l_RotationQuaternion);

    Mat33f l_Rotation = Mat33f( l_RotationMatrix.dxdx   , l_RotationMatrix.dydx  , l_RotationMatrix.dzdx,
                                l_RotationMatrix.dxdy   , l_RotationMatrix.dydy  , l_RotationMatrix.dzdy,
                                l_RotationMatrix.dxdz   , l_RotationMatrix.dydz  , l_RotationMatrix.dzdz).Get33RotationNormalized();

    float l_fAngleX = pi32 - l_Rotation.GetAngleX();
    float l_fAngleY = pi32 - l_Rotation.GetAngleY();
    float l_fAngleZ = pi32 - l_Rotation.GetAngleZ();

    Math::Mat44f l_Transform;
    l_Transform.SetIdentity();
    l_Transform.RotByAnglesYXZ(l_fAngleY, l_fAngleX, l_fAngleZ);
    l_Transform.Translate(Math::Vect3f(-l_vTranslation.x, l_vTranslation.y, l_vTranslation.z));

    return l_Transform;

}


//Funcions per afegir els actors de fisica.
bool CPhysxBone::AddBoxActor(CXMLTreeNode _XMLObjects, CObject3D* _pEntity)
{
    string l_szName;
    Math::Vect3f l_vSize, l_fMiddlePoint;
    float l_fDensity;

    l_szName                  = _XMLObjects.GetAttribute<std::string>("name" , "");
    l_fDensity            = _XMLObjects.GetAttribute<float>("density" ,0.0f );
    l_fMiddlePoint        = _XMLObjects.GetAttribute<Math::Vect3f>("bounding_box_middle_point", Math::Vect3f(0.0f) );
    l_vSize               = _XMLObjects.GetAttribute<Math::Vect3f>("bounding_box_size", Math::Vect3f(0.0f));

    CPhysicsManager* l_pPM = PhysXMInstance;
    Math::Mat44f l_vMatActor;
    l_vMatActor = GetBoneLeftHandedAbsoluteTransformation(m_pCalBone);
    m_vMiddlePoint = l_fMiddlePoint;

    Math::Mat44f l_mTotal = m_vMatAnimatedModel * l_vMatActor;

    CPhysicUserData* l_pUserData = new CPhysicUserData(l_szName);
    l_pUserData->SetPaint(true);
    l_pUserData->SetColor(colGREEN);
    l_pUserData->SetEntity(_pEntity);
    CPhysicActor* l_pActor = new CPhysicActor(l_pUserData);
    l_pActor->AddBoxShape(Math::Vect3f(l_vSize.x, l_vSize.z, l_vSize.y) * 0.5f, l_mTotal.GetPos(), l_fMiddlePoint, v3fZERO,
                          NULL, m_iCollisionGroup);
    l_pActor->CreateBody(l_fDensity, 1.0f, 1.0f);
    l_pPM->AddPhysicActor(l_pActor);
    l_pActor->SetActorSolverIterationCount(75);
    l_pActor->SetMat44(l_mTotal);
    //l_pActor->GetPhXActor()->raiseBodyFlag(NX_BF_KINEMATIC);
    //l_pActor->GetPhXActor()->putToSleep();

    m_pActor = l_pActor;
    m_pBoneUserData = l_pUserData;

    return true;
}

bool CPhysxBone::AddSphereActor(CXMLTreeNode _XMLObjects, CObject3D* _pEntity)
{
    string l_szName;
    Math::Vect3f l_vSize, l_fMiddlePoint;
    float l_fDensity;

    l_szName              = _XMLObjects.GetAttribute<std::string>("name" , "");
    l_fDensity            = _XMLObjects.GetAttribute<float>("density", 0.0f);
    l_fMiddlePoint        = _XMLObjects.GetAttribute<Math::Vect3f>("bounding_box_middle_point", Math::Vect3f(0.0f));
    l_vSize               = _XMLObjects.GetAttribute<Math::Vect3f>("bounding_box_size", Math::Vect3f(0.0f));

    CPhysicsManager* l_pPM = PhysXMInstance;
    Math::Mat44f l_vMatActor;
    l_vMatActor = GetBoneLeftHandedAbsoluteTransformation(m_pCalBone);
    m_vMiddlePoint = l_fMiddlePoint;
    CPhysicUserData* l_pUserData = new CPhysicUserData(l_szName);
    l_pUserData->SetPaint(true);
    l_pUserData->SetColor(colYELLOW);
    l_pUserData->SetEntity(_pEntity);
    CPhysicActor* l_pActor = new CPhysicActor(l_pUserData);
    l_pActor->AddSphereShape(l_vSize.x * 0.5f, m_vMatActor.GetPos(), Math::Vect3f(l_fMiddlePoint.x, 0.0f, 0.0f), NULL,
                             m_iCollisionGroup);
    l_pActor->CreateBody(l_fDensity, 1.0f);

    l_pPM->AddPhysicActor(l_pActor);
    l_pActor->SetActorSolverIterationCount(75);
    l_pActor->SetMat44(m_vMatAnimatedModel * l_vMatActor);

    //l_pActor->GetPhXActor()->putToSleep();

    m_pActor = l_pActor;
    m_pBoneUserData = l_pUserData;

    return true;
}


bool CPhysxBone::AddCapsuleActor(CXMLTreeNode _XMLObjects, CObject3D* _pEntity)
{

    return true;
}


void CPhysxBone::UpdateCal3dFromPhysx()
{

    if ((m_pActor != 0) && (m_pParent != 0))
    {
        if (m_pParent->GetPhysxActor() != 0)
        {
            Math::Mat44f l_vAbsoluteParent, l_vAbsolute;
            m_pParent->GetPhysxActor()->GetMat44(l_vAbsoluteParent);
            GetPhysxActor()->GetMat44(l_vAbsolute);
            Math::Mat44f l_vInverseParent = l_vAbsoluteParent.GetInverted();
            Math::Mat44f l_vRelative = l_vInverseParent * l_vAbsolute;

            Vect4f l_vTranslate = l_vRelative.GetTranslationVector();
            CalVector l_vRelativeTranslation(-l_vTranslate.x, l_vTranslate.y, l_vTranslate.z);

            Mat33f l_vMat33 = l_vRelative.Get33RotationNormalized();

            float l_fAngleX = pi32 - l_vMat33.GetAngleX();
            float l_fAngleY = pi32 - l_vMat33.GetAngleY();
            float l_fAngleZ = pi32 - l_vMat33.GetAngleZ();

            NxMat33 l_vMat33PhysxX;
            NxMat33 l_vMat33PhysxY;
            NxMat33 l_vMat33PhysxZ;
            NxMat33 l_vMat33Physx;

            l_vMat33PhysxX.rotX(l_fAngleX);
            l_vMat33PhysxY.rotY(l_fAngleY);
            l_vMat33PhysxZ.rotZ(l_fAngleZ);
            l_vMat33Physx = l_vMat33PhysxZ * l_vMat33PhysxX * l_vMat33PhysxY;


            NxQuat l_vQuat(l_vMat33Physx);
            CalQuaternion l_vQuatRelative;

            l_vQuatRelative.x = -l_vQuat.x;
            l_vQuatRelative.y = -l_vQuat.y;
            l_vQuatRelative.z = -l_vQuat.z;
            l_vQuatRelative.w = l_vQuat.w;

            m_pCalBone->setRotation(l_vQuatRelative);
            m_pCalBone->setTranslation(l_vRelativeTranslation);
        }
        else
        {
            Math::Mat44f l_vAbsolute;
            GetPhysxActor()->GetMat44(l_vAbsolute);
            Math::Mat44f l_vRelative = l_vAbsolute;

            Math::Mat44f l_matRelativeTransformation;
            l_matRelativeTransformation = GetBoneLeftHandedAbsoluteTransformation(m_pCalBone).Invert();

            m_vMatActor.SetIdentity();
            m_vMatActor *= l_vAbsolute;
            m_vMatActor *= l_matRelativeTransformation;

        }
    }

}

void CPhysxBone::UpdatePhysxFromCal3d(const Math::Mat44f& _mTransform)
{
    if (m_pActor != 0)
    {
        m_pActor->MoveGlobalPoseMat44(_mTransform * GetBoneLeftHandedAbsoluteTransformation(m_pCalBone));
    }
}

void CPhysxBone::SetTransformAfterUpdate(const Math::Mat44f& _mTransform)
{
    m_vMatActor = _mTransform;

    if (m_pActor != 0)
    {
        m_pActor->SetMat44(m_vMatActor * GetBoneLeftHandedAbsoluteTransformation(m_pCalBone));
    }
}

