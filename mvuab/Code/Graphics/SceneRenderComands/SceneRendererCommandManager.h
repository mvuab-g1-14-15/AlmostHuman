#ifndef SCENE_RENDERER_COMMAND_MANAGER_
#define SCENE_RENDERER_COMMAND_MANAGER_
#pragma once


#include "Utils\TemplatedVectorMapManager.h"
#include "RenderGUISceneRendererCommand.h"
#include "SceneRendererCommand.h"
#include <string>
#include "Utils/Manager.h"
#include "Utils\ObjectFactory.h"

class CSceneRendererCommandManager : public CManager
{
public:
    CSceneRendererCommandManager();
    CSceneRendererCommandManager( CXMLTreeNode& atts );
    ~ CSceneRendererCommandManager();
    CRenderGUISceneRendererCommand* GetCommandGUI( );

    void Init();
    bool Load( const std::string& lFile );
    void ReLoad();
    bool Execute();
    void Update() {}
    void Render() {}

    bool GetVisibleCommand( std::string );
    void SetVisibleCommand( std::string, bool );

private:
    CTemplatedVectorMapManager<CSceneRendererCommand> m_SceneRendererCommands;

    void CleanUp();
    std::string GetNextName();

    ObjectFactory1< CSceneRendererCommand, CXMLTreeNode, std::string > m_CommandFactory;
};
#endif