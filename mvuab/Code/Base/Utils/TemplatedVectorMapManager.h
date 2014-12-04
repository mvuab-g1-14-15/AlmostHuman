#pragma once

#ifndef _TEMPLATED_VECTOR_MAP_MANAGER_H
#define _TEMPLATED_VECTOR_MAP_MANAGER_H

#include <string>
#include <vector>
#include <map>
#include "Utils\Defines.h"

template <class T>
class CTemplatedVectorMapManager
{
public:
        class CMapResourceValue
        {
        public:
                T                       *m_Value;
                size_t          m_Id;

                CMapResourceValue(T *value, size_t id)
                        : m_Value(value)
                        , m_Id(id)
                {
                }

                CMapResourceValue()
                {
                }
        };

        typedef std::vector<T*>                                                                 TVectorResources;
        typedef std::map<std::string, CMapResourceValue>                TMapResources;

protected:
        TVectorResources                                m_ResourcesVector;
        TMapResources                                   m_ResourcesMap;

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
                size_t l_ID = 0;
                TMapResources::iterator it = m_ResourcesMap.find(Name);
                if (it != m_ResourcesMap.end())
                {
                        l_ID = m_ResourcesMap[Name].m_Id;
                        m_ResourcesMap.erase(it);
                        m_ResourcesVector.erase(l_ID);

                        for(uint16 i=static_cast<uint16>(l_ID); i<m_ResourcesVector.size(); ++i)
                        {
                                T* l_TElement = m_ResourcesVector[l_ID];
                                TMapResources::iterator l_ItMap = m_ResourcesMap.begin();
                                while(l_ItMap->second.m_Value != l_TElement)
                                {
                                        ++l_ItMap;
                                }

                                l_ItMap->second.m_Id = l_ID;
                        }
                }
        }

        virtual T* GetResourceById(size_t Id)
        {
                if( m_ResourcesVector.size() > Id )
                {
                        return m_ResourcesVector[Id];
                }

                return NULL;
        }

        virtual T* GetResource(const std::string &Name)
        {
                TMapResources::iterator it = m_ResourcesMap.find(Name);
                if (it != m_ResourcesMap.end())
                {
                        return it->second.m_Value;
                }
      
                return NULL;
        }
        virtual bool AddResource(const std::string &Name, T *Resource)
        {
                TMapResources::iterator it = m_ResourcesMap.find(Name);
                if (it == m_ResourcesMap.end())
                {
                        m_ResourcesVector.push_back(Resource);

                        CMapResourceValue l_Resource(Resource, m_ResourcesVector.size());
                        m_ResourcesMap[Name] = l_Resource;

                        return true;
                }

                return false;
        }

        virtual void Destroy()
        {
                m_ResourcesMap.clear();

                for ( uint16 i = 0; i < m_ResourcesVector.size() ; ++i )
                {
                        CHECKED_DELETE( m_ResourcesVector[i] );
                }
                m_ResourcesVector.clear();
        }

        TMapResources&          GetResourcesMap()                       { return m_ResourcesMap; }
        TVectorResources&       GetResourcesVector()            { return m_ResourcesVector; }
};

#endif