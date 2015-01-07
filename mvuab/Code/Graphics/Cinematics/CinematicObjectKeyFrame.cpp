#include "CinematicObjectKeyFrame.h"
#include "XML\XMLTreeNode.h"
#include "Math\MathUtils.h"

CCinematicObjectKeyFrame::CCinematicObjectKeyFrame(CXMLTreeNode &atts)
{
	m_KeyFrameTime = atts.GetFloatProperty("time", 0.0f);
	m_Position = atts.GetVect3fProperty("pos", Math::Vect3f(0,0,0));
	m_fYaw = Math::Utils::Deg2Rad( atts.GetFloatProperty("yaw", 0.0f) );
	m_fPitch = Math::Utils::Deg2Rad( atts.GetFloatProperty("pitch", 0.0f) );
	m_fRoll = Math::Utils::Deg2Rad( atts.GetFloatProperty("roll", 0.0f) );
	m_Scale = atts.GetVect3fProperty("scale", Math::Vect3f(1,1,1));
}