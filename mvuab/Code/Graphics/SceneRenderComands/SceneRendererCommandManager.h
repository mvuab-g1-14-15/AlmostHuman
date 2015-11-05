#ifndef SCENE_RENDERER_COMMAND_MANAGER_
#define SCENE_RENDERER_COMMAND_MANAGER_
#pragma once


#include "Utils\TemplatedVectorMapManager.h"

#include "SceneRendererCommand.h"
#include <string>
#include "Utils/Manager.h"
#include "Utils\ObjectFactory.h"

class CZBlurSceneRendererCommand;
class CRenderGUISceneRendererCommand;
class CScatteredLightSceneRendererCommand;

class CSceneRendererCommandManager : public CManager, public CTemplatedVectorMapManager<CSceneRendererCommand>
{
    public:
        CSceneRendererCommandManager();
        CSceneRendererCommandManager(CXMLTreeNode& atts);
        ~ CSceneRendererCommandManager();

        CRenderGUISceneRendererCommand *GetCommandGUI();
        CScatteredLightSceneRendererCommand *GetScatteredLight();
        CZBlurSceneRendererCommand *GetZBlur();

        void Init();
        bool Load(const std::string& lFile);

        void ReLoad();
        bool Execute();

        void Update() {}
        void Render() {}

        bool GetVisibleCommand(std::string);
        void SetVisibleCommand(std::string, bool);

        CSceneRendererCommand* GetCommand(const std::string& aName)
        {
            return GetResource(aName);
        }

    private:
        void CleanUp();
        std::string GetNextName();

        ObjectFactory1< CSceneRendererCommand, CXMLTreeNode, std::string > m_CommandFactory;
};
#endif