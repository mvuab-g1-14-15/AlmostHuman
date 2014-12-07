#ifndef _CAMERA_H_
#define _CAMERA_H_
#pragma once

#include "Utils\Defines.h"
#include "Math\Vector3.h"
#include "Math\MathTypes.h"
#include <string>

static const float32 PITCH_LIM_UP = (-ehalfPIf + 0.8f);
static const float32 PITCH_LIM_DOWN = (ehalfPIf - 0.35f);

class CObject3D;

class CCamera
{
public:

    CCamera( Vect3f aEyePosition, Vect3f aLookAt, float32 aZNear, float32 aZFar, float32 aFOV, float32 aAspectRatio, std::string aType );
    CCamera( Vect3f aEyePosition, Vect3f aLookAt, CObject3D* apAttachedObject, std::string aType );
    virtual ~CCamera();
    
    Vect3f GetDirection() const;
    Vect3f GetVecUp() const;
    Vect3f GetVecSide() const;
    void AddYawPitch( float32 dx, float32 dy );
    void Update(float32 aElapsedTime);
    
    enum CameraMovementDirecction{ UP = 0, DOWN, FORWARD, BACKWARD, LEFT, RIGHT };
    void Move( CameraMovementDirecction aMovementDir, float32 speed );
    
    // Getters and setters
    GET_SET(float32, FOV);
    GET_SET(float32, AspectRatio);
    GET_SET(float32, ZNear);
    GET_SET(float32, ZFar);
    GET_SET(float32, Yaw);
    GET_SET(float32, Pitch);
    GET_SET(float32, Roll);
    GET_SET_REF(Vect3f, EyePosition);
    GET_SET_REF(Vect3f, LookAt);
    GET_SET_REF(std::string, TypeStr);
    GET_SET_PTR(CObject3D, AttachedObject);

protected:
    float32             m_FOV;
    float32             m_AspectRatio;
    float32             m_ZNear;
    float32             m_ZFar;
    float32             m_Yaw;
    float32             m_Pitch;
    float32             m_Roll;
    Vect3f              m_EyePosition;
    Vect3f              m_LookAt;
    std::string         m_TypeStr;
    CObject3D*          m_pAttachedObject;

    virtual void RecalculateCameraData() = 0;
    void InitYawAndPitch();
};

#endif // _CAMERA_H_