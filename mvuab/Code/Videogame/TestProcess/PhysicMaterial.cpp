#include "TestProcess\PhysicMaterial.h"
#include "Utils\Defines.h"
#include "GraphicsManager.h"
#include "InputManager.h"
#include "ActionManager.h"
#include "Fonts\FontManager.h"
#include "Object3D.h"
#include "Math\Matrix44.h"
#include "Logger\Logger.h"
#include "Utils\LogRender.h"
#include "Utils\Defines.h"
#include "Math\AABB.h"

#include "Utils\BaseUtils.h"
#include "Timer\Timer.h"
#include "Periphericals\Mouse.h"

#include "ScriptManager.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "StaticMeshes\InstanceMesh.h"
#include "StaticMeshes\StaticMesh.h"

#include "RenderableVertex\RenderableVertex.h"
#include "RenderableVertex\IndexedVertexs.h"
#include "RenderableVertex\VertexTypes.h"
#include "Texture\Texture.h"

#include "StaticMeshes\StaticMesh.h"
#include "Cameras\CameraFPShooter.h"
#include "Cameras\CameraManager.h"

#include "Effects\EffectManager.h"
#include "RenderableVertex\VertexTypes.h"

#include "PhysicsManager.h"
#include "Actor\PhysicActor.h"
#include "Utils/PhysicUserData.h"

#include <d3dx9.h>

#include "Items\Grenade.h"

void GetFilesFromPath( const std::string& Path, std::vector<std::string>& _OutFiles );

CPhysicMaterial::CPhysicMaterial() : CProcess(),
    m_Speed( 0.1f ),
    m_Amount( 0.0f ), m_Angle( 0.0f ),  m_AngleMoon( 0.0f ), m_PaintAll( false )
{
    CameraMInstance->NewCamera( CCamera::FirstPerson, "TestProcessCam", Math::Vect3f( -15.0f, 2.0f, 0.0f ),
                                Math::Vect3f( 0.0f, 2.0f, 0.0f ) );
    CameraMInstance->SetCurrentCamera( "TestProcessCam" );

    unsigned short debug = VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TANGENT | VERTEX_TYPE_BINORMAL |
                           VERTEX_TYPE_TEXTURE1 |
                           VERTEX_TYPE_DIFFUSE;
}
bool done = false;

CPhysicMaterial::~CPhysicMaterial()
{
    CLogger::GetSingletonPtr()->SaveLogsInFile();
}

