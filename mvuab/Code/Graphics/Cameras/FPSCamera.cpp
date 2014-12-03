#include "FPSCamera.h"
#include "Math\Matrix33.h"
#include "Math\Matrix44.h"
#include <d3dx9.h>

CFPSCamera::CFPSCamera(Vect3f aEyePosition, Vect3f aLookAt, float32 aZNear, float32 aZFar, float32 aFOV, float32 aAspectRatio)
: CCamera( aEyePosition, aLookAt, aZNear, aZFar, aFOV, aAspectRatio )
{}

CFPSCamera::CFPSCamera( Vect3f aEyePosition, Vect3f aLookAt, CObject3D* apAttachedObject )
: CCamera( aEyePosition, aLookAt, apAttachedObject )
{}

void CFPSCamera::RecalculateCameraData()
{
	Vect3f newVectDir( mathUtils::Cos(m_Yaw) * mathUtils::Cos(m_Pitch),
					   mathUtils::Sin(m_Pitch),
					   mathUtils::Cos(m_Pitch) * mathUtils::Sin(m_Yaw) );

	newVectDir.Normalize();
	float32 distanceEyeLookAt = m_EyePosition.Distance(m_LookAt);
	m_LookAt = m_EyePosition - ( newVectDir * distanceEyeLookAt );
}