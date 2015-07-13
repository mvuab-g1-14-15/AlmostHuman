#pragma once
#ifndef ROOM_H
#define ROOM_H

#include "Utils/Name.h"
#include "Utils\MapManager.h"
#include <string>

class CRenderableObjectsLayersManager;

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
	
private:
	std::string m_Name;
	std::string m_RenderableObjectsPath;
	std::string m_StaticMeshesPath;
	std::string m_BasePath;
	CRenderableObjectsLayersManager* m_pLayers;
	bool m_Active;
};

/*
class CRoom : public CName, public CMapManager<CLayer>
{
    public:
        CRoom   ();
        ~CRoom  ();

        bool Load(const std::string &l_FilePath, const std::string &l_RoomName);
};
*/
#endif