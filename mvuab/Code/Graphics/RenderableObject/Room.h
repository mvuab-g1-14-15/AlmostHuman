#pragma once
#ifndef ROOM_H
#define ROOM_H

#include "Utils/Name.h"
#include <string>

class CRoom : public CName
{
    public:
        CRoom   ();
        ~CRoom  ();

        bool Load(const std::string &l_FilePath, const std::string &l_RoomName);
};

#endif