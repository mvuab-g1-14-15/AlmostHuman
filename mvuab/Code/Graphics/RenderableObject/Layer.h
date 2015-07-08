#pragma once
#ifndef LAYER_H
#define LAYER_H

#include <string>
#include "Utils/Name.h"

class CLayer : public CName
{
    public:
        CLayer   ();
        ~CLayer  ();

        bool Load(const std::string &l_FilePath);
};

#endif