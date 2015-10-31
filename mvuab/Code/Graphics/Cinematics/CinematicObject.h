#ifndef CINEMATIC_OBJECT_H
#define CINEMATIC_OBJECT_H

#include "CinematicPlayer.h"
#include "Utils\Types.h"

#include <vector>

class CCinematicObjectKeyFrame;
class CRenderableObject;
class CXMLTreeNode;
class CPhysicActor;

class CCinematicObject : public CCinematicPlayer
{
private:
	std::vector<CCinematicObjectKeyFrame *> m_CinematicObjectKeyFrames;
	size_t									m_CurrentKeyFrame;
	CRenderableObject*						m_RenderableObject;
	CPhysicActor*							m_Actor;
	bool									m_PlayerUpdate;
	bool									m_bLuaEnable;
	float32									m_KeyAction;
	std::string								m_LuaCode;

public:
	CCinematicObject(CXMLTreeNode &atts);
	bool IsOk();
	virtual ~CCinematicObject();
	void AddCinematicObjectKeyFrame(CCinematicObjectKeyFrame *CinematicObjectKeyFrame);
	void Update();
	void Stop();
	void OnRestartCycle();
	void Render();
};


#endif // CINEMATIC_OBJECT_H
