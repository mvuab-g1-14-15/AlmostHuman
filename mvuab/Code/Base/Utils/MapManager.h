#pragma once
#ifndef INC_MAP_MANAGER_H_
#define INC_MAP_MANAGER_H_

#include <map>
#include <string.h>

#include "./RWLock.h"
#include "Logger\Logger.h"
#include "Utils\Defines.h"

template<class T> class CMapManager
{
    protected:
        typedef std::map<std::string, T*> TMapResource;
        TMapResource m_Resources;
        CRWLock m_Lock;

    public:
        virtual T* GetResource(const std::string &Name)
        {
            TMapResource::iterator it;

            //m_Lock.ReadLock();
                it = m_Resources.find(Name);
                if (it == m_Resources.end())
                {
                    //m_Lock.ReadUnLock();
                    //LOG_WARNING_APPLICATION("CMapManager::GetResource->(%s)", Name.c_str());
                    return 0;
                }
            //m_Lock.ReadUnLock();

            return it->second;
        }

        virtual T* GetConstResource(const std::string &Name) const
        {
            TMapResource::const_iterator it;

            //m_Lock.ReadLock();
                it = m_Resources.find(Name);
                if (it == m_Resources.end())
                {
                    //m_Lock.ReadUnLock();
                    //LOG_WARNING_APPLICATION("CMapManager::GetResource->(%s)", Name.c_str());
                    return 0;
                }
            //m_Lock.ReadUnLock();

            return it->second;
        }

        virtual bool AddResource(const std::string& Name, T *Resource)
        {
            m_Lock.WriteLock();
                if (m_Resources.find(Name) != m_Resources.end())
                {
                    m_Lock.WriteUnLock();
                    LOG_WARNING_APPLICATION("CMapManager::AddResource->(%s)", Name.c_str());
                    return false;
                }

                m_Resources[Name] = Resource;
            m_Lock.WriteUnLock();

            return true;
        }

        bool Exist( const std::string& aName )
        {
            bool e = false;

            m_Lock.ReadLock();
                e = m_Resources.find(aName) != m_Resources.end();
            m_Lock.ReadUnLock();

            return e;
        }

        void Destroy()
        {
            TMapResource::iterator itb = m_Resources.begin(), ite = m_Resources.end();
            for (; itb != ite; ++itb)
            {
                CHECKED_DELETE(itb->second);
            }

            m_Resources.clear();
        }

        virtual TMapResource& GetResourcesMap()
        {
            return m_Resources;
        }
		void RemoveResource( const std::string& Name )
        {
            m_Lock.WriteLock();
                TMapResource::iterator it = m_Resources.find(Name);
                if ( it == m_Resources.end() ) return;

                CHECKED_DELETE(it->second);
			    m_Resources.erase(it);  
            m_Lock.WriteUnLock();
        }
};

#endif //INC_MAP_MANAGER_H_