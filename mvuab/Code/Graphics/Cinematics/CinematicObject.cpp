#include "CinematicObject.h"
#include "XML/XMLTreeNode.h"
#include "CinematicObjectKeyFrame.h"
#include "Core.h"
#include "RenderableObject/RenderableObjectsManager.h"

CCinematicObject::CCinematicObject(CXMLTreeNode &atts)
{
	std::string resource = atts.GetPszProperty("resource", "");

	m_RenderableObject = CCore::GetSingletonPtr()->GetRenderableObjectsManager()->GetResource(resource);

	for(int i=0;i<atts.GetNumChildren();++i)
		m_CinematicObjectKeyFrames.push_back(new CCinematicObjectKeyFrame(atts(i)));
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
	m_CurrentKeyFrame = (int)(m_Duration/m_CurrentTime);
	m_RenderableObject->SetPosition(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetPosition());
	m_RenderableObject->SetYaw(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetRoll());
	m_RenderableObject->SetPitch(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetPitch());
	m_RenderableObject->SetRoll(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetRoll());
	m_RenderableObject->SetScale(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetScale());
}

void CCinematicObject::OnRestartCycle()
{
	m_CurrentKeyFrame = 0;
	m_RenderableObject->SetPosition(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetPosition());
	m_RenderableObject->SetYaw(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetRoll());
	m_RenderableObject->SetPitch(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetPitch());
	m_RenderableObject->SetRoll(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetRoll());
	m_RenderableObject->SetScale(m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetScale());
}