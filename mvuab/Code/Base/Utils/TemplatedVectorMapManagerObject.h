#pragma once

#ifndef _TEMPLATED_VECTOR_MAP_MANAGER_OBJ_H
#define _TEMPLATED_VECTOR_MAP_MANAGER_OBJ_H

#include <string>
#include <vector>
#include <map>

#include "Utils\Defines.h"

template <class T> class CTemplatedVectorMapManagerObject
{
    public:
        class CMapResourceValue
        {
            public:
                T      m_Value;
                size_t m_Id;

                CMapResourceValue(T value, size_t id) : m_Value(value), m_Id(id) { }
                CMapResourceValue() : m_Value(0), m_Id(0) { }
        };

        typedef std::vector<T>                          TVectorResources;
        typedef std::map<std::string, CMapResourceValue> TMapResources;

    protected:
        TVectorResources    m_ResourcesVector;
        TMapResources       m_ResourcesMap;

    public:
        CTemplatedVectorMapManagerObject() { }
        virtual ~CTemplatedVectorMapManagerObject()
        {
            Destroy();
        }

        bool Exist( const std::string & Name )
        {
            return m_ResourcesMap.find( Name ) != m_ResourcesMap.end();
        }

        void RemoveResource( const std::string& Name )
        {
          TMapResources::iterator it = m_ResourcesMap.find(Name);
          if ( it == m_ResourcesMap.end() ) return;

          CHECKED_DELETE(it->second.m_Value);
          size_t l_ID = it->second.m_Id;

          m_ResourcesMap.erase(it);
          m_ResourcesVector.erase(m_ResourcesVector.begin() + l_ID);

          for (size_t i =  l_ID; i < m_ResourcesVector.size();  ++i)
          {
            T* l_TElement = m_ResourcesVector[i];
            TMapResources::iterator l_ItMap = m_ResourcesMap.begin();

            while (l_ItMap->second.m_Value != l_TElement) ++l_ItMap;
            l_ItMap->second.m_Id = i;
          }
        }

        virtual T GetResourceById(size_t Id)
        {
            return (m_ResourcesVector.size() > Id) ? m_ResourcesVector[Id] : 0;
        }

        virtual T GetConstResourceById(size_t Id)
        {
          return (m_ResourcesVector.size() > Id) ? m_ResourcesVector[Id] : 0;
        }

        virtual T GetResource(const std::string& Name)
        {
            TMapResources::iterator it;

            it = m_ResourcesMap.find(Name);
            if(it == m_ResourcesMap.end())
            {
              return 0;
            }

            return it->second.m_Value;
        }

        virtual bool AddResource( const std::string& Name, T Resource )
        {
            if( m_ResourcesMap.end() !=  m_ResourcesMap.find(Name))
            {
              return false;
            }

            CMapResourceValue l_Resource(Resource, m_ResourcesVector.size());
            m_ResourcesVector.push_back(Resource);
            m_ResourcesMap[Name] = l_Resource;

            return true;
        }

        virtual void Destroy()
        {
            for ( size_t i = 0; i < m_ResourcesVector.size() ; ++i ) CHECKED_RELEASE( m_ResourcesVector[i] );

            m_ResourcesMap.clear();
            m_ResourcesVector.clear();
        }

        TMapResources &GetResourcesMap()
        {
            return m_ResourcesMap;
        }

        TVectorResources &GetResourcesVector()
        {
            return m_ResourcesVector;
        }

        void Clear()
        {
            m_ResourcesVector.clear();
            m_ResourcesMap.clear();
        }

        const uint32 GetResourcesCount()
        {
            uint32 l_Size = 0;

            ASSERT( m_ResourcesMap.size() == m_ResourcesVector.size(), "map size != vector size" );
            l_Size = m_ResourcesVector.size();

            return l_Size;
        }
};

#endif