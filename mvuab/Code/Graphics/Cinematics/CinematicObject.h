#ifndef CINEMATIC_OBJECT_H
#define CINEMATIC_OBJECT_H

#include "CinematicPlayer.h"

#include <vector>

class CCinematicObjectKeyFrame;
class CInstanceMesh;
class CXMLTreeNode;
class CPhysicActor;

class CCinematicObject : public CCinematicPlayer
{
private:
	std::vector<CCinematicObjectKeyFrame *> m_CinematicObjectKeyFrames;
	size_t m_CurrentKeyFrame;
	CInstanceMesh *m_RenderableObject;
	CPhysicActor *m_Actor;
	bool m_PlayerUpdate;
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
