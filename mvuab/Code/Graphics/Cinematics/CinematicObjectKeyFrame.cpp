#include "CinematicObjectKeyFrame.h"
#include "XML\XMLTreeNode.h"
#include "Math\MathUtils.h"

CCinematicObjectKeyFrame::CCinematicObjectKeyFrame(CXMLTreeNode &atts)
{
    m_KeyFrameTime = atts.GetAttribute<float>("time", 0.0f);
    m_Position = atts.GetAttribute<Math::Vect3f>("pos", Math::Vect3f(0, 0, 0));
    m_fYaw = /*Math::Utils::Deg2Rad(*/ atts.GetAttribute<float>("yaw", 0.0f)/* )*/;
    m_fPitch = /*Math::Utils::Deg2Rad(*/ atts.GetAttribute<float>("pitch", 0.0f)/* )*/;
    m_fRoll = /*Math::Utils::Deg2Rad(*/ atts.GetAttribute<float>("roll", 0.0f) /*)*/;
    m_Scale = atts.GetAttribute<Math::Vect3f>("scale", Math::Vect3f(1, 1, 1));
}