#include "ViewerProcess.h"
//SYSTEM
#include <iosfwd>
#include <shlobj.h>
#include <winuser.h>
#include "Shlobj.h"
#include <sstream>
#include <iostream>

//BASE
#include "Logger\Logger.h"
#include "Utils\Defines.h"
#include "Fonts\FontManager.h"
#include "ScriptManager.h"
#include "Utils\Manager.h"

//CORE

#include "EngineConfig.h"
#include "EngineManagers.h"
#include "Engine.h"

//GRAPHICS
#include "GraphicsManager.h"
#include "Lights\LightManager.h"
#include "Cameras\CameraManager.h"
#include "Effects\EffectManager.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "RenderableVertex\IndexedVertexs.h"
#include "SceneRenderComands\SceneRendererCommandManager.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "Billboard\Billboard.h"

//INPUTS
#include "InputManager.h"
#include "ActionManager.h"

//PHYSX
#include "PhysicsManager.h"
#include "Actor\PhysicActor.h"
#include "Utils/PhysicUserData.h"
#include "Actor\PhysicController.h"
#include "Cooking Mesh\PhysicCookingMesh.h"

//SOUND
#include "SoundManager.h"

//GUI
#include "GUIManager.h"

#include <algorithm>
#include "RenderableVertex\VertexTypes.h"
#include "Lights\OmniLight.h"
#include "StaticMeshes\InstanceMesh.h"

using namespace Math;

CViewerProcess::CViewerProcess( void ) :
    CProcess()
{
    m_AStar = 0;
}

CViewerProcess::~CViewerProcess( void )
{
    DeInit();
}

void CViewerProcess::Init()
{
    CreateViewerCameras();
}

std::string CViewerProcess::OpenPicker( std::string Extension = "" )
{
    //fileNameExample       // common dialog box structure
    OPENFILENAME ofn;
    char szFile[560];       // buffer for file name
    HWND hwnd = EngineConfigInstance->GetWindowId();             // owner window


    // Initialize OPENFILENAME
    ZeroMemory( &ofn, sizeof( ofn ) );
    ofn.lStructSize = sizeof( ofn );
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof( szFile );

    if ( Extension == "mesh" )
    {
        ofn.lpstrFilter = "mesh\0*.MESH\0";
    }
    else if ( Extension == "xml" )
    {
        ofn.lpstrFilter = "xml\0*.XML\0";
    }
    else
    {
        ofn.lpstrFilter = "*\0*.ALL\0";
    }

    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = ( "./Data" );
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    int test = GetOpenFileName( &ofn );

    if ( test )
    {
        return ofn.lpstrFile;
    }

    return "";
}
INT CALLBACK BrowseCallbackProc( HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData )
{
    TCHAR szDir[MAX_PATH];
    char* Data = "\\Data";
    int sze = sizeof( szDir ) / sizeof( TCHAR );

    switch ( uMsg )
    {
        case BFFM_INITIALIZED:
            if ( GetCurrentDirectory( sizeof( szDir ) / sizeof( TCHAR ), szDir ) )
            {
                int sze = strlen( szDir );
                memcpy( szDir + sze, Data, strlen( Data ) + 1 );
                szDir;
                SendMessage( hwnd, BFFM_SETSELECTION, TRUE, ( LPARAM )szDir );
            }

            break;

        case BFFM_SELCHANGED:

            // Set the status window to the currently selected path.
            if ( SHGetPathFromIDList( ( LPITEMIDLIST )lp, szDir ) )
            {
                SendMessage( hwnd, BFFM_SETSTATUSTEXT, 0, ( LPARAM )szDir );
            }

            break;
    }

    return 0;
}

