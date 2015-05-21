#include "FunctionsToRegister\GuiFunctions.h"
#include "GUIManager.h"
#include "luabind_macros.h"
#include "Utils/Defines.h"
#include "Window/Windows.h"
#include "GuiElement/GuiElement.h"
#include "Widgets/ProgressBar.h"
#include "Widgets\ConsoleGUI.h"
#include "Widgets/Map.h"
#include "Widgets\EditableTextBox.h"

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
	  LUA_DECLARE_METHOD( CGUIManager, GetConsole )
	  LUA_DECLARE_METHOD( CGUIManager, SetRenderPointer )
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

  //CWindows
  LUA_BEGIN_DECLARATION( aLuaState )
	  LUA_DECLARE_CLASS( CWindows )
	  LUA_DECLARE_METHOD( CWindows, GetElement )
  LUA_END_DECLARATION

  //CMap
  LUA_BEGIN_DECLARATION( aLuaState )
	  LUA_DECLARE_DERIVED_CLASS( CMap, CGuiElement )
	  LUA_DECLARE_METHOD( CMap, SetPositionPlayer )
	  LUA_DECLARE_METHOD( CMap, SetYawPlayer )
	  LUA_DECLARE_METHOD( CMap, SetPositionEnemy )
	  LUA_DECLARE_METHOD( CMap, SetYawEnemy )
  LUA_END_DECLARATION  

  //CEditableTextBox
  LUA_BEGIN_DECLARATION( aLuaState )
	  LUA_DECLARE_DERIVED_CLASS( CEditableTextBox, CGuiElement )
  LUA_END_DECLARATION  

  //CConsoleGUI
  LUA_BEGIN_DECLARATION( aLuaState )
	  LUA_DECLARE_DERIVED_CLASS( CConsoleGUI, CEditableTextBox )
	  LUA_DECLARE_METHOD( CConsoleGUI, GetVisible )
  LUA_END_DECLARATION

}
