#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <string>
#include "Utils\MapManager.h"
#include "Utils\SingletonPattern.h"

class CRoom;

class CScene : public CMapManager<CRoom>, public CSingleton<CScene>
{
    public:
        CScene   ();
        ~CScene  ();

        bool Load(const std::string &l_FilePath);
};

#endif