std::string CViewerProcess::OpenFolder()
{
    TCHAR szDir[MAX_PATH];
    TCHAR szPath[MAX_PATH];
    BROWSEINFO bInfo;
    bInfo.hwndOwner = EngineConfigInstance->GetWindowId();
    bInfo.pidlRoot = NULL;
    bInfo.pszDisplayName = szDir; // Address of a buffer to receive the display name of the folder selected by the user
    bInfo.lpszTitle = "Please, select a folder"; // Title of the dialog
    bInfo.ulFlags = BIF_STATUSTEXT || BIF_NEWDIALOGSTYLE;
    bInfo.lpfn = BrowseCallbackProc;
    bInfo.lParam = ( LPARAM )&".";
    bInfo.iImage = -1;
    LPITEMIDLIST lpItem = SHBrowseForFolder( &bInfo );

    if ( lpItem != NULL )
    {
        SHGetPathFromIDList( lpItem, szPath );
        CoTaskMemFree( lpItem );
        return szPath;
    }

    return "";
}
void CViewerProcess::RenderModel( int wParam )
{
    if ( wParam != ALLMODELS && wParam > 10000 )
    {
        int l_Layer = wParam / 10000;
        int l_Obj = wParam - ( l_Layer * 10000 );
        l_Layer--;
        std::ostringstream Ostring;
        Ostring  << "move_camera_to_object(" << l_Layer << "," << l_Obj << ")";
        std::string funcion( Ostring.str() );
        //ScriptMInstance->RunCode( funcion );
    }

    if ( wParam >= 500 && wParam < 600 )
    {
        int l_Light = wParam - 500;
        std::ostringstream Ostring;
        Ostring  << "move_camera_to_light(" << l_Light << ")";
        std::string funcion( Ostring.str() );
        //ScriptMInstance->RunCode( funcion );
    }

    if ( wParam >= 600 && wParam < 700 )
    {
        int l_Light = wParam - 500;
        std::ostringstream Ostring;
        Ostring  << "move_camera_to_light(" << l_Light << ")";
        std::string funcion( Ostring.str() );
        //ScriptMInstance->RunCode( funcion );
    }
}

