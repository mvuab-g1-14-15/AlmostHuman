#include "ActionManager.h"
#include "XML\XMLTreeNode.h"

#include "EngineConfig.h"
#include "EngineManagers.h"

#include <Windows.h>

CActionManager::CActionManager(): CManager()
{
}

CActionManager::CActionManager( CXMLTreeNode& atts )
  : CManager( atts )
{
}

CActionManager::~CActionManager()
{
  Destroy();
}

void CActionManager::Destroy()
{
  mActions.clear();
}
void CActionManager::Init( )
{
  mInputManager = InputManagerInstance;
  LoadXML();
}

bool CActionManager::DoAction( const std::string& action )
{
  float32 amount = 0;
  return ( DoAction( action, amount ) );
}

void CActionManager::ProcessInputs()
{
  BROFILER_CATEGORY( "CActionManager::ProcessInputs()", Profiler::Color::Orchid )
  mDoActions.clear();

  MapActions::const_iterator lItb = mActions.begin(), lIte = mActions.end();

  for ( ; lItb != lIte; ++lItb )
  {
    bool doIt = true;
    TInputsVector::const_iterator lItb_inputs = lItb->second.begin(), lIte_inputs = lItb->second.end();
    float32 amount = 0.0f;

    EInputDeviceType lDeviceType = IDV_KEYBOARD;

    for ( ; lItb_inputs != lIte_inputs; ++lItb_inputs )
    {

      SInputActions current_action = *lItb_inputs;

      if ( current_action.m_AxisType != AXIS_NOTHING )
      {
        switch ( current_action.m_AxisType )
        {
        case AXIS_MOUSE_X:
        {
          Math::Vect3i deltas = mInputManager->GetMouseDelta();
          amount = ( float )deltas.x * current_action.m_fDelta;
        }
        break;

        case AXIS_MOUSE_Y:
        {
          Math::Vect3i deltas = mInputManager->GetMouseDelta();
          amount = ( float )deltas.y * current_action.m_fDelta;
        }
        break;

        case AXIS_MOUSE_Z:
        {
          Math::Vect3i deltas = mInputManager->GetMouseDelta();
          amount = ( float )deltas.z * current_action.m_fDelta;
        }
        break;

        case AXIS_LEFT_THUMB_X:
        {
          float x, y;
          mInputManager->GetGamePadLeftThumbDeflection( &x, &y, current_action.m_DeviceType );
          amount = x * current_action.m_fDelta;
        }
        break;

        case AXIS_LEFT_THUMB_Y:
        {
          float x, y;
          mInputManager->GetGamePadLeftThumbDeflection( &x, &y, current_action.m_DeviceType );
          amount = y * current_action.m_fDelta;
        }
        break;

        case AXIS_RIGHT_THUMB_X:
        {
          float x, y;
          mInputManager->GetGamePadRightThumbDeflection( &x, &y, current_action.m_DeviceType );
          amount = x * current_action.m_fDelta;
        }
        break;

        case AXIS_RIGHT_THUMB_Y:
        {
          float x, y;
          mInputManager->GetGamePadRightThumbDeflection( &x, &y, current_action.m_DeviceType );
          amount = y * current_action.m_fDelta;
        }
        break;

        case AXIS_DELTA_TRIGGER_RIGHT:
        {
          float right, left;
          mInputManager->GetGamePadDeltaTriggers( &left, &right, current_action.m_DeviceType );
          amount = right * current_action.m_fDelta;
        }
        break;

        case AXIS_DELTA_TRIGGER_LEFT:
        {
          float right, left;
          mInputManager->GetGamePadDeltaTriggers( &left, &right, current_action.m_DeviceType );
          amount = left * current_action.m_fDelta;
        }
        break;
        }//END switch(current_action.m_AxisType)

        if ( amount == 0.f )
          doIt = false;

      }

      lDeviceType = current_action.m_DeviceType;
      if ( current_action.m_EventType == EVENT_DOWN )
        doIt = doIt && mInputManager->IsDown( current_action.m_DeviceType, current_action.m_Code );
      else if ( current_action.m_EventType == EVENT_DOWN_UP )
        doIt = doIt && mInputManager->IsDownUp( current_action.m_DeviceType, current_action.m_Code );
      else if ( current_action.m_EventType == EVENT_UP_DOWN )
        doIt = doIt && mInputManager->IsUpDown( current_action.m_DeviceType, current_action.m_Code );
      else if ( current_action.m_EventType == EVENT_NOTHING && current_action.m_AxisType != AXIS_NOTHING )
        doIt = true;
      else
        doIt = false;
    }

    if( doIt )
    {
      SActionDone lAction = { doIt, amount, lDeviceType };
      mDoActions[lItb->first] = lAction;
    }
  }
}

