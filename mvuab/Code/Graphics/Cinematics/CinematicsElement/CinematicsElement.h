#ifndef CINEMATICS_ELEMENT_
#define CINEMATICS_ELEMENT_
#pragma once


#include "Utils\Name.h"
#include "Utils\Visible.h"
#include "XML\XMLTreeNode.h"
#include "Utils/Types.h"

class CGraphicsManager;

class CCinematicsElement: public CVisible, public CName
{
public:
  CCinematicsElement( CXMLTreeNode& atts );
  virtual ~CCinematicsElement();

  virtual void Update() = 0;
  virtual void Execute( CGraphicsManager& GM ) = 0;
  virtual void Render() = 0;
  virtual void Render2D( CGraphicsManager& GM ) = 0;

  GET_SET(bool, IsBlocker);
  GET_SET(float32, Time);
  GET_SET(float32, CurrentTime);
protected:
  bool m_IsBlocker;
  float32 m_Time;
  float32 m_CurrentTime;
};
#endif //CINEMATICS_ELEMENT_