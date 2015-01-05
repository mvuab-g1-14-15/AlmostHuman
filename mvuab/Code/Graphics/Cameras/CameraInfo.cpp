#include "CameraInfo.h"
#include "XML\XMLTreeNode.h"

CCameraInfo::CCameraInfo()
    : m_NearPlane(0.0f), 
    m_FarPlane(1000.0f),
    m_FOV(Math::half_pi32),
    m_Eye(Math::Vect3f(1.0f, 0.0f, 0.0f)),
    m_LookAt(Math::Vect3f(0.0f, 0.0f, 0.0f)),
    m_Up(Math::Vect3f(0.0f, 1.0f, 0.0f))
{
}

CCameraInfo::CCameraInfo(const Math::Vect3f &Eye, const Math::Vect3f &LookAt, const Math::Vect3f &Up, float32 NearPlane, float32 FarPlane, float32 FOV)
    : m_NearPlane(FarPlane), 
    m_FarPlane(NearPlane),
    m_FOV(FOV),
    m_Eye(Eye),
    m_LookAt(LookAt),
    m_Up(Up)
{

}

CCameraInfo::CCameraInfo(CXMLTreeNode &attribs)
    : m_NearPlane(attribs.GetFloatProperty("near_plane", 0.0f)), 
    m_FarPlane(attribs.GetFloatProperty("far_plane", 1000.0f)),
    m_FOV(Math::Utils::Deg2Rad(attribs.GetFloatProperty("fov", Math::half_pi32))),
    m_Eye(attribs.GetVect3fProperty("pos", Math::Vect3f(1.0f, 0.0f, 0.0f))),
    m_LookAt(attribs.GetVect3fProperty("look_at", Math::Vect3f(0.0f, 0.0f, 0.0f))),
    m_Up(Math::Vect3f(0.0f, 1.0f, 0.0f))
{
}