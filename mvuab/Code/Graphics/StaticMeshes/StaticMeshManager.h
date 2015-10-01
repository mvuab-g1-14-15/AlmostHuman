#ifndef STATIC_MESH_MANAGER_H
#define STATIC_MESH_MANAGER_H
#pragma once

#include "StaticMesh.h"
#include "Utils\MapManager.h"
#include "Utils/Manager.h"
#include "XML\XMLTreeNode.h"
#include <string>
#include "Utils\TemplatedVectorMapManager.h"

class CMaterial;

class CStaticMeshManager : public CMapManager<CStaticMesh>, public CManager
{
private:
  CMapManager< CMaterial > mMaterials;
  void LoadMaterials(const std::string& aMaterialsPath );
    public:
        CStaticMeshManager();
		CStaticMeshManager( CXMLTreeNode& atts);
        ~ CStaticMeshManager ();

		void Init();
		void Update(){}
		void Render(){}

    CMaterial* GetMaterial( const std::string& aMaterialName ) { return mMaterials.GetResource( aMaterialName ); }

        bool threadMeshLoad(std::string &l_File, std::string &l_Name, unsigned int iD);
		void Load( std::string aFilePath, std::string aBasePath );
};

typedef struct 
{
    unsigned int iD;

    std::string FileName;
    std::string ResourceName;

    CStaticMeshManager *MeshManager;
} MESH_THREAD_INFO;

void runMeshLoad(MESH_THREAD_INFO *l_ThreadInfo);
#endif