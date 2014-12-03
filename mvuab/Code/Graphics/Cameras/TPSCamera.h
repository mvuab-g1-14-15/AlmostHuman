#ifndef _TPS_CAMERA_H_
#define _TPS_CAMERA_H_
#pragma once

#include "Camera.h"

class CTPSCamera: public CCamera
{
public:

	CTPSCamera( Vect3f aEyePosition, Vect3f aLookAt, float32 aZNear, float32 aZFar, float32 aFOV, float32 aAspectRatio );
	CTPSCamera( Vect3f aEyePosition, Vect3f aLookAt, CObject3D* apAttachedObject );
	void AddZoom( float32 dz );
	virtual ~CTPSCamera(){}

protected:
	virtual void RecalculateCameraData();

	float32 m_Zoom;

};

#endif // INC_FPS_CAMERA_H_