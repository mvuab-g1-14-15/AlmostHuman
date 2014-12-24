#ifndef CINEMATIC_H
#define CINEMATIC_H

#include "RenderableObject/RenderableObject.h"
#include "CinematicPlayer.h"

#include <vector>
#include <string>

class CCinematicObject;
class CXMLTreeNode;

class CCinematic : public CRenderableObject, public CCinematicPlayer
{
protected:
	std::vector<CCinematicObject *> m_CinematicObjects;
public:
	CCinematic(CXMLTreeNode &atts);
	virtual ~CCinematic();
	virtual void Stop();
	virtual void Play(bool Cycle);
	virtual void Pause();
	void LoadXML(const std::string &Filename);
	void AddCinematicObject(CCinematicObject *CinematicObject);
	void Update(float ElapsedTime);
	virtual void Render(CGraphicsManager* gm);
};

#endif // CINEMATIC_H