bool CViewerProcess::LoadFile( S_MenuType menu )
{
    std::string l_FileName;

    switch ( menu )
    {
        //Load SCENE
        case SCENE:
            {


                /*  l_FileName = OpenFolder();

                    if ( l_FileName != "" )
                    {
                    std::vector<std::string> strings;
                    std::istringstream f( l_FileName );
                    std::string s;

                    while ( std::getline( f, s, '\\' ) )
                    {
                     std::cout << s << std::endl;
                     strings.push_back( s );
                    }

                    const std::string &Level = strings[strings.size() - 1];
                    //ScriptMInstance->RunCode( "load_gameplay()" );

                    //Create light
                    COmniLight* l_newLight = new COmniLight();
                    l_newLight->SetPosition( Vect3f( 0, 4, 0 ) );

                    if (LightMInstance->GetLightCount() == 0)
                    {
                     //TODO poner en metodo
                      LightMInstance->AddResource( "newLight", l_newLight );
                    }



                    HMENU l_MenuSuperior = GetMenu( EngineConfigInstance->GetWindowId() );
                    int a = DeleteMenu( l_MenuSuperior, 3, MF_BYPOSITION );
                    int b = DeleteMenu( l_MenuSuperior, 2, MF_BYPOSITION );
                    size_t c_layers = ROLMInstance->GetResourcesVector().size();

                    HMENU l_MenuModelos = CreateMenu();
                    AppendMenu( l_MenuModelos, MF_STRING, ALLMODELS, TEXT( "Scene" ) );
                    AppendMenu( l_MenuModelos, MF_SEPARATOR, 0, NULL );

                    for ( size_t i = 0; i < ( c_layers ); ++i )
                    {
                     size_t l_countObjs = ROLMInstance->GetResourceById( i )->GetResourcesVector().size();
                     std::string l_LayerName = ROLMInstance->GetResourceById( i )->GetResource()->GetName();
                     HMENU l_MenuLayer = CreateMenu();

                     for ( size_t j = 0; j < l_countObjs; ++j )
                     {
                       std::string l_ObjName = ROLMInstance->GetResourceById( i )->GetResourceById( j )->GetName();
                       AppendMenu( l_MenuLayer, MF_STRING, 10000 * ( i + 1 ) + j, TEXT( l_ObjName.c_str() ) );
                     }

                     AppendMenu( l_MenuModelos, MF_STRING | MF_POPUP, ( UINT )l_MenuLayer, TEXT( l_LayerName.c_str() ) );
                    }

                    size_t c_lights = LightMInstance->GetResourcesVector().size();
                    HMENU l_MenuLights = CreateMenu();

                    for ( size_t i = 0; i < c_lights; ++i )
                    {
                     std::string l_LightName = LightMInstance->GetResourceById( i )->GetName();
                     AppendMenu( l_MenuLights, MF_STRING, 500 + i, TEXT( l_LightName.c_str() ) );
                    }

                    AppendMenu( l_MenuSuperior, MF_STRING | MF_POPUP, ( UINT )l_MenuModelos, TEXT( "Models" ) );
                    AppendMenu( l_MenuSuperior, MF_STRING | MF_POPUP, ( UINT )l_MenuLights, TEXT( "Lights" ) );
                    DrawMenuBar( EngineConfigInstance->GetWindowId() );
                    return true;
                    }

                    break;*/
            }

        case MESH:
            {
                /*  l_FileName = OpenPicker( "mesh" );

                    if ( l_FileName != "" )
                    {
                    HMENU l_MenuSuperior = GetMenu( );
                    DeleteMenu( l_MenuSuperior, 3, MF_BYPOSITION );
                    DeleteMenu( l_MenuSuperior, 2, MF_BYPOSITION );
                    SMeshMInstance->Destroy();
                    //AnimatedMInstance->Destroy();
                    ROLMInstance->Destroy();

                    COmniLight* l_newLight = new COmniLight();
                    l_newLight->SetPosition( Vect3f( 0, 4, 0 ) );
                    LightMInstance->AddResource( "newLight", l_newLight );
                    CStaticMesh* l_StaticMesh = new CStaticMesh();
                    CRenderableObjectsManager* l_RenderableObjectLayer = new CRenderableObjectsManager( "solid" );
                    CRenderableObjectsManager* l_RenderableObjectLayer1 = new CRenderableObjectsManager( "solid_transferred" );

                    if ( l_StaticMesh->Load( l_FileName ) )
                    {
                    SMeshMInstance->AddResource( "newStaticMesh", l_StaticMesh );
                    CInstanceMesh* l_InstanceMesh = new CInstanceMesh( "newInstance", "newStaticMesh" );
                    //l_InstanceMesh->SetVisible( true );
                    l_RenderableObjectLayer->AddResource( "newObject", l_InstanceMesh );
                    ROLMInstance->AddResource( "solid", l_RenderableObjectLayer );
                    ROLMInstance->AddResource( "solid_transferred", l_RenderableObjectLayer1 );

                    //Script load mesh TODO
                    //ScriptMInstance->RunCode( "on_mesh_load(\"static\")" );
                    return true;
                    }
                    }

                    break;*/
            }

        case ALPHA:
            {
                /*  l_FileName1 = OpenPicker( "mesh" );

                    if ( l_FileName1 != "" )
                    {
                    //HMENU l_MenuSuperior = GetMenu( CORE->GetHWND() );
                    DeleteMenu( l_MenuSuperior, 3, MF_BYPOSITION );
                    DeleteMenu( l_MenuSuperior, 2, MF_BYPOSITION );
                    SMeshMInstance->Destroy();
                    //CORE->GetAnimatedModelManager()->Destroy();
                    ROLMInstance->Destroy();
                    LightMInstance->Destroy();
                    COmniLight* l_newLight = new COmniLight();
                    l_newLight->SetPosition( Vect3f( 0, 4, 0 ) );
                    LightMInstance->AddResource( "newLight", l_newLight );
                    CStaticMesh* l_StaticMesh = new CStaticMesh();
                    CRenderableObjectsManager* l_RenderableObjectLayer = new CRenderableObjectsManager( "decall" );
                    CRenderableObjectsManager* l_RenderableObjectLayer1 = new CRenderableObjectsManager( "solid" );
                    CRenderableObjectsManager* l_RenderableObjectLayer2 = new CRenderableObjectsManager( "solid_transferred" );

                    if ( l_StaticMesh->Load( l_FileName1 ) )
                    {
                    CORE->GetStaticMeshManager()->AddResource( "newStaticMesh", l_StaticMesh );
                    CInstanceMesh* l_InstanceMesh = new CInstanceMesh( "newInstance", "newStaticMesh" );
                    //l_InstanceMesh->SetVisible( true );
                    l_RenderableObjectLayer->AddResource( "newObject", l_InstanceMesh );
                    CORE->GetRenderableObjectsLayersManager()->AddResource( "decall", l_RenderableObjectLayer );
                    CORE->GetRenderableObjectsLayersManager()->AddResource( "solid", l_RenderableObjectLayer1 );
                    CORE->GetRenderableObjectsLayersManager()->AddResource( "solid_transferred", l_RenderableObjectLayer2 );
                    CORE->GetScriptManager()->RunCode( "on_mesh_load(\"decall\")" );
                    return true;
                    }
                    }

                    break;*/
            }

        case ANIMATION:
            {
                /*  l_FileName = OpenFolder();

                    if ( l_FileName != "" )
                    {
                    l_FileName1 += "//";
                    HMENU l_MenuSuperior = GetMenu( CORE->GetHWND() );
                    DeleteMenu( l_MenuSuperior, 3, MF_BYPOSITION );
                    DeleteMenu( l_MenuSuperior, 2, MF_BYPOSITION );
                    SMeshMInstance->Destroy();
                    //CORE->GetAnimatedModelManager()->Destroy();
                    ROLMInstance->Destroy();
                    LightMInstance->Destroy();
                    COmniLight* l_newLight = new COmniLight();
                    l_newLight->SetPosition( Vect3f( 0, 4, 0 ) );
                    LightMInstance->AddResource( "newLight", l_newLight );
                    CRenderableObjectsManager* l_RenderableObjectLayer = new CRenderableObjectsManager( "solid" );
                    CRenderableObjectsManager* l_RenderableObjectLayer1 = new CRenderableObjectsManager( "solid_transferred" );
                    ROLMInstance->AddResource( "solid", l_RenderableObjectLayer );
                    ROLMInstance->AddResource( "solid_transferred", l_RenderableObjectLayer1 );
                    LightMInstance->AddResource( "newLight", l_newLight );
                    // CAnimatedInstanceModel* l_AnimatedIModel = new CAnimatedInstanceModel();
                    // l_RenderableObjectLayer->AddResource( "newObject", l_AnimatedIModel );
                    // CAnimatedCoreModel* l_AnimatedCModel = CORE->GetAnimatedModelManager()->GetCore( "newCoreModel", l_FileName1 );
                    //l_AnimatedIModel->Initialize( l_AnimatedCModel, GraphicsInstance );
                    // l_AnimatedIModel->SetVisible( true );
                    // l_RenderableObjectLayer->AddResource( "newObject", l_AnimatedIModel );
                    //ScriptMInstance->RunCode( "on_mesh_load(\"animated\")" );
                    return true;
                    }

                    break;*/
            }

        /*  case CAMARAS:

            l_FileName1 = OpenPicker();
            if (l_FileName1 == "camaras.xml")
            {
            CORE->GetCameraManager()->Unload();
            CORE->GetCameraManager()->Load(l_FileName1);
            return true;
            }
            break;*/

        case CINEMATICS:
            /*  l_FileName1 = OpenPicker( "xml" );

                if ( l_FileName1 != "" )
                {
                CCinematic* CM = CORE->GetCinematicManager();
                ->LoadXML( l_FileName1 );
                CM->Play( true );
                return true;
                }*/

            break;

        default:
            break;
    }

    return false;
}

