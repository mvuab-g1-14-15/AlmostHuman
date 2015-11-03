#ifndef _WAIT_TIME_ELEMENT_
#define _WAIT_TIME_ELEMENT_

#include "Cinematics\CinematicsElement\CinematicsElement.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CWaitTimeElement : public CCinematicsElement
{
    public:
      CWaitTimeElement(CXMLTreeNode& atts);
      virtual void Update();
      virtual void Execute(CGraphicsManager& GM);
		  virtual void Render();
		  virtual void Render2D(CGraphicsManager &GM);     

private:
	std::string			m_LuaCode;
	bool				m_bLua;
	float				m_KeyAction;

};

#endif //_WAIT_TIME_ELEMENT_