#include "Room.h"
#include "Utils\Defines.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"

CRoom::CRoom()
	: m_Name( "" )
	, m_RenderableObjectsPath( "" )
	, m_StaticMeshesPath( "" )
	, m_pLayers( 0 )
	, m_Active( false )
{
}

CRoom::~CRoom() { 
	CHECKED_DELETE(m_pLayers);
}

