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
#include <vector>
#include "Math/Vector3.h"
#include "Math\MathUtils.h"
#include "Utils\TemplatedVectorMapManager.h"

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
  AKRESULT SetSwitch( const std::string& _Group, const std::string& _Gravel, const std::string& _KeyGameObjectMap );
  AKRESULT RegisterGameObject( const std::string& _KeyGameObjectMap );
  AKRESULT UnregisterGameObject( const std::string& _KeyGameObjectMap );
  AKRESULT SetState( const std::string& _Group, const std::string& _State );
  AKRESULT SetListenerPosition( Math::Vect3f _ListenerPosition, Math::Vect3f _Direction, Math::Vect3f _Up );
  AKRESULT SetGameObjectPosition( const std::string& _KeyGameObjectMap, Math::Vect3f _GameObjectPosition, Math::Vect3f _GameObjectOrientation );
  AKRESULT SetGameObjectMultiplePositions( const std::string& _KeyGameObjectMap, std::vector<Math::Vect3f> _GameObjectPosition,
      std::vector<Math::Vect3f> _GameObjectOrientation, const std::string& _TypePos );
  /******************************* Particles OP  ************/
  void LoadParticlePosSound(const std::string& aFileName);
  void PlayParticlesSFX();
    /***************************** BASIC OPS ***********************************************/
  AkPlayingID PlayEvent( const std::string& _Event, const std::string& _GameObjectId );
  void ResumeEvent( const std::string& _Event, const std::string& _KeyGameObjectMap );
  void PauseEvent( const std::string& _Event, const std::string& _KeyGameObjectMap );
  void ResumeAllFromGameObject( const std::string& _KeyGameObjectMap );
  void PauseAllFromGameObject( const std::string& _KeyGameObjectMap );

  AkGameObjectID GetGameObjectMapById( const std::string& _KeyGameObjectMap );
  void SetGameObjectMapById( const std::string& _KeyGameObjectMap );
  //void PauseAllSounds();
  //void ResumeAllSounds();

private:

  CAkDefaultIOHookBlocking* m_lowLevelIO;
  std::map<std::string, AkGameObjectID> m_GameObjectMap;
  std::map<std::string, std::string> m_ParticlesSoundMap;
  bool m_GamePaused;
  AkGameObjectID m_ObjectId;

};
#endif // INC_SOUND_MANAGER_H_
