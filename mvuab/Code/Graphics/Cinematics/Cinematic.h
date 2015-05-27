#ifndef CINEMATIC_H
#define CINEMATIC_H

#include "RenderableObject/RenderableObject.h"
#include "CinematicPlayer.h"

#include <vector>
#include <string>

class CCinematicObject;

class CCinematic : public CRenderableObject, public CCinematicPlayer
{
	protected:
		std::vector<CCinematicObject *> m_CinematicObjects;

	public:
		CCinematic(const std::string &Filename);
		virtual ~CCinematic();

		virtual void Stop();
		virtual void Play(bool Cycle);
		virtual void Pause();

		void LoadXML(const std::string &Filename);
		void AddCinematicObject(CCinematicObject *CinematicObject);

		virtual void Update();
		virtual void Render();
};

#endif // CINEMATIC_H
