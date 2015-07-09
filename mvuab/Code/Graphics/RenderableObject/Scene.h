#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <string>
#include "Utils\MapManager.h"
#include "Utils\Manager.h"
#include "Utils\SingletonPattern.h"

class CRenderableObjectsLayersManager;
class CRoom;

class CScene : public CMapManager<CRoom>, public CManager
{
    public:
		CScene   () : CManager() {};
		CScene(const CXMLTreeNode& atts);
        ~CScene  ();

        bool Load( const std::string &l_FilePath );
		bool Reload();

		void Init();
        void Update();
        void Render();

		void Destroy();

		void LoadRoom( std::string aRoomName );
		void ActivateRoom( std::string aRoomName );

		void UnloadRoom( std::string aRoomName );
		void DesactivateRoom( std::string aRoomName );
};

#endif