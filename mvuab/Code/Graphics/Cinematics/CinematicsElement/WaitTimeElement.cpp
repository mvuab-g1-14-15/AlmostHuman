#include "Cinematics\CinematicsElement\WaitTimeElement.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Timer\Timer.h"
#include "ScriptManager.h"

CWaitTimeElement::CWaitTimeElement( CXMLTreeNode& atts ) : CCinematicsElement( atts ),
m_bLua( false )
{
	m_LuaCode = atts.GetAttribute<std::string>("lua_code", "");
	m_KeyAction = atts.GetAttribute<float>("key_action", 0.0f);
	if(m_LuaCode != "" )
		m_bLua = true;
}

void CWaitTimeElement::Update()
{
  m_CurrentTime += constFrameTime;
  if( m_CurrentTime > m_KeyAction && m_bLua)
  {
	  ScriptMInstance->RunCode(m_LuaCode);
	  m_bLua = false;
  }
  if( m_CurrentTime >= m_Time )
  {
    m_IsBlocker = false;
  }
}

void CWaitTimeElement::Execute( CGraphicsManager& GM )
{
  
}

void CWaitTimeElement::Render()
{
}

void CWaitTimeElement::Render2D(CGraphicsManager &GM)
{
}