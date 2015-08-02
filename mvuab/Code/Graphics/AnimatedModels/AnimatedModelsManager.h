#ifndef ANIMATED_MODEL_MANAGER_H
#define ANIMATED_MODEL_MANAGER_H
#pragma once

#include "Utils\MapManager.h"
#include "Utils\Manager.h"

class CAnimatedInstanceModel;
class CAnimatedCoreModel;


class CAnimatedModelsManager : public CMapManager<CAnimatedCoreModel>, public CManager
{
    public:
        CAnimatedModelsManager();
        CAnimatedModelsManager( CXMLTreeNode& atts );
        ~CAnimatedModelsManager();
        CAnimatedCoreModel* GetCore( const std::string& Name );
        CAnimatedInstanceModel* GetInstance( const std::string& Name );
        void Init();
        void Reload();
        void Update() {}
        void Render() {}

};

#endif //ANIMATED_MODEL_MANAGER_H