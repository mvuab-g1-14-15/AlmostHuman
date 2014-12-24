#ifndef CINEMATIC_OBJECT_H
#define CINEMATIC_OBJECT_H

#include "CinematicPlayer.h"

#include <vector>

class CCinematicObjectKeyFrame;
class CRenderableObject;
class CXMLTreeNode;

class CCinematicObject : public CCinematicPlayer
{
private:
	std::vector<CCinematicObjectKeyFrame *> m_CinematicObjectKeyFrames;
	size_t m_CurrentKeyFrame;
	CRenderableObject *m_RenderableObject;
public:
	CCinematicObject(CXMLTreeNode &atts);
	bool IsOk();
	virtual ~CCinematicObject();
	void AddCinematicObjectKeyFrame(CCinematicObjectKeyFrame *CinematicObjectKeyFrame);
	void Update(float ElapsedTime);
	void Stop();
	void OnRestartCycle();
};

#endif // CINEMATIC_OBJECT_H