bool CActionManager::DoAction( const std::string& action, float32& amount )
{
  //mMutex.Lock();
  MapActionsDone::iterator lItfind = mDoActions.find( action );

  if ( lItfind != mDoActions.end() )
  {
    amount = lItfind->second.mActionAmount;
    mMutex.UnLock();
    return lItfind->second.mIsActionDone;
  }

  //mMutex.UnLock();
  return false;
}

bool CActionManager::AnyKey()
{
  for( MapActionsDone::iterator lItb = mDoActions.begin(), lIte = mDoActions.end(); lItb != lIte; ++lItb )
  {
     if ( lItb->second.m_DeviceType == IDV_KEYBOARD )
       return true;
  }
  return false;
}

bool CActionManager::LoadXML()
{
  bool lOk = true;

  CXMLTreeNode l_File, lActions;

  if ( !l_File.LoadAndFindNode( mConfigPath.c_str(), "Actions", lActions ) )
  {
    FATAL_ERROR( "Error opening the actions file %s", mConfigPath.c_str() );
    lOk = false;
  }
  else
  {
    for ( uint32 i = 0, lCount = lActions.GetNumChildren(); i < lCount; ++i )
    {
      const CXMLTreeNode& lCurrentNode = lActions( i );

      if ( std::string( lCurrentNode.GetName() ) == "action" )
      {
        std::vector<SInputActions> vector;

        for ( int j = 0, lChildren = lCurrentNode.GetNumChildren(); j < lChildren; ++j )
        {
          const CXMLTreeNode& lCurrentSubNode = lCurrentNode( j );
          const std::string lInputTag = lCurrentSubNode.GetName();

          if ( lInputTag == "input" )
          {
            const std::string& lDeviceType  = lCurrentSubNode.GetAttribute<std::string>( "deviceType", "IDV_NOTHING" );
            const std::string& lAxisType    = lCurrentSubNode.GetAttribute<std::string>( "AxisType", "AXIS_NOTHING" );
            const std::string& lEventType   = lCurrentSubNode.GetAttribute<std::string>( "EventType", "EVENT_NOTHING" );
            const std::string& lKeyType     = lCurrentSubNode.GetAttribute<std::string>( "Code", "MOUSE_BUTTON_NOTHING" );

            SInputActions new_action =
            {
              ( EInputDeviceType )strDeviceToCode( lDeviceType ),
              ( EInputEventType )strEventToCode( lEventType ),
              ( EInputAxisType )strAxisToCode( lAxisType ),
              strKeyToCode( lKeyType ),
              lCurrentSubNode.GetAttribute<float32>( "Delta", 1.0f ),
              lDeviceType,
              lAxisType,
              lEventType,
              lKeyType
            };

            vector.push_back( new_action );
          }
        }

        mActions[std::string( lCurrentNode.GetAttribute<std::string>( "name", "" ) )] = vector;
      }
    }
  }

  return lOk;
}

bool CActionManager::SaveXML( const std::string& xmlFile )
{
  bool isOk = true;
  CXMLTreeNode NewXML;

  if ( NewXML.StartNewFile( xmlFile.c_str() ) )
  {
    // We fill the doc here
    NewXML.StartElement( "Actions" );
    MapActions::const_iterator it = mActions.begin();
    MapActions::const_iterator itEnd = mActions.end();

    for ( ; it != itEnd; ++it )
    {
      NewXML.StartElement( "action" );
      NewXML.AddAttribute<std::string>( "name", it->first );
      TInputsVector vInputs = it->second;
      TInputsVector::const_iterator aux_it = vInputs.begin();
      TInputsVector::const_iterator aux_itEnd = vInputs.end();

      for ( ; aux_it != aux_itEnd; ++aux_it )
      {
        //<input deviceType="IDV_KEYBOARD"      EventType="EVENT_DOWN_UP"               Code="KEY_F4"/>
        //<input deviceType="IDV_MOUSE"                 AxisType="AXIS_MOUSE_X"                 Delta="1.f"/>
        NewXML.StartElement( "input" );
        NewXML.AddAttribute<std::string>( "deviceType", aux_it->m_sDeviceType );

        if ( aux_it->m_sEventType != "EVENT_NOTHING" )
          NewXML.AddAttribute<std::string>( "EventType", aux_it->m_sEventType );

        if ( aux_it->m_sAxisType != "AXIS_NOTHING" )
        {
          NewXML.AddAttribute<std::string>( "AxisType", aux_it->m_sAxisType );
          NewXML.AddAttribute<float>( "Delta", aux_it->m_fDelta );
        }

        if ( aux_it->m_sCode != "MOUSE_BUTTON_NOTHING" )
          NewXML.AddAttribute<std::string>( "Code", aux_it->m_sCode );

        NewXML.EndElement(); //("input")
      }

      NewXML.EndElement(); //("action");
    }

    NewXML.EndElement(); //("Actions");

    NewXML.EndNewFile();
  }
  else
    bool isOk = false;

  return isOk;
}

