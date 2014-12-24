#include "CinematicObject.h"
#include "XML/XMLTreeNode.h"
#include "CinematicObjectKeyFrame.h"

CCinematicObject::CCinematicObject(CXMLTreeNode &atts)
{
	for(int i=0;i<atts.GetNumChildren();++i)
		m_CinematicObjectKeyFrames.push_back(new CCinematicObjectKeyFrame(atts(i)));
}