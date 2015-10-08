#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <string>
#include "Utils\Manager.h"
#include "Utils/TemplatedVectorMapManager.h"
#include "Utils\SingletonPattern.h"
#include "Room.h"

class CRenderableObjectsLayersManager;
class CRenderableObjectsManager;
class CLightProbe;

class CScene : public CTemplatedVectorMapManager<CRoom>, public CManager
{
public:

  CScene() : CManager() {};
  CScene( const CXMLTreeNode& atts );
  ~CScene();

  bool Load( const std::string& l_FilePath );
  bool Reload();
  void RenderLayer( const std::string& aLayerName );

  void Init();
  void Update();
  void Render();

  void Destroy();

  void ThreadLoadRoom();
  void ActivateRoom(  const std::string& aRoomName );
  void LoadRoom( const std::string& aRoomName );

  void UnloadRoom( std::string aRoomName );
  void DesactivateRoom( std::string aRoomName );

  std::vector<CLightProbe*> GetClosedLightProbes( std::string, Math::Vect3f );
  const std::string GetActivateRoom();

  CRoom* GetCurrentRoom(){return mCurrentRoom;}

private:
  CRoom* mCurrentRoom;
  CRenderableObjectsManager *mToDelete;
};

#endif