void CActionManager::SetAction( const std::string& action, TInputsVector& a_vInputs )
{
  MapActions::const_iterator it = mActions.find( action );

  if ( it != mActions.end() )
    mActions.erase( it );

  mActions[action] = a_vInputs;
}

EInputDeviceType CActionManager::strDeviceToCode( const std::string& strAction )
{
  if ( "IDV_MOUSE" == strAction )
    return ( IDV_MOUSE );
  else if ( "IDV_KEYBOARD" == strAction )
    return ( IDV_KEYBOARD );
  else if ( "IDV_GAMEPAD1" == strAction )
    return ( IDV_GAMEPAD1 );
  else if ( "IDV_GAMEPAD2" == strAction )
    return ( IDV_GAMEPAD2 );
  else if ( "IDV_GAMEPAD3" == strAction )
    return ( IDV_GAMEPAD3 );
  else if ( "IDV_GAMEPAD4" == strAction )
    return ( IDV_GAMEPAD4 );

  return ( IDV_NOTHING );
}

EInputEventType CActionManager::strEventToCode( const std::string& strEvent )
{
  if ( "EVENT_DOWN" == strEvent )
    return ( EVENT_DOWN );
  else if ( "EVENT_DOWN_UP" == strEvent )
    return ( EVENT_DOWN_UP );
  else if ( "EVENT_UP_DOWN" == strEvent )
    return ( EVENT_UP_DOWN );

  return ( EVENT_NOTHING );
}

