//----------------------------------------------------------------------------------
// CSoundManager class
// Author: Enric Vergara
//
// Description:
// This secures availability of the necessary sounds functions.
// It internally uses the OpenAL library.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_WW_SOUND_MANAGER_H_
#define INC_WW_SOUND_MANAGER_H_

#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AK/SoundEngine/Common/AkModule.h>
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>
#include "AkSoundEngineExports.h"
#include "AkDefaultIOHookBlocking.h"
#include "XML\XMLTreeNode.h"
#include "Utils/Manager.h"
#include "Utils/BaseUtils.h"
#include <map>
#include "Math/Vector3.h"

class CWWSoundManager : public CManager
{
public:
  //----Init and End protocols------------------------------------
  CWWSoundManager() {}
  CWWSoundManager( const CXMLTreeNode& atts );
  virtual ~CWWSoundManager()
  {
    Done();
  }
  void  Init();
  void  Update();
  void  Done();

  void Render() {}
  bool Load( const std::string& xmlFile );

  void SetGamePaused( bool in_Paused )
  {
    m_GamePaused = in_Paused;
  }


  /***************************** WWISE FUNCTIONS *****************************************/
  AKRESULT SetSwitch( std::string _Group, std::string _Gravel, std::string _KeyGameObjectMap );
  AKRESULT RegisterGameObject( std::string _KeyGameObjectMap );
  AKRESULT UnregisterGameObject( std::string _KeyGameObjectMap );
  AKRESULT SetState( std::string _Group, std::string _State );
  AKRESULT SetListenerPosition( Math::Vect3f _ListenerPosition, Math::Vect3f _Direction, Math::Vect3f _Up );
  AKRESULT SetGameObjectPosition( std::string _KeyGameObjectMap, Math::Vect3f _GameObjectPosition, Math::Vect3f _GameObjectOrientation );

  /***************************** BASIC OPS ***********************************************/
  AkPlayingID PlayEvent( std::string _Event, std::string _GameObjectId );
  void ResumeEvent( std::string _Event, std::string _KeyGameObjectMap );
  void PauseEvent( std::string _Event, std::string _KeyGameObjectMap );
  void ResumeAllFromGameObject( std::string _KeyGameObjectMap );
  void PauseAllFromGameObject( std::string _KeyGameObjectMap );

  AkGameObjectID GetGameObjectMapById( std::string _KeyGameObjectMap );
  void SetGameObjectMapById( std::string _KeyGameObjectMap );
  //void PauseAllSounds();
  //void ResumeAllSounds();

private:

  CAkDefaultIOHookBlocking* m_lowLevelIO;
  std::map<std::string, AkGameObjectID> m_GameObjectMap;
  bool m_GamePaused;
  AkGameObjectID m_ObjectId;
};
#endif // INC_SOUND_MANAGER_H_
