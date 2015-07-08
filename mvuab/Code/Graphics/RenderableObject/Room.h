#pragma once
#ifndef ROOM_H
#define ROOM_H

#include "Utils/Name.h"
#include "Utils\MapManager.h"
#include <string>

class CLayer;

class CRoom : public CName, public CMapManager<CLayer>
{
    public:
        CRoom   ();
        ~CRoom  ();

        bool Load(const std::string &l_FilePath, const std::string &l_RoomName);
};

#endif