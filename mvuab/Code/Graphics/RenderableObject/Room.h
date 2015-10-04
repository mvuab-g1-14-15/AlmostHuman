#pragma once
#ifndef ROOM_H
#define ROOM_H

#include "Utils/Name.h"
#include "XML/XMLTreeNode.h"
#include "Math/Vector3.h"
#include <string>

class CRenderableObjectsLayersManager;
class CRenderableObjectsManager;
class CLightManager;
class CLightProbe;

class CRoom : public CName
{
    public:
        CRoom( const CXMLTreeNode& aNode );
        ~CRoom();

		void Update();
		void LoadLightProbe();
		std::vector<CLightProbe*> GetClosedLightProbes(Math::Vect3f);
		void Load();
		void Unload();
		void LoadMeshes();
		void LoadInstances();
		void LoadLights();
		void RenderLayer( const std::string& aLayerName );
		bool IsActive() const { return m_Active; }
		void Activate() { m_Active = true; }
		void Deactivate() { m_Active = false; }
		bool IsLoaded() const { return mIsLoaded; }
		CRenderableObjectsManager* GetLayer( const std::string& aLayer ) const;

        GET_SET(std::string, BasePath);

    private:
        std::string m_RenderableObjectsPath;
        std::string m_StaticMeshesPath;
        std::string m_BasePath;
        CRenderableObjectsLayersManager* m_pLayers;
        CLightManager* m_pLights;
        bool m_Active;
		bool mIsLoaded;

		std::vector<CLightProbe*> mLightProbes;
};

#endif