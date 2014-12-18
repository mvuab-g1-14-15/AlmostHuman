#include "ScriptManager.h"

#include <Texture\TextureManager.h>
#include <Texture\Texture.h>

#include <RenderableObject\RenderableObject.h>
#include <RenderableObject\RenderableObjectsManager.h>

#include "GraphicsManager.h"

#include <Math\Matrix44.h>
#include <Math\Vector3.h>
#include <Object3D.h>

#include <Utils\Name.h>
#include <Utils\Visible.h>

#include <assert.h>

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

using namespace luabind;


CScriptManager::CScriptManager()
{
}

CScriptManager::~CScriptManager()
{
	Destroy();
}

void CScriptManager::Initialize()
{
	m_LS = luaL_newstate();

	luaL_openlibs(m_LS);
	luabind::open(m_LS);

	RegisterLUAFunctions();
}

void CScriptManager::Destroy()
{
	lua_close(m_LS);
}

void CScriptManager::RunCode(const std::string &Code) const
{
	if(luaL_dostring(m_LS,Code.c_str()))
	{
		const char *l_Str = lua_tostring(m_LS, -1);
		assert(l_Str);
	}
}

void CScriptManager::RunFile(const std::string &FileName) const
{
	if(luaL_dofile(m_LS, FileName.c_str()))
	{
		const char *l_Str = lua_tostring(m_LS, -1);
		assert(l_Str);
	}
}

void CScriptManager::RegisterLUAFunctions()
{
	module(m_LS)
	[
		class_<Vect3f>("Vect3f")
			.def(constructor<float, float, float>())
			.def(constructor<const Vect3f &>())
			.def(constructor<float>())
			.def(constructor<>())


			.def(const_self + const_self)
			.def(const_self - const_self)

			.def(const_self * const_self)
			.def(const_self ^ const_self)

			.def(const_self * float())
			.def(const_self / float())

			.def_readwrite("x", &Vect3f::x)
			.def_readwrite("y", &Vect3f::y)
			.def_readwrite("z", &Vect3f::z)
	];

	module(m_LS)
	[
		class_<CObject3D>("CObject3D")
			.def(constructor<const Vect3f &, float, float, float>())
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

	/*module(m_LS)
	[
		class_<Mat44f>("Mat44f") //TODO: Ask Jordi
			.def(constructor<>())	
	];*/

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
		class_<CName>("CName")
			.def(constructor<>())
			
			.def("SetName", &CName::SetName)
			.def("GetName", &CName::GetName)
	];

	module(m_LS)
	[
		class_<CVisible>("CVisible")
			.def(constructor<>())
			
			.def("SetVisible", &CVisible::SetVisible)
			.def("GetVisible", &CVisible::GetVisible)
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
			.def("GetNumberOfVisibleObjects", &CGraphicsManager::GetNumberOfVisibleObjects)
			.def("GetRay", &CGraphicsManager::GetRay)
			.def("GetuHeight", &CGraphicsManager::GetuHeight)
			.def("GetuWidth", &CGraphicsManager::GetuWidth)
			.def("GetWidthAndHeight", &CGraphicsManager::GetWidthAndHeight)
			.def("Init", &CGraphicsManager::Init)
			.def("Release", &CGraphicsManager::Release)
			.def("Render", &CGraphicsManager::Render)
			.def("RenderCursor", &CGraphicsManager::RenderCursor)
			//.def("SetTransform", &CGraphicsManager::SetTransform)
			.def("SetuHeight", &CGraphicsManager::SetuHeight)
			.def("SetupMatrices", &CGraphicsManager::SetupMatrices)
			.def("SetuWidth", &CGraphicsManager::SetuWidth)
			.def("Update", &CGraphicsManager::Update)
	];
}