void CViewerProcess::DeInit()
{
}

void CViewerProcess::Update()
{
    ScriptMInstance->RunCode( "UpdateCameras()" );
}

void CViewerProcess::Render()
{
    uint32 lBaseGridSize = 500;
    GraphicsInstance->DrawGrid( float32(lBaseGridSize), CColor(0.5f, 0.5f, 0.5f, 1.0f), uint32( lBaseGridSize / 3 ),
                                uint32( lBaseGridSize / 3 ));
}

void CViewerProcess::RenderScene2D( CGraphicsManager * GM, CFontManager * FM, float ElapsedTime, float FPS )
{
    /*  CColor quad2dColor( 0.f, 0.f, 0.5f, 0.7f );
        Vect3f pos = m_Object->GetPosition();
        size_t count = 20;
        int id = FM->GetTTF_Id( "Titania" );
        count += FM->DrawTextA( 20, count, colWHITE, 1, "Posicion Cobaya: %f %f %f", pos.x, pos.y, pos.z );
        count += FM->DrawTextA( 20, count, colWHITE, 1, "Angulos Cobaya:\n   Yaw %f Pitch %f Roll %f", m_Object->GetYaw(), m_Object->GetPitch(),
                             m_Object->GetRoll() );
        count += FM->DrawTextA( 20, count, colWHITE, 1, "FPS: %f", FPS );
        CColor edgeColor = colWHITE;
        edgeColor.SetAlpha( 0.9f );
        GM->DrawRectangle2D( Vect2i( 18, 18 ), 300, count - 16, quad2dColor, 3, 3, edgeColor );
        count = 20;
        id = FM->GetTTF_Id( "Titania" );

        count += FM->DrawTextA( 20, count, colWHITE, 1, "Posicion Cobaya: %f %f %f", pos.x, pos.y, pos.z );
        count += FM->DrawTextA( 20, count, colWHITE, 1, "Angulos Cobaya:\n   Yaw %f Pitch %f Roll %f", m_Object->GetYaw(), m_Object->GetPitch(),
                             m_Object->GetRoll() );
        count += FM->DrawTextA( 20, count, colWHITE, 1, "FPS: %f", FPS );
        //count += FM->DrawDefaultText(20, count, colWHITE, "FPS: %f", FPS);
        //count += FM->DrawLiteral(20,count,"Exit");
        //count += FM->DrawLiteral(20,count,"Exit2");
        //count += FM->DrawLiteral(20,count,"Exit3");*/
}

