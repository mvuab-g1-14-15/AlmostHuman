#ifndef MANAGER_H
#define MANAGER_H
#pragma once

#include <string>
#include "Utils\SingletonPattern.h"


class CManager
{
public:
  CManager(){};
  virtual ~CManager(){};
  virtual void Init( const std::string& Path) = 0;
  virtual void Update() = 0;
  virtual void Render() = 0;
};

#endif // MANAGER_H
