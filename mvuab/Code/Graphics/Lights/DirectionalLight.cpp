#include "DirectionalLight.h"
#include "Math\Vector3.h"

#include "XML\XMLTreeNode.h"

CDirectionalLight::CDirectionalLight(CXMLTreeNode node) : CLight()
{
    m_Direction = node.GetVect3fProperty("dir",Math::Vect3f(0,0,0));
    m_Name = node.GetPszProperty("name","");
    m_Color = Math::colWHITE;
    m_Position = node.GetVect3fProperty("pos",Math::Vect3f(0,0,0));
    m_Intensity = node.GetFloatProperty("intensity", 0);
    m_fYaw = node.GetFloatProperty("yaw", 0);
    m_fPitch = node.GetFloatProperty("pitch", 0);
    m_fRoll = node.GetFloatProperty("roll", 0);
    m_Scale = node.GetVect3fProperty("scale",Math::Vect3f(0,0,0));
    m_StartRangeAttenuation = node.GetFloatProperty("att_start_range", 0);
    m_EndRangeAttenuation = node.GetFloatProperty("att_end_range", 0);
	SetType(CLight::DIRECTIONAL);
}

void CDirectionalLight::SetDirection(const Math::Vect3f &Direction)
{
	m_Direction = Direction;
}

Math::Vect3f CDirectionalLight::GetDirection() const
{
	return m_Direction;
}

void CDirectionalLight::Render()
{
}