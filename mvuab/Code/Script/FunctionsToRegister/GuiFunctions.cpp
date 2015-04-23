#include "FunctionsToRegister\GuiFunctions.h"
#include "GUIManager.h"
#include "luabind_macros.h"
#include "Utils/Defines.h"
#include <assert.h>


using namespace luabind;

void registerGUI( lua_State* aLuaState )
{
	ASSERT( aLuaState, "LuaState error in Register Cameras" );
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // WWSOUND MANAGER
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  LUA_BEGIN_DECLARATION( aLuaState )
  LUA_DECLARE_CLASS( CGUIManager )
  LUA_DECLARE_METHOD( CGUIManager, ActiveWindows )
  LUA_DECLARE_METHOD( CGUIManager, SetMessageBox )

  LUA_END_DECLARATION
}
