#include "GraphicsFunctions.h"

#include "Texture\Texture.h"
#include "Texture\TextureManager.h"

#include "RenderableObject\RenderableObject.h"
#include "RenderableObject\RenderableObjectsManager.h"
#include "RenderableObject/RenderableObjectsLayersManager.h"
#include "Utils\Name.h"
#include "Math\Color.h"

#include "Object3D.h"

#include "Cameras/CameraManager.h"
#include "Cameras/Camera.h"
#include "Cameras/CameraInfo.h"

#include "Cinematics/Cinematic.h"
#include "Cinematics/CinematicPlayer.h"

#include "Lights\LightManager.h"
#include "Lights\OmniLight.h"

#include "PhysicsDefs.h"
#include "Math\Matrix44.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>
#include "StaticMeshes/InstanceMesh.h"

using namespace luabind;

void registerGraphics( lua_State* m_LS )
{
  module( m_LS )
  [
    class_<CObject3D>( "CObject3D" )
    .def( constructor<const Math::Vect3f&, float, float, float>() )
    .def( constructor<>() )
    .def( "GetYaw", &CObject3D::GetYaw )
    .def( "GetRoll", &CObject3D::GetRoll )
    .def( "GetPitch", &CObject3D::GetPitch )
    .def( "GetPosition", &CObject3D::GetPosition )
    .def( "GetTransform", &CObject3D::GetTransform )
    .def( "SetYaw", &CObject3D::SetYaw )
    .def( "SetRoll", &CObject3D::SetRoll )
    .def( "SetPitch", &CObject3D::SetPitch )
    .def( "SetPosition", &CObject3D::SetPosition )
    .def( "SetScale", &CObject3D::SetScale )
    .def( "MakeTransform", &CObject3D::MakeTransform )
  ];
  module( m_LS )
  [
    class_<CLight, CObject3D>( "CLight" )
    .def( "GetPosition", &CObject3D::GetPosition )
    .def( "SetPosition", &CObject3D::SetPosition )
  ];
  module( m_LS )
  [
    class_<CLightManager>( "CLightManager" )
    .def( constructor<>() )
    .def( "GetLight", &CLightManager::GetLight )
  ];
  module( m_LS )
  [
    class_<COmniLight, bases<CLight, CObject3D>>( "COmniLight" )
    .def( constructor<>() )
  ];
  module( m_LS )
  [
    class_<CTextureManager>( "CTextureManager" )
    .def( constructor<>() )
    .def( "GetTexture", &CTextureManager::GetTexture ) //TODO: Ask Jordi
    .def( "AddResource", &CTextureManager::AddResource )
    .def( "GetResource", &CTextureManager::GetResource )
    .def( "Reload", &CTextureManager::Reload )
  ];
  module( m_LS )
  [
    class_<CTexture>( "CTexture" )
    .def( constructor<>() )
    .def( "GetFileName", &CTexture::GetFileName )
    .def( "Load", &CTexture::Load )
    .def( "Reload", &CTexture::Reload )
    .def( "Activate" , &CTexture::Activate )
  ];
  module( m_LS )
  [
    class_<CRenderableObject, bases<CObject3D, CName>>( "CRenderableObject" )
  ];
  module( m_LS )
  [
    class_<CInstanceMesh, CRenderableObject>( "CInstanceMesh" )
    .def( constructor<const std::string&, const std::string&>() )
  ];
  module( m_LS )
  [
    class_<CTemplatedVectorMapManager<CRenderableObjectsManager>>( "CTemplatedVectorMapManagerCRenderableObjectManager" )
  ];
  module( m_LS )
  [
    class_<CRenderableObjectsLayersManager, CTemplatedVectorMapManager<CRenderableObjectsManager>>( "CRenderableObjectsManager" )
    .def( "GetResource", &CRenderableObjectsLayersManager::GetResource )
  ];
  module( m_LS )
  [
    class_<CTemplatedVectorMapManager<CRenderableObject>>( "CTemplatedVectorMapManagerCRenderableObject" )
  ];
  module( m_LS )
  [
    class_<CRenderableObjectsManager, CTemplatedVectorMapManager<CRenderableObject>>( "CRenderableObjectsManager" )
    .def( constructor<>() )
    .def( "Update", &CRenderableObjectsManager::Update )
    .def( "Render", &CRenderableObjectsManager::Render )
    /*.def("AddMeshInstance", &CRenderableObjectsManager::AddMeshInstance)
    .def("AddAnimatedInstanceModel", &CRenderableObjectsManager::AddAnimatedInstanceModel)*/
    .def( "CleanUp", &CRenderableObjectsManager::CleanUp )
    .def( "Load", &CRenderableObjectsManager::Load )
    .def( "AddResource", &CRenderableObjectsManager::AddResource )
    .def( "CreateCinematic", &CRenderableObjectsManager::CreateCinematic )
  ];
  module( m_LS )
  [
    class_<CGraphicsManager>( "CGraphicsManager" )
    .def( constructor<>() )
    .def( "BeginRender", &CGraphicsManager::BeginRender )
    .def( "DisableAlphaBlend", &CGraphicsManager::DisableAlphaBlend )
    .def( "DisableZBuffering", &CGraphicsManager::DisableZBuffering )
    .def( "DrawAxis", &CGraphicsManager::DrawAxis )
    .def( "DrawBox", &CGraphicsManager::DrawBox )
    //.def("DrawCamera", &CGraphicsManager::DrawCamera)
    .def( "DrawCapsule", &CGraphicsManager::DrawCapsule )
    .def( "DrawCircle", &CGraphicsManager::DrawCircle )
    .def( "DrawCube", ( void( CGraphicsManager::* )( float32 ) ) &CGraphicsManager::DrawCube )
    .def( "DrawCylinder", &CGraphicsManager::DrawCylinder )
    .def( "DrawGrid", &CGraphicsManager::DrawGrid )
    .def( "DrawIcoSphere", &CGraphicsManager::DrawIcoSphere )
    .def( "DrawLine", &CGraphicsManager::DrawLine )
    .def( "DrawPlane", &CGraphicsManager::DrawPlane )
    .def( "DrawQuad2D", &CGraphicsManager::DrawQuad2D )
    //.def("DrawQuad3D", &CGraphicsManager::DrawQuad3D)
    .def( "DrawRectangle2D", &CGraphicsManager::DrawRectangle2D )
    .def( "DrawSphere", &CGraphicsManager::DrawSphere )
    .def( "EnableAlphaBlend", &CGraphicsManager::EnableAlphaBlend )
    .def( "EnableZBuffering", &CGraphicsManager::EnableZBuffering )
    .def( "EndRender", &CGraphicsManager::EndRender )
    //.def("GetCurrentCamera", &CGraphicsManager::GetCurrentCamera)
    .def( "GetDevice", &CGraphicsManager::GetDevice )
    .def( "GetRay", &CGraphicsManager::GetRay )
    .def( "GetWidthAndHeight", &CGraphicsManager::GetWidthAndHeight )
    .def( "Init", &CGraphicsManager::Init )
    .def( "Release", &CGraphicsManager::Release )
    .def( "Render", &CGraphicsManager::Render )
    .def( "RenderCursor", &CGraphicsManager::RenderCursor )
    //.def( "SetTransform", ( void( CGraphicsManager::* )(const Math::Mat44f& ) ) &CGraphicsManager::SetTransform )
    .def( "SetupMatrices", &CGraphicsManager::SetupMatrices )
    .def( "Update", &CGraphicsManager::Update )
  ];
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  module( m_LS )
  [
    class_<CCameraManager>( "CCameraManager" )
    .def( constructor<>() )
    .def( "GetCamera", &CCameraManager::GetCamera )
    //.def("SetCurrentCamera", &CCameraManager::SetCurrentCamera) // Ask Jordi
    .def( "GetCurrentCamera", &CCameraManager::GetCurrentCamera )
    .def( "NewCamera", &CCameraManager::NewCamera )
    .def( "DeleteCamera", &CCameraManager::DeleteCamera )
  ];
  module( m_LS )
  [
    class_<CCamera, CName>( "CCamera" )
    .def( "GetDirection", &CCamera::GetDirection )
    .def( "GetLookAt", &CCamera::GetLookAt )
    .def( "GetVecUp", &CCamera::GetVecUp )
    .def( "GetPitch", &CCamera::GetPitch )
    .def( "GetYaw", &CCamera::GetYaw )
    .def( "GetCameraType", &CCamera::GetCameraType )
    .def( "GetZNear", &CCamera::GetZNear )
    .def( "GetZFar", &CCamera::GetZFar )
    .def( "GetAspectRatio", &CCamera::Getaspect_ratio )
    .def( "GetPos", &CCamera::GetPos )
    .def( "GetEnable", &CCamera::GetEnable )
    .def( "SetPitch", &CCamera::SetPitch )
    .def( "SetYaw", &CCamera::SetYaw )
    .def( "SetCameraType", &CCamera::SetCameraType )
    .def( "SetZNear", &CCamera::SetZNear )
    .def( "SetZFar", &CCamera::SetZFar )
    .def( "SetAspectRatio", &CCamera::Setaspect_ratio )
    .def( "SetPos", &CCamera::SetPos )
    .def( "SetEnable", &CCamera::SetEnable )
    .def( "AddYaw", &CCamera::AddYaw )
    .def( "AddPitch", &CCamera::AddPitch )
  ];

  module( m_LS )
  [
    class_<CCameraInfo>( "CCameraInfo" )
    .def( "SetLookAt", &CCameraInfo::SetLookAt )
  ];
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  module( m_LS )
  [
    class_<CCinematicPlayer>( "CCinematicPlayer" )
  ];
  module( m_LS )
  [
    class_<CCinematic, bases<CRenderableObject, CCinematicPlayer>>( "CCinematic" )
    .def( constructor<const std::string&>() )
    .def( "Stop", &CCinematic::Stop )
    .def( "Play", &CCinematic::Play )
    .def( "Pause", &CCinematic::Pause )
  ];
}