unsigned int CActionManager::strKeyToCode( const std::string& strKey )
{
  if ( "KEY_TAB" == strKey )
    return ( KEY_TAB );
  else if ( "KEY_BACK" == strKey )
    return ( KEY_BACK );
  else if ( "KEY_SPACE" == strKey )
    return ( KEY_SPACE );
  else if ( "KEY_ESCAPE" == strKey )
    return ( KEY_ESCAPE );
  else if ( "KEY_RETURN" == strKey )
    return ( KEY_RETURN );
  else if ( "KEY_CAPITAL" == strKey )
    return ( KEY_CAPITAL );

  else if ( "KEY_MINUS" == strKey )
    return ( KEY_MINUS );
  else if ( "KEY_EQUALS" == strKey )
    return ( KEY_EQUALS );
  else if ( "KEY_LBRACKET" == strKey )
    return ( KEY_LBRACKET );
  else if ( "KEY_RBRACKET" == strKey )
    return ( KEY_RBRACKET );
  else if ( "KEY_SEMICOLON" == strKey )
    return ( KEY_SEMICOLON );
  else if ( "KEY_APOSTROPHE" == strKey )
    return ( KEY_APOSTROPHE );

  else if ( "KEY_BACKSLASH" == strKey )
    return ( KEY_BACKSLASH );
  else if ( "KEY_PERIOD" == strKey )
    return ( KEY_PERIOD );
  else if ( "KEY_SLASH" == strKey )
    return ( KEY_SLASH );
  else if ( "KEY_COMMA" == strKey )
    return ( KEY_COMMA );
  else if ( "KEY_GRAVE" == strKey )
    return ( KEY_GRAVE );

  else if ( "KEY_A" == strKey )
    return ( KEY_A );
  else if ( "KEY_S" == strKey )
    return ( KEY_S );
  else if ( "KEY_D" == strKey )
    return ( KEY_D );
  else if ( "KEY_F" == strKey )
    return ( KEY_F );
  else if ( "KEY_G" == strKey )
    return ( KEY_G );
  else if ( "KEY_H" == strKey )
    return ( KEY_H );
  else if ( "KEY_J" == strKey )
    return ( KEY_J );
  else if ( "KEY_K" == strKey )
    return ( KEY_K );
  else if ( "KEY_L" == strKey )
    return ( KEY_L );
  else if ( "KEY_X" == strKey )
    return ( KEY_X );
  else if ( "KEY_C" == strKey )
    return ( KEY_C );
  else if ( "KEY_B" == strKey )
    return ( KEY_B );
  else if ( "KEY_N" == strKey )
    return ( KEY_N );
  else if ( "KEY_M" == strKey )
    return ( KEY_M );
  else if ( "KEY_Q" == strKey )
    return ( KEY_Q );
  else if ( "KEY_W" == strKey )
    return ( KEY_W );
  else if ( "KEY_E" == strKey )
    return ( KEY_E );
  else if ( "KEY_R" == strKey )
    return ( KEY_R );
  else if ( "KEY_T" == strKey )
    return ( KEY_T );
  else if ( "KEY_Y" == strKey )
    return ( KEY_Y );
  else if ( "KEY_U" == strKey )
    return ( KEY_U );
  else if ( "KEY_I" == strKey )
    return ( KEY_I );
  else if ( "KEY_O" == strKey )
    return ( KEY_O );
  else if ( "KEY_P" == strKey )
    return ( KEY_P );

  else if ( "KEY_1" == strKey )
    return ( KEY_1 );
  else if ( "KEY_2" == strKey )
    return ( KEY_2 );
  else if ( "KEY_3" == strKey )
    return ( KEY_3 );
  else if ( "KEY_4" == strKey )
    return ( KEY_4 );
  else if ( "KEY_5" == strKey )
    return ( KEY_5 );
  else if ( "KEY_6" == strKey )
    return ( KEY_6 );
  else if ( "KEY_7" == strKey )
    return ( KEY_7 );
  else if ( "KEY_8" == strKey )
    return ( KEY_8 );
  else if ( "KEY_9" == strKey )
    return ( KEY_9 );
  else if ( "KEY_0" == strKey )
    return ( KEY_0 );

  else if ( "KEY_F1" == strKey )
    return ( KEY_F1 );
  else if ( "KEY_F2" == strKey )
    return ( KEY_F2 );
  else if ( "KEY_F3" == strKey )
    return ( KEY_F3 );
  else if ( "KEY_F4" == strKey )
    return ( KEY_F4 );
  else if ( "KEY_F5" == strKey )
    return ( KEY_F5 );
  else if ( "KEY_F6" == strKey )
    return ( KEY_F6 );
  else if ( "KEY_F7" == strKey )
    return ( KEY_F7 );
  else if ( "KEY_F8" == strKey )
    return ( KEY_F8 );
  else if ( "KEY_F9" == strKey )
    return ( KEY_F9 );
  else if ( "KEY_F10" == strKey )
    return ( KEY_F10 );
  else if ( "KEY_F11" == strKey )
    return ( KEY_F11 );
  else if ( "KEY_F12" == strKey )
    return ( KEY_F12 );

  else if ( "KEY_NPPLUS" == strKey )
    return ( KEY_NPPLUS );
  else if ( "KEY_NPMINUS" == strKey )
    return ( KEY_NPMINUS );
  else if ( "KEY_NPENTER" == strKey )
    return ( KEY_NPENTER );
  else if ( "KEY_NUMLOCK" == strKey )
    return ( KEY_NUMLOCK );
  else if ( "KEY_NPCOMMA" == strKey )
    return ( KEY_NPCOMMA );
  else if ( "KEY_NPDIVIDE" == strKey )
    return ( KEY_NPDIVIDE );
  else if ( "KEY_NPDECIMAL" == strKey )
    return ( KEY_NPDECIMAL );
  else if ( "KEY_NPMULTIPLY" == strKey )
    return ( KEY_NPMULTIPLY );

  else if ( "KEY_NP1" == strKey )
    return ( KEY_NP1 );
  else if ( "KEY_NP2" == strKey )
    return ( KEY_NP2 );
  else if ( "KEY_NP3" == strKey )
    return ( KEY_NP3 );
  else if ( "KEY_NP4" == strKey )
    return ( KEY_NP4 );
  else if ( "KEY_NP5" == strKey )
    return ( KEY_NP5 );
  else if ( "KEY_NP6" == strKey )
    return ( KEY_NP6 );
  else if ( "KEY_NP7" == strKey )
    return ( KEY_NP7 );
  else if ( "KEY_NP8" == strKey )
    return ( KEY_NP8 );
  else if ( "KEY_NP9" == strKey )
    return ( KEY_NP9 );
  else if ( "KEY_NP0" == strKey )
    return ( KEY_NP0 );

  else if ( "KEY_RSHIFT" == strKey )
    return ( KEY_RSHIFT );
  else if ( "KEY_LSHIFT" == strKey )
    return ( KEY_LSHIFT );
  else if ( "KEY_RCTRL" == strKey )
    return ( KEY_RCTRL );
  else if ( "KEY_LCTRL" == strKey )
    return ( KEY_LCTRL );
  else if ( "KEY_RALT" == strKey )
    return ( KEY_RALT );
  else if ( "KEY_LALT" == strKey )
    return ( KEY_LALT );
  else if ( "KEY_LWIN" == strKey )
    return ( KEY_LWIN );
  else if ( "KEY_RWIN" == strKey )
    return ( KEY_RWIN );

  else if ( "KEY_UP" == strKey )
    return ( KEY_UP );
  else if ( "KEY_DOWN" == strKey )
    return ( KEY_DOWN );
  else if ( "KEY_LEFT" == strKey )
    return ( KEY_LEFT );
  else if ( "KEY_RIGHT" == strKey )
    return ( KEY_RIGHT );

  else if ( "KEY_INSERT" == strKey )
    return ( KEY_INSERT );
  else if ( "KEY_DELETE" == strKey )
    return ( KEY_DELETE );
  else if ( "KEY_HOME" == strKey )
    return ( KEY_HOME );
  else if ( "KEY_END" == strKey )
    return ( KEY_END );

  else if ( "KEY_PGDOWN" == strKey )
    return ( KEY_PGDOWN );
  else if ( "KEY_PGUP" == strKey )
    return ( KEY_PGUP );

  else if ( "KEY_PAUSE" == strKey )
    return ( KEY_PAUSE );
  else if ( "KEY_SCROLL" == strKey )
    return ( KEY_SCROLL );

  else if ( "MOUSE_BUTTON_LEFT" == strKey )
    return ( MOUSE_BUTTON_LEFT );
  else if ( "MOUSE_BUTTON_RIGHT" == strKey )
    return ( MOUSE_BUTTON_RIGHT );
  else if ( "MOUSE_BUTTON_MIDDLE" == strKey )
    return ( MOUSE_BUTTON_MIDDLE );

  return ( 0 );
}

