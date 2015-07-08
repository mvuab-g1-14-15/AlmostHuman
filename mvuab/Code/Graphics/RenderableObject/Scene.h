#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <string>
#include "Utils\MapManager.h"
#include "Utils\Manager.h"
#include "Utils\SingletonPattern.h"

class CRenderableObjectsLayersManager;

class CScene : public CMapManager<CRenderableObjectsLayersManager>, public CManager
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
};

#endif