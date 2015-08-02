#pragma once
#ifndef ROOM_H
#define ROOM_H

#include "Utils/Name.h"
#include "Utils\MapManager.h"
#include <string>

class CRenderableObjectsLayersManager;
class CLightManager;

class CRoom
{
    public:
        CRoom();
        ~CRoom();

        GET_SET(std::string, Name);
        GET_SET(std::string, RenderableObjectsPath);
        GET_SET(std::string, StaticMeshesPath);
        GET_SET(std::string, BasePath);
        GET_SET(bool, Active);

        CRenderableObjectsLayersManager* GetLayers()
        {
            return m_pLayers;
        }

        void SetLayers( CRenderableObjectsLayersManager* aLayers )
        {
            m_pLayers = aLayers;
        }

        CLightManager* GetLights()
        {
            return m_pLights;
        }

        void SetLights( CLightManager* aLights)
        {
            m_pLights = aLights;
        }

    private:
        std::string m_Name;
        std::string m_RenderableObjectsPath;
        std::string m_StaticMeshesPath;
        std::string m_BasePath;
        CRenderableObjectsLayersManager* m_pLayers;
        CLightManager* m_pLights;
        bool m_Active;
};

#endif