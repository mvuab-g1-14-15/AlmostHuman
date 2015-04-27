#include "FunctionsToRegister\GuiFunctions.h"
#include "GUIManager.h"
#include "luabind_macros.h"
#include "Utils/Defines.h"
#include "Window/Windows.h"
#include "GuiElement/GuiElement.h"
#include "Widgets/ProgressBar.h"

#include <assert.h>


using namespace luabind;

void registerGUI( lua_State* aLuaState )
{
	ASSERT( aLuaState, "LuaState error in Register GUI" );
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // GUI MANAGER
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  LUA_BEGIN_DECLARATION( aLuaState )
	  LUA_DECLARE_CLASS( CGUIManager )
	  LUA_DECLARE_METHOD( CGUIManager, ActiveWindows )
	  LUA_DECLARE_METHOD( CGUIManager, SetMessageBox )
	  LUA_DECLARE_METHOD( CGUIManager, GetWindow )
  LUA_END_DECLARATION

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // GUI ELEMENT
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  LUA_BEGIN_DECLARATION( aLuaState )
	  LUA_DECLARE_CLASS( CGuiElement )
	  LUA_DECLARE_METHOD( CGuiElement, SetPositionPercent )
	  LUA_DECLARE_METHOD( CGuiElement, SetHeightPercent )
	  LUA_DECLARE_METHOD( CGuiElement, SetWidthPercent )
	  LUA_DECLARE_METHOD( CGuiElement, SetProgress )
  LUA_END_DECLARATION

  LUA_BEGIN_DECLARATION( aLuaState )
	  LUA_DECLARE_CLASS( CWindows )
	  LUA_DECLARE_METHOD( CWindows, GetElement )
  LUA_END_DECLARATION
}
