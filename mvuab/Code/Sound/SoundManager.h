#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#pragma once

#include <vector>
#include <string>
#include "Utils\SingletonPattern.h"
#include "Utils\Types.h"

class CSoundManager : public CSingleton<CSoundManager>
{
private:
public:
    CSoundManager();
    ~CSoundManager();

    void Update();
    void Render();
};

#endif // SOUND_MANAGER_H