void CPhysicMaterial::Update()
{
    m_Amount +=  0.01f;
    m_Angle  += deltaTime * 0.05f;
    m_AngleMoon += deltaTime * 0.05f;

    CActionManager* pActionManager = ActionManagerInstance;

    if ( pActionManager->DoAction( "ReloadStaticMesh" ) )
    { SMeshMInstance->Reload(); }

    if ( pActionManager->DoAction( "ReloadLUA" ) )
    { ScriptMInstance->Reload(); }

    if ( pActionManager->DoAction( "ReloadShaders" ) )
    { EffectManagerInstance->Reload(); }

    if ( pActionManager->DoAction( "ReloadActionToInput" ) )
    { ActionManagerInstance->Reload(); }

    if ( pActionManager->DoAction( "SaveActionToInput" ) )
    { ActionManagerInstance->SaveXML( "Data/Prueba.xml" ); }

    if ( pActionManager->DoAction( "SetActionToInput" ) )
    {
        std::vector<S_INPUT_ACTION> vecInputAction;
        S_INPUT_ACTION sInputAction;
        sInputAction.m_DeviceType = IDV_KEYBOARD;
        sInputAction.m_EventType = EVENT_DOWN_UP;
        sInputAction.m_Code = KEY_H;
        sInputAction.m_sDeviceType = "IDV_KEYBOARD";
        sInputAction.m_sEventType = "EVENT_DOWN_UP";
        sInputAction.m_sCode = "KEY_H";
        vecInputAction.push_back( sInputAction );
        sInputAction.m_DeviceType = IDV_KEYBOARD;
        sInputAction.m_EventType = EVENT_DOWN;
        sInputAction.m_Code = KEY_LCTRL;
        sInputAction.m_sDeviceType = "IDV_KEYBOARD";
        sInputAction.m_sEventType = "EVENT_DOWN";
        sInputAction.m_sCode = "KEY_LCTRL";
        vecInputAction.push_back( sInputAction );
        ActionManagerInstance->SetAction( "SaveActionToInput", vecInputAction );
    }

    //RAYCAST
    if ( pActionManager->DoAction( "ShootRayCast" ) )
    {
        CCamera* l_CurrentCamera = CameraMInstance->GetCurrentCamera();

        if ( l_CurrentCamera )
        {
            CPhysicsManager* l_PM = PhysXMInstance;
            CPhysicUserData* l_PhysicUserData = new CPhysicUserData( "RayCast" );
            l_PhysicUserData->SetPaint( true );
            CPhysicActor* l_Actor = new CPhysicActor( l_PhysicUserData );
            //If don't want box, you can remove this line
            l_Actor->AddBoxShape( Math::Vect3f( 0.05f, 0.05f, 0.05f ), l_CurrentCamera->GetPos(), Math::Vect3f( 0, 0, 0 ) );
            l_Actor->CreateBody( 1.0f );

            // Add at the end allways it needs to have a shape
            l_PM->AddPhysicActor( l_Actor );

            l_Actor->SetLinearVelocity( l_CurrentCamera->GetDirection().GetNormalized() * 20.0f );

            SCollisionInfo& l_SCollisionInfo = SCollisionInfo::SCollisionInfo();
            uint32 mask = 1 << ECG_ESCENE;

            //CPhysicUserData* l_PUD = l_PM->RaycastClosestActor(l_CurrentCamera->GetPos(), l_CurrentCamera->GetDirection().GetNormalized(), mask, l_SCollisionInfo);
            CPhysicUserData* l_PUD = l_PM->RaycastClosestActorShoot( l_CurrentCamera->GetPos(),
                                     l_CurrentCamera->GetDirection().GetNormalized(), mask,
                                     l_SCollisionInfo, 40.0f );

            if ( l_PUD )
            { std::string l_Object = l_PUD->GetName(); }
            else
            { std::string l_Object = ""; }
        }
    }

    CCamera* l_CurrentCamera = CameraMInstance->GetCurrentCamera();

    if ( pActionManager->DoAction( "DisableCamera" ) )
    {
        CCamera* l_CurrentCamera = CameraMInstance->GetCurrentCamera();

        if ( l_CurrentCamera )
        {
            if ( l_CurrentCamera->GetEnable() )
            { l_CurrentCamera->SetEnable( false ); }
            else
            { l_CurrentCamera->SetEnable( true ); }
        }
    }

    if ( pActionManager->DoAction( "DrawActor" ) )
    {
        Vect2i l_PosMouse = InputManagerInstance->GetMouse()->GetPosition();
        Vect3f l_Pos, l_Dir;

        GraphicsInstance->GetRay( l_PosMouse, l_Pos, l_Dir );

        CPhysicsManager* l_PM = PhysXMInstance;

        SCollisionInfo& l_SCollisionInfo = SCollisionInfo::SCollisionInfo();
        uint32 mask = 1 << ECG_ESCENE;

        CPhysicUserData* l_PUD = l_PM->RaycastClosestActor( l_CurrentCamera->GetPos(),
                                 l_CurrentCamera->GetDirection().GetNormalized(), mask,
                                 l_SCollisionInfo );

        //CPhysicUserData* l_PUD = l_PM->RaycastClosestActor(l_Pos, l_Dir.GetNormalized(), mask, l_SCollisionInfo);
        if ( l_PUD )
        { l_PUD->SetColor( colRED ); }
    }

    ScriptMInstance->RunCode( "update()" );
}

void CPhysicMaterial::Init()
{
    ScriptMInstance->RunCode( "init()" );

    CPhysicsManager* l_PM = PhysXMInstance;

    CPhysicUserData* l_PUD = new CPhysicUserData( "Plane" );
    l_PUD->SetPaint( true );
    CPhysicActor* l_pPhysicActor = new CPhysicActor( l_PUD );
    int id = l_PM->AddMaterial( 0, 0, 0 );
    l_pPhysicActor->SetMaterialIndex( id );
    l_pPhysicActor->AddPlaneShape( Math::Vect3f( 0, 1.5f, 1 ), 0 );
    l_PM->AddPhysicActor( l_pPhysicActor );

    CPhysicUserData* l_PUD1 = new CPhysicUserData( "Box" );
    l_PUD1->SetPaint( true );
    l_PUD1->SetColor( colBLACK );
    CPhysicActor* l_pPhysicActor1 = new CPhysicActor( l_PUD1 );
    l_pPhysicActor1->AddBoxShape( Math::Vect3f( 1, 1, 1 ), Math::Vect3f( 0, 4, 0 ), Math::Vect3f( 0, 0, 0 ) );
    l_pPhysicActor1->CreateBody( 1.0f );
    l_PM->AddPhysicActor( l_pPhysicActor1 );

    CPhysicUserData* l_PUD2 = new CPhysicUserData( "Box2" );
    l_PUD2->SetPaint( true );
    CPhysicActor* l_pPhysicActor2 = new CPhysicActor( l_PUD2 );
    l_pPhysicActor2->SetMaterialIndex( 1 );
    l_pPhysicActor2->AddBoxShape( Math::Vect3f( 1, 1, 1 ), Math::Vect3f( 4, 4, 0 ), Math::Vect3f( 0, 0, 0 ) );
    l_pPhysicActor2->CreateBody( 1.0f );
    l_PM->AddPhysicActor( l_pPhysicActor2 );
}

void CPhysicMaterial::Render()
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
    FontInstance->DrawDefaultText( 300, 0, Math::CColor( 0.0f, 0.0f, 0.0f ), "Vertex: %u   Faces: %u   Draws:%u", v, f, d );
    // END: TO DELETE LATER IF IS NOT NECESSARY
}

void CPhysicMaterial::RenderDebugInfo()
{
    CProcess::RenderDebugInfo();
}