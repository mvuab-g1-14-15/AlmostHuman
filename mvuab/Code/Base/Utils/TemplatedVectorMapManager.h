#pragma once
#ifndef INC_MAP_MANAGER_H_
#define INC_MAP_MANAGER_H_

#include <map>
#include <vector>
#include <string.h>
#include "Utils\Types.h"
#include "Utils\SingletonPattern.h"
#include "Logger\Logger.h"
#include "Utils\Defines.h"

template<class T>
class CTemplatedVectorMapManager
{
public:
	class CMapResourceValue
	{
	public:
		T *m_Value;
		size_t m_Id;
		CMapResourceValue(T *Value, size_t Id) : m_Value(Value) , m_Id(Id)
		{
		}
	};

	typedef std::vector<T *> TVectorResources;
	typedef std::map<std::string, CMapResourceValue> TMapResources;
protected:
	TVectorResources m_ResourcesVector;
	TMapResources m_ResourcesMap;

public:
	CTemplatedVectorMapManager()
	{
	}

	virtual ~CTemplatedVectorMapManager()
	{
		Destroy();
	}

	void RemoveResource(const std::string &Name)
	{
		TMapResources::iterator it = m_ResourcesMap.find(Name);
		if( it != m_Resources.end() )
		{
			// Obtain the resource from the map
			CMapResourceValue &Resource = m_ResourcesMap[Name];

			// Change all the id that are upper than the deleted
			std::map<std::string, T*>::iterator itb = m_ResourcesMap.begin(), ite = m_ResourcesMap.end();
			for(; itb != ite; ++itb)
			{
				CMapResourceValue &currentResource = itb->second;
				if( currentResource.m_Id > Resource.m_Id )
				{
					--currentResource.m_Id;
				}
			}
			
			m_ResourcesVector.erase(m_ResourcesVector.begin()+Resource.m_Id);
			m_ResourcesMap.erase( it );
		}
		else
		{
			CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CTemplatedVectorMapManager::RemoveResource->(%s)", Name.c_str() );
		}
	}

	virtual T * GetResourceById(size_t Id)
	{
		return m_ResourcesMap.size() > Id ? m_ResourcesMap[Id] : 0;
	}

	virtual T * GetResource(const std::string &Name)
	{
		if( m_ResourcesMap.find(Name) != m_Resources.end() )
		{
			CMapResourceValue Resource = m_ResourcesMap[Name];
			return Resource.m_Value;
		}
		else
		{
			CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CTemplatedVectorMapManager::GetResource->(%s)", Name.c_str() );
			return 0;
		}
	}

	virtual bool AddResource(const std::string &Name, T *Resource)
	{
		if( m_ResourcesMap.find(Name) == m_Resources.end() )
		{
			size_t Id = m_ResourcesVector.size();
			CMapResourceValue newResource(Resource, Id);
			m_ResourcesMap[Name] = newResource;
			m_ResourcesVector.push_back( newResource );
		}
		else
		{
			CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CTemplatedVectorMapManager::AddResource->(%s)", Name.c_str() );
		}
	}

	virtual void Destroy()
	{
		for( uint32 i = 0; i < m_ResourcesVector.size(); ++i )
		{
			CHECKED_DELETE(m_ResourcesVector[i]);
		}
		m_ResourcesVector.clear();
		m_ResourcesMap.clear();
	}

	TMapResources & GetResourcesMap()
	{
		return m_ResourcesMap;
	}

	TVectorResources & GetResourcesVector()
	{
		return m_ResourcesVector;
	}
};

#endif //INC_MAP_MANAGER_H_