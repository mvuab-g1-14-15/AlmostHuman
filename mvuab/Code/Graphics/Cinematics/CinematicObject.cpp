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

CCinematicObject::CCinematicObject(CXMLTreeNode &atts)
{
	std::string resource = atts.GetPszProperty("resource", "");

	m_RenderableObject = CCore::GetSingletonPtr()->GetRenderableObjectsManager()->GetResource(resource);

	for(int i=0;i<atts.GetNumChildren();++i)
		m_CinematicObjectKeyFrames.push_back(new CCinematicObjectKeyFrame(atts(i)));
}

CCinematicObject::~CCinematicObject()
{
	//std::vector<CCinematicObjectKeyFrame *>::iterator it = m_CinematicObjectKeyFrames.begin();
	//std::vector<CCinematicObjectKeyFrame *>::iterator it_end = m_CinematicObjectKeyFrames.end();

	//for(; it != it_end; ++it)
	//	CHECKED_DELETE(*it);
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
	CCinematicPlayer::Update(ElapsedTime);
	for(int i=0; i<m_CinematicObjectKeyFrames.size(); ++i)
	{
		if(m_CinematicObjectKeyFrames[i]->GetKeyFrameTime() > m_CurrentTime)
		{
			m_CurrentKeyFrame = i-1;
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
			m_CurrentKeyFrame = m_CinematicObjectKeyFrames.size()-1;
			Pause();
		}
	}

	float l_CurrentP = (m_CurrentTime-m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetKeyFrameTime())/(m_CinematicObjectKeyFrames[m_CurrentKeyFrame+1]->GetKeyFrameTime()-m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetKeyFrameTime());

	Math::CLerpAnimator3D lerp_animator_3D;
	lerp_animator_3D.SetValues(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetPosition(), m_CinematicObjectKeyFrames[m_CurrentKeyFrame+1]->GetPosition(),1.0f,Math::FUNC_CONSTANT);
	Math::Vect3f pos;
	lerp_animator_3D.Update(l_CurrentP, pos);

	Math::CLerpAnimator1D lerp_animator_1D;
	lerp_animator_1D.SetValues(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetYaw(), m_CinematicObjectKeyFrames[m_CurrentKeyFrame+1]->GetYaw(),1.0f,Math::FUNC_CONSTANT);
	float yaw;
	lerp_animator_1D.Update(l_CurrentP, yaw);

	lerp_animator_1D.SetValues(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetPitch(), m_CinematicObjectKeyFrames[m_CurrentKeyFrame+1]->GetPitch(),1.0f,Math::FUNC_CONSTANT);
	float pitch;
	lerp_animator_1D.Update(l_CurrentP, pitch);

	lerp_animator_1D.SetValues(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetRoll(), m_CinematicObjectKeyFrames[m_CurrentKeyFrame+1]->GetRoll(),1.0f,Math::FUNC_CONSTANT);
	float roll;
	lerp_animator_1D.Update(l_CurrentP, roll);
	
	lerp_animator_3D.SetValues(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetScale(), m_CinematicObjectKeyFrames[m_CurrentKeyFrame+1]->GetScale(),1.0f,Math::FUNC_CONSTANT);
	Math::Vect3f scale;
	lerp_animator_3D.Update(l_CurrentP, scale);

	m_RenderableObject->SetPosition(pos);
	m_RenderableObject->SetYaw(yaw);
	m_RenderableObject->SetPitch(pitch);
	m_RenderableObject->SetRoll(roll);
	m_RenderableObject->SetScale(scale);
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