EInputAxisType    CActionManager::strAxisToCode( const std::string& strAxis )
{
  if ( "AXIS_MOUSE_X" == strAxis )
    return ( AXIS_MOUSE_X );
  else if ( "AXIS_MOUSE_Y" == strAxis )
    return ( AXIS_MOUSE_Y );
  else if ( "AXIS_MOUSE_Z" == strAxis )
    return ( AXIS_MOUSE_Z );
  else if ( "AXIS_LEFT_THUMB_X" == strAxis )
    return ( AXIS_LEFT_THUMB_X );
  else if ( "AXIS_LEFT_THUMB_Y" == strAxis )
    return ( AXIS_LEFT_THUMB_Y );
  else if ( "AXIS_RIGHT_THUMB_X" == strAxis )
    return ( AXIS_RIGHT_THUMB_X );
  else if ( "AXIS_RIGHT_THUMB_Y" == strAxis )
    return ( AXIS_RIGHT_THUMB_Y );
  else if ( "AXIS_DELTA_TRIGGER_RIGHT" == strAxis )
    return ( AXIS_DELTA_TRIGGER_RIGHT );
  else if ( "AXIS_DELTA_TRIGGER_LEFT"  == strAxis )
    return ( AXIS_DELTA_TRIGGER_LEFT );

  return ( AXIS_NOTHING );

}

bool CActionManager::Reload()
{
  mActions.clear();
  LoadXML();

  return true;
}

void CActionManager::Update()
{
  if ( DoAction( "ReloadActionToInput" ) )
    Reload();
}