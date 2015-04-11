#ifndef MANAGER_H
#define MANAGER_H
#pragma once

#include <string>
#include "Utils\SingletonPattern.h"
#include "XML/XMLTreeNode.h"


class CManager
{
public:
  CManager(){}
  CManager( const CXMLTreeNode &atts) : mConfigPath(atts.GetPszProperty( "file_config", "" )) {}
  virtual ~CManager(){}
  virtual void Init() = 0;
  virtual void Update() = 0;
  virtual void Render() = 0;
protected:
	std::string mConfigPath;
};

#endif // MANAGER_H
