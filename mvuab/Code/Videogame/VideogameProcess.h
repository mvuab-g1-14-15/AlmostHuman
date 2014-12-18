#ifndef VIDEOGAME_PROCESS_H
#define VIDEOGAME_PROCESS_H
#pragma once

#include "Process.h"
#include "Utils\Types.h"
#include "Math\Vector3.h"

class CVideogameProcess : public CProcess
{
private:
    float32 m_Speed;
    Math::Vect2i m_LastMousePoint;
    CCamera* m_pFPSCamera;
    CCamera* m_pTPSCamera;

public:
    CVideogameProcess();
    virtual ~CVideogameProcess();

    virtual void Init();
    virtual void Update(float32 deltaTime);
    virtual void Render();
};

#endif // GRAPHICS_MANAGER_H
