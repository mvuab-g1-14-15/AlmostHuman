#ifndef CINEMATIC_OBJECT_KEYFRAME_H
#define CINEMATIC_OBJECT_KEYFRAME_H

#include "Utils/Defines.h"
#include "Object3D.h"

class CXMLTreeNode;

class CCinematicObjectKeyFrame : public CObject3D
{
private:
	float m_KeyFrameTime;
public:
	CCinematicObjectKeyFrame(CXMLTreeNode &atts);
	GET_SET(float, KeyFrameTime);
};


#endif // CINEMATIC_OBJECT_KEYFRAME_H