void CViewerProcess::Clear()
{

    HMENU l_MenuSuperior = GetMenu( EngineConfigInstance->GetWindowId() );
    DeleteMenu( l_MenuSuperior, 3, MF_BYPOSITION );
    DeleteMenu( l_MenuSuperior, 2, MF_BYPOSITION );
    SMeshMInstance->Destroy();
    //AnimatedMInstance->Destroy();
    ROLMInstance->Destroy();

}

uint32 CViewerProcess::RenderDebugInfo( CGraphicsManager * GM, CFontManager * FM, float l_Fps )
{
    //CProcess::RenderDebugInfo(GM, FM, l_Fps);
    return 0;
}

void CViewerProcess::CreateLight( Math::Vect3f Position )
{
    COmniLight* l_newLight = new COmniLight();
    l_newLight->SetPosition( Position );
    LightMInstance->AddResource( "newLight", l_newLight );
}


void CViewerProcess::CreateCamera( Math::Vect3f Position , std::string CameraName )
{
    CCamera* l_newCamera = new CCamera();
    l_newCamera->SetPosition( Position );
    CameraMInstance->AddResource( CameraName, l_newCamera );
}

void CViewerProcess::CreateViewerCameras()
{
    CCameraManager* lCM = CameraMInstance;
    lCM->NewCamera
    (
        CCamera::Free,
        "ViewerFreeCamera",
        Vect3f(20.0f, 20.0f, 20.0f ),
        Vect3f(0.0f, 0.0f, 0.0f)
    );

    lCM->SetCurrentCamera("ViewerFreeCamera");
}