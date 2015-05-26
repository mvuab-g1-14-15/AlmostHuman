#include "TestProcess.h"

//BASE
#include <Fonts\FontManager.h>
#include "Logger\Logger.h"
#include "ScriptManager.h"

//CORE

#include "EngineManagers.h"

//GRAPHICS
#include "GraphicsManager.h"
#include "Lights\LightManager.h"
#include "Cameras\CameraManager.h"
#include "Effects\EffectManager.h"
#include "RenderableVertex\VertexTypes.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "RenderableVertex\IndexedVertexs.h"
#include "SceneRenderComands\SceneRendererCommandManager.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"

//INPUT
#include "InputManager.h"
#include "ActionManager.h"





CTestProcess::CTestProcess() : CProcess()

{
    //CameraMInstance->NewCamera(CCamera::FirstPerson, "TestProcessCam", Math::Vect3f(-15.0f,2.0f,0.0f),
    //  Math::Vect3f(0.0f,2.0f,0.0f) );
    CameraMInstance->SetCurrentCamera( "TestProcessCam" );
    unsigned short debug = VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL |
                           VERTEX_TYPE_TANGENT | VERTEX_TYPE_BINORMAL | VERTEX_TYPE_TEXTURE1 |
                           VERTEX_TYPE_DIFFUSE;
}

CTestProcess::~CTestProcess()
{
    CLogger::GetSingletonPtr()->SaveLogsInFile();
}

void CTestProcess::Update()
{
    ScriptMInstance->RunCode( "update()" );
}

void CTestProcess::Init()
{
    ScriptMInstance->RunCode( "init()" );
}

void CTestProcess::Render()
{
    CGraphicsManager* pGraphicsManager = GraphicsInstance;

    /*  pGraphicsManager->DrawAxis(5);
        pGraphicsManager->DrawGrid(100, Math::colORANGE, 50, 50);*/
    //pGraphicsManager->DrawTeapot();
    ScriptMInstance->RunCode( "render()" );
    // START: TO DELETE LATER IF IS NOT NECESSARY,
    unsigned int v = CGPUStatics::GetSingletonPtr()->GetVertexCount();
    unsigned int f = CGPUStatics::GetSingletonPtr()->GetFacesCount();
    unsigned int d = CGPUStatics::GetSingletonPtr()->GetDrawCount();
    CGPUStatics::GetSingletonPtr()->SetToZero();
    FontInstance->DrawDefaultText( 300, 0,
                                   Math::CColor( 0.0f, 0.0f, 0.0f ), "Vertex: %u   Faces: %u   Draws:%u", v, f,
                                   d );
    // END: TO DELETE LATER IF IS NOT NECESSARY
}

void CTestProcess::RenderDebugInfo()
{
    CProcess::RenderDebugInfo();
}