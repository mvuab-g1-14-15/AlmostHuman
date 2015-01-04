#include "CinematicObject.h"
#include "XML/XMLTreeNode.h"
#include "CinematicObjectKeyFrame.h"
#include "Core.h"
#include "RenderableObject/RenderableObjectsManager.h"
#include "GraphicsManager.h"

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
			m_CurrentKeyFrame = i;
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
	m_RenderableObject->SetPosition(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetPosition());
	m_RenderableObject->SetYaw(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetRoll());
	m_RenderableObject->SetPitch(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetPitch());
	m_RenderableObject->SetRoll(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetRoll());
	m_RenderableObject->SetScale(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetScale());
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