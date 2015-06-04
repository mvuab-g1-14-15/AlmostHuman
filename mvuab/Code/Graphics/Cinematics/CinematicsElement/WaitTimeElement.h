#ifndef _WAIT_TIME_ELEMENT_
#define _WAIT_TIME_ELEMENT_

#include "Cinematics\CinematicsElement\CinematicsElement.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CWaitTimeElement : public CCinematicsElement
{
    public:
      CWaitTimeElement(CXMLTreeNode& atts);
      virtual void Execute(CGraphicsManager& GM);
		  virtual void Render(CGraphicsManager &GM);
		  virtual void Render2D(CGraphicsManager &GM);
    private:
      float		m_Time;
};

#endif //_WAIT_TIME_ELEMENT_