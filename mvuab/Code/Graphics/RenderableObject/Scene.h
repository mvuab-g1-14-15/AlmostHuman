#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <string>
#include "Utils\MapManager.h"
#include "Utils\SingletonPattern.h"

class CRoom;

class CSene : public CMapManager<CRoom>, public CSingleton<CSene>
{
    public:
        CSene   ();
        ~CSene  ();

        bool Load(const std::string &l_FilePath);
};

#endif