#pragma once
#ifndef INC_MAP_MANAGER_H_
#define INC_MAP_MANAGER_H_

#include <map>
#include <string.h>
#include "Logger\Logger.h"
#include "Utils\Defines.h"

template<class T>
class CMapManager
{
protected:
  typedef std::map<std::string, T*> TMapResource;
  TMapResource m_Resources;

public:
  virtual T* GetResource( const std::string& Name )
  {
    if ( m_Resources.find( Name ) == m_Resources.end() )
    {
      LOG_ERROR_APPLICATION( "CMapManager::GetResource->(%s)", Name.c_str() );
      return 0;
    }
    else
      return m_Resources[Name];
  }

  virtual bool AddResource( const std::string& Name, T* Resource )
  {
    if ( m_Resources.find( Name ) != m_Resources.end() )
    {
      //CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CMapManager::AddResource->(%s)", Name.c_str() );
      return false;
    }

    m_Resources[Name] = Resource;
    return true;
  }

  void Destroy()
  {
    std::map<std::string, T*>::iterator itb = m_Resources.begin(), ite = m_Resources.end();

    for ( ; itb != ite; ++itb )
      CHECKED_DELETE( itb->second );

    m_Resources.clear();
  }
  virtual TMapResource& GetResourcesMap()
  {
    return m_Resources;
  }
};

#endif //INC_MAP_MANAGER_H_