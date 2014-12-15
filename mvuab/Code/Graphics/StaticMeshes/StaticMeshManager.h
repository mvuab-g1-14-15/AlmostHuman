#ifndef STATIC_MESH_MANAGER_H
#define STATIC_MESH_MANAGER_H
#pragma once

#include "Utils\MapManager.h"
#include "StaticMesh.h"
#include "Utils/SingletonPattern.h"

#include <string>

class CStaticMeshManager : public CMapManager<CStaticMesh>, public CSingleton<CStaticMeshManager>
{
	protected:
		std::string m_FileName;

	public:
		CStaticMeshManager();
		~ CStaticMeshManager ();

		bool Load(const std::string &FileName);
		bool Reload();
    void Update( float32 deltaTime );
};
#endif