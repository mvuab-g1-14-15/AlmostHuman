#ifndef INC_CAMERA_INFO_H_
#define INC_CAMERA_INFO_H_

#include "Utils\Defines.h"
#include "Utils\Types.h"
#include "Math\Vector3.h"
#include "XML\XMLTreeNode.h"

class CCameraInfo
{
    public:
        CCameraInfo();
        CCameraInfo(const Math::Vect3f &Eye, const Math::Vect3f &LookAt, const Math::Vect3f &Up, float32 NearPlane,
                    float32 FarPlane, float32 FOV);
        CCameraInfo(const CXMLTreeNode &attribs);

        GET_SET( float32, NearPlane )
        GET_SET( float32, FarPlane )
        GET_SET( float32, FOV )
        GET_SET_REF( Math::Vect3f, Eye )
        GET_SET_REF( Math::Vect3f, LookAt )
        GET_SET_REF( Math::Vect3f, Up )

    protected: // Methods
        float32             m_NearPlane, m_FarPlane;
        float32             m_FOV;
        Math::Vect3f        m_Eye;
        Math::Vect3f        m_LookAt;
        Math::Vect3f        m_Up;
};

#endif // INC_CAMERA_INFO_H_