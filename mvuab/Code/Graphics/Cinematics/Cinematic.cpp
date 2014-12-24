#include "Cinematic.h"
#include "XML\XMLTreeNode.h"
#include "Math\Vector3.h"
#include "Logger\Logger.h"
#include "CinematicObject.h"

CCinematic::CCinematic(const std::string &FileName)
{
	CXMLTreeNode newFile;

	if (!newFile.LoadFile(FileName.c_str()))
	{
		CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR,"CCinematic::Constructor --> Error loading XML %s.",FileName.c_str());
		return;
	}

	CXMLTreeNode cinematic = newFile["cinematic"];
	if (!cinematic.Exists())
	{
		CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR,"CCinematic::Constructor --> Error reading %s, cinematic no existeix.",FileName.c_str());
		return;
	}
	m_Name = cinematic.GetPszProperty("name", "");
	m_Duration = cinematic.GetFloatProperty("duration", 0.0f);

	for(int i=0;i<cinematic.GetNumChildren();++i)
	{
		m_CinematicObjects.push_back(new CCinematicObject(cinematic(i)));
	}
}

void CCinematic::LoadXML(const std::string &FileName)
{
	CXMLTreeNode newFile;

	if (!newFile.LoadFile(FileName.c_str()))
	{
		CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR,"CCinematic::Constructor --> Error loading XML %s.",FileName.c_str());
		return;
	}

	CXMLTreeNode cinematic = newFile["cinematic"];
	if (!cinematic.Exists())
	{
		CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR,"CCinematic::Constructor --> Error reading %s, cinematic no existeix.",FileName.c_str());
		return;
	}
	m_Name = cinematic.GetPszProperty("name", "");
	m_Duration = cinematic.GetFloatProperty("duration", 0.0f);

	for(int i=0;i<cinematic.GetNumChildren();++i)
	{
		std::string resource = cinematic(i).GetPszProperty("resource", "");

		m_CinematicObjects.push_back(new CCinematicObject(cinematic(i)));
	}
}

void CCinematic::AddCinematicObject(CCinematicObject *CinematicObject)
{
	m_CinematicObjects.push_back(CinematicObject);
}

void CCinematic::Update(float ElapsedTime)
{

	std::vector<CCinematicObject *>::iterator it = m_CinematicObjects.begin(), it_end = m_CinematicObjects.end();
	for(; it != it_end; ++it)
		(*it)->Update(ElapsedTime);
}