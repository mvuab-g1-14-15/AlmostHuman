#pragma once
#ifndef _PHYSX_OBJ_DEFS_H
#define _PHYSX_OBJ_DEFS_H

#include <string>
#include "Math\Vector3.h"
#include "Object3D.h"
#include "Utils\Types.h"
#include "Utils\Name.h"
#include "XML/XMLTreeNode.h"
#include "EngineManagers.h"
#include "PhysicsManager.h"

#define PHYSX_OBJ_BOX   0x0001

using namespace Math;

class CPhysXObj : public CObject3D, public CName
{
    public:
        CPhysXObj(const CXMLTreeNode& aNode )
            : CObject3D( aNode )
            , m_Type(aNode.GetAttribute<int32>("type", 0))
            , m_Group(0)
            , m_Dimensions(aNode.GetAttribute<Vect3f>("dimension", Math::Vect3f()))
        {
            SetName(aNode.GetAttribute<std::string>("name", "no_name") );
            const std::string& lGroupName = aNode.GetAttribute<std::string>("group", "ECG_ESCENE");
            m_Group = PhysXMInstance->GetCollisionGroup(lGroupName);
        }
        GET_SET_REF( int32, Type );
        GET_SET_REF( uint32, Group );
        GET_SET_REF( Vect3f, Dimensions );

    protected:
        Vect3f m_Dimensions;
        int32   m_Type;
        uint32  m_Group;
};

#endif