#ifndef STATIC_MESH_MANAGER_H
#define STATIC_MESH_MANAGER_H
#pragma once

#include "StaticMesh.h"
#include "Utils\MapManager.h"
#include "Utils/Manager.h"
#include "XML\XMLTreeNode.h"
#include <string>

class CStaticMesh;

class CStaticMeshManager : public CMapManager<CStaticMesh>, public CManager
{
    public:
        CStaticMeshManager();
		CStaticMeshManager( CXMLTreeNode& atts);
        ~ CStaticMeshManager ();

		virtual void Init();
		virtual void Update(){}
		virtual void Render(){}
        void Reload();
	
		const CStaticMesh* GetStaticMesh( const std::string & aName ) const;

};
#endif