#ifndef STATIC_MESH_MANAGER_H
#define STATIC_MESH_MANAGER_H
#pragma once

#include "Utils\MapManager.h"
#include "StaticMesh.h"

#include <string>

class CStaticMeshManager : public CMapManager<CStaticMesh>
{
	protected:
		std::string m_FileName;

	public:
		CStaticMeshManager();
		~ CStaticMeshManager ();

		bool Load(const std::string &FileName);
		bool Reload();
};
#endif