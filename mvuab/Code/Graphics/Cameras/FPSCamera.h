#ifndef _FPS_CAMERA_H_
#define _FPS_CAMERA_H_
#pragma once

#include "Camera.h"

class CFPSCamera: public CCamera
{
public:

    CFPSCamera( Vect3f aEyePosition, Vect3f aLookAt, float32 aZNear, float32 aZFar, float32 aFOV, float32 aAspectRatio );
    CFPSCamera( Vect3f aEyePosition, Vect3f aLookAt, CObject3D* apAttachedObject );
    virtual ~CFPSCamera(){}

protected:
    virtual void RecalculateCameraData();

};

#endif // INC_FPS_CAMERA_H_