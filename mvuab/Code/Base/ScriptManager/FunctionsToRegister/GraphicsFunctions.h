#ifndef GRAPHICSREGISTER_H
#define GRAPHICSREGISTER_H

#include "Texture\Texture.h"
#include "Texture\TextureManager.h"

#include "RenderableObject\RenderableObject.h"
#include "RenderableObject\RenderableObjectsManager.h"

#include "Object3D.h"

#include "Cameras/CameraManager.h"

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

using namespace luabind;


void registerGraphics(lua_State *m_LS)
{
	module(m_LS)
	[
		class_<CObject3D>("CObject3D")
			.def(constructor<const Math::Vect3f &, float, float, float>())
			.def(constructor<>())

			.def("GetYaw", &CObject3D::GetYaw)
			.def("GetRoll", &CObject3D::GetRoll)
			.def("GetPitch", &CObject3D::GetPitch)
			
			.def("GetPosition", &CObject3D::GetPosition)
			.def("GetTransform", &CObject3D::GetTransform)

			.def("SetYaw", &CObject3D::SetYaw)
			.def("SetRoll", &CObject3D::SetRoll)
			.def("SetPitch", &CObject3D::SetPitch)

			.def("SetPosition", &CObject3D::SetPosition)
	];

	module(m_LS)
	[
		class_<CTextureManager>("CTextureManager")
			.def(constructor<>())

			.def("GetTexture", &CTextureManager::GetTexture) //TODO: Ask Jordi
			.def("AddResource", &CTextureManager::AddResource)
			.def("GetResource", &CTextureManager::GetResource)
			.def("Reload", &CTextureManager::Reload)
	];

	module(m_LS)
	[
		class_<CTexture>("CTexture")
			.def(constructor<>())

			.def("GetFileName", &CTexture::GetFileName)
			.def("Load", &CTexture::Load)
			.def("Reload", &CTexture::Reload)
			.def("Activate"	, &CTexture::Activate)
	];

	module(m_LS)
	[
		class_<CRenderableObjectsManager>("CRenderableObjectsManager")
			.def(constructor<>())
			
			.def("Update", &CRenderableObjectsManager::Update)
			.def("Render", &CRenderableObjectsManager::Render)
			.def("AddMeshInstance", &CRenderableObjectsManager::AddMeshInstance)
			.def("AddAnimatedInstanceModel", &CRenderableObjectsManager::AddAnimatedInstanceModel)
			
			.def("CleanUp", &CRenderableObjectsManager::CleanUp)
			.def("Load", &CRenderableObjectsManager::Load)

	];

	module(m_LS)
	[
		class_<CGraphicsManager>("CGraphicsManager")
			.def(constructor<>())
			.def("BeginRender", &CGraphicsManager::BeginRender)
			.def("DisableAlphaBlend", &CGraphicsManager::DisableAlphaBlend)
			.def("DisableZBuffering", &CGraphicsManager::DisableZBuffering)
			.def("DrawAxis", &CGraphicsManager::DrawAxis)
			.def("DrawBox", &CGraphicsManager::DrawBox)
			//.def("DrawCamera", &CGraphicsManager::DrawCamera)
			.def("DrawCapsule", &CGraphicsManager::DrawCapsule)
			.def("DrawCircle", &CGraphicsManager::DrawCircle)
			.def("DrawCube", &CGraphicsManager::DrawCube)
			.def("DrawCylinder", &CGraphicsManager::DrawCylinder)
			.def("DrawGrid", &CGraphicsManager::DrawGrid)
			.def("DrawIcoSphere", &CGraphicsManager::DrawIcoSphere)
			.def("DrawLine", &CGraphicsManager::DrawLine)
			.def("DrawPlane", &CGraphicsManager::DrawPlane)
			.def("DrawQuad2D", &CGraphicsManager::DrawQuad2D)
			//.def("DrawQuad3D", &CGraphicsManager::DrawQuad3D)
			.def("DrawRectangle2D", &CGraphicsManager::DrawRectangle2D)
			.def("DrawSphere", &CGraphicsManager::DrawSphere)
			.def("EnableAlphaBlend", &CGraphicsManager::EnableAlphaBlend)
			.def("EnableZBuffering", &CGraphicsManager::EnableZBuffering)
			.def("EndRender", &CGraphicsManager::EndRender)
			//.def("GetCurrentCamera", &CGraphicsManager::GetCurrentCamera)
			.def("GetDevice", &CGraphicsManager::GetDevice)
			.def("GetRay", &CGraphicsManager::GetRay)
			.def("GetWidthAndHeight", &CGraphicsManager::GetWidthAndHeight)
			.def("Init", &CGraphicsManager::Init)
			.def("Release", &CGraphicsManager::Release)
			.def("Render", &CGraphicsManager::Render)
			.def("RenderCursor", &CGraphicsManager::RenderCursor)
			//.def("SetTransform", &CGraphicsManager::SetTransform)
			.def("SetupMatrices", &CGraphicsManager::SetupMatrices)
			.def("Update", &CGraphicsManager::Update)
	];


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    module(m_LS)
        [
            class_<CCameraManager>("CCameraManager")
            .def(constructor<>())

            .def("GetCamera", &CCameraManager::GetCamera)
            //.def("SetCurrentCamera", &CCameraManager::SetCurrentCamera) // Ask Jordi
            .def("GetCurrentCamera", &CCameraManager::GetCurrentCamera)
            .def("NewCamera", &CCameraManager::NewCamera)
            .def("DeleteCamera", &CCameraManager::DeleteCamera)
        ];
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}

#endif
