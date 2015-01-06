#include "CinematicObject.h"
#include "XML/XMLTreeNode.h"
#include "CinematicObjectKeyFrame.h"
#include "Core.h"
#include "RenderableObject/RenderableObjectsManager.h"
#include "GraphicsManager.h"
#include "Math\LerpAnimator3D.h"
#include "Math\LerpAnimator1D.h"
#include "Math\MathTypes.h"
#include "Math\Vector3.h"
#include "Utils\BaseUtils.h"

CCinematicObject::CCinematicObject(CXMLTreeNode &atts)
{
	std::string resource = atts.GetPszProperty("resource", "");

	m_RenderableObject = CCore::GetSingletonPtr()->GetRenderableObjectsManager()->GetResource(resource);

	for(int i=0;i<atts.GetNumChildren();++i)
		m_CinematicObjectKeyFrames.push_back(new CCinematicObjectKeyFrame(atts(i)));
}

CCinematicObject::~CCinematicObject()
{
    for(std::vector<CCinematicObjectKeyFrame *>::iterator it = m_CinematicObjectKeyFrames.begin(); it != m_CinematicObjectKeyFrames.end(); ++it)
    {
         CHECKED_DELETE((*it));
    }

    m_CinematicObjectKeyFrames.clear();
}

bool CCinematicObject::IsOk()
{
	return true;
}

void CCinematicObject::AddCinematicObjectKeyFrame(CCinematicObjectKeyFrame *CinematicObjectKeyFrame)
{
	m_CinematicObjectKeyFrames.push_back(CinematicObjectKeyFrame);
}

void CCinematicObject::Update(float ElapsedTime)
{
    bool KeyFrameChanged(false);
	CCinematicPlayer::Update(ElapsedTime);
	for(int i=0; i<m_CinematicObjectKeyFrames.size(); ++i)
	{
		if(m_CinematicObjectKeyFrames[i]->GetKeyFrameTime() > m_CurrentTime)
		{
            float l_PrevKeyFrame = m_CurrentKeyFrame;
			m_CurrentKeyFrame = i-1;
            if( l_PrevKeyFrame < m_CurrentKeyFrame )
            {
                KeyFrameChanged = true;
                //baseUtils::Trace("KeyFrameChanged - Next=>%d\n", m_CurrentKeyFrame);
                //baseUtils::Trace("KeyFrameChanged - Prev=>%d\n", l_PrevKeyFrame);
            }
            
			break;
		}
	}
	if(m_CurrentTime >= m_Duration)
	{
		if(m_Cycle)
		{
			OnRestartCycle();
		}
		else
		{
			m_CurrentKeyFrame = m_CinematicObjectKeyFrames.size()-2;
            m_CurrentTime = m_Duration;
			Pause();
		}
	}

    size_t l_NextKeyFrame = m_CurrentKeyFrame + 1;

	float l_CurrentP = (m_CurrentTime-m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetKeyFrameTime())/(m_CinematicObjectKeyFrames[l_NextKeyFrame]->GetKeyFrameTime()-m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetKeyFrameTime());
    
    //baseUtils::Trace("Percentage=>%f\nCurrent Time=>%f\n", l_CurrentP, m_CurrentTime);

	Math::CLerpAnimator3D lerp_animator_3D;
	lerp_animator_3D.SetValues(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetPosition(), m_CinematicObjectKeyFrames[l_NextKeyFrame]->GetPosition(),1.0f,Math::FUNC_CONSTANT);
	Math::Vect3f pos;
	lerp_animator_3D.Update(l_CurrentP, pos);

    //baseUtils::Trace("Pos=>%f-%f-%f\n", pos.x, pos.y,pos.z);

	Math::CLerpAnimator1D lerp_animator_1D;
	lerp_animator_1D.SetValues(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetYaw(), m_CinematicObjectKeyFrames[l_NextKeyFrame]->GetYaw(),1.0f,Math::FUNC_CONSTANT);
	float yaw;
	lerp_animator_1D.Update(l_CurrentP, yaw);
    baseUtils::Trace("yaw=>%f\n",yaw);

    //Math::Vect3f actual(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetYaw(), 0.0, 0.0);
    //Math::Vect3f next(m_CinematicObjectKeyFrames[l_NextKeyFrame]->GetYaw(), 0.0, 0.0);
    //lerp_animator_3D.SetValues(actual, next,1.0f,Math::FUNC_CONSTANT);
    //Math::Vect3f yaw_vector;
    //lerp_animator_3D.Update(l_CurrentP, yaw_vector);
    //float yaw = yaw_vector.x;

    if(KeyFrameChanged)
    {
        //baseUtils::Trace("Current Time=>%f\n", m_CurrentTime);
//         baseUtils::Trace("YawPrev=>%f\n",m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetYaw());
//         baseUtils::Trace("YawNext=>%f\n",m_CinematicObjectKeyFrames[l_NextKeyFrame]->GetYaw());
//         baseUtils::Trace("KeyFrameChanged - Next=>%d\n", l_NextKeyFrame);
//         baseUtils::Trace("KeyFrameChanged - Prev=>%d\n", m_CurrentKeyFrame);
    }

	lerp_animator_1D.SetValues(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetPitch(), m_CinematicObjectKeyFrames[l_NextKeyFrame]->GetPitch(),1.0f,Math::FUNC_CONSTANT);
	float pitch;
	lerp_animator_1D.Update(l_CurrentP, pitch);

    //baseUtils::Trace("pitch=>%f\n", pitch);

	lerp_animator_1D.SetValues(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetRoll(), m_CinematicObjectKeyFrames[l_NextKeyFrame]->GetRoll(),1.0f,Math::FUNC_CONSTANT);
	float roll;
	lerp_animator_1D.Update(l_CurrentP, roll);

    //baseUtils::Trace("roll=>%f\n", roll);
	
	lerp_animator_3D.SetValues(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetScale(), m_CinematicObjectKeyFrames[l_NextKeyFrame]->GetScale(),1.0f,Math::FUNC_CONSTANT);
	Math::Vect3f scale;
	lerp_animator_3D.Update(l_CurrentP, scale);

    //baseUtils::Trace("Scale=>%f-%f-%f\n", scale.x, scale.y,scale.z);

	m_RenderableObject->SetPosition(pos);
	m_RenderableObject->SetYaw(yaw);
	m_RenderableObject->SetPitch(pitch);
	m_RenderableObject->SetRoll(roll);
	m_RenderableObject->SetScale(scale);
    baseUtils::Trace("KeyFrame:=>%d\n", m_CurrentKeyFrame);
}

void CCinematicObject::OnRestartCycle()
{
	m_CurrentKeyFrame = 0;
	m_CurrentTime = 0.0;
	m_RenderableObject->SetPosition(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetPosition());
	m_RenderableObject->SetYaw(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetRoll());
	m_RenderableObject->SetPitch(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetPitch());
	m_RenderableObject->SetRoll(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetRoll());
	m_RenderableObject->SetScale(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetScale());
}

void CCinematicObject::Render()
{
	m_RenderableObject->Render();
}

void CCinematicObject::Stop()
{
	m_CurrentKeyFrame = 0;
}