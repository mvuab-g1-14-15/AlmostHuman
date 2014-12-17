//----------------------------------------------------------------------------------
// CObject3D class
// Author: Enric Vergara
//
// Description:
// Esta clase será la base de toda instancia que pueda existir en la escena
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_OBJECT_3D_H_
#define INC_OBJECT_3D_H_

#include "Math\MathTypes.h"
#include "Math\Vector3.h"

class CObject3D
{
public:
    CObject3D(const Vect3f& pos, float32 yaw, float32 pitch, float32 roll = 0, Vect3f scale = Vect3f(1.0f, 1.0f, 1.0f));
    CObject3D();
    virtual ~CObject3D(void) {/*Nothing*/;}

    //---Get Functions
    float32                        GetYaw            () const {return m_fYaw;}
    float32                        GetRoll            () const {return m_fRoll;}
    float32                        GetPitch        () const {return m_fPitch;}
    const Vect3f&        GetPosition    () const {return m_Position;}
	const Vect3f&		 GetScale		() const {return m_Scale;}

    //---Set Functions
    void        SetPosition    ( const Vect3f& pos )        {m_Position = pos;}
    void        SetYaw            ( float32 yaw )                        {m_fYaw = yaw;}
    void        SetPitch        ( float32 pitch )                    {m_fPitch = pitch;}
    void        SetRoll            ( float32 roll )                    {m_fRoll = roll;}
	void		SetScale			(const Vect3f& scale)	{m_Scale = scale;}

	Mat44f GetTransform();

protected:
    Vect3f    m_Position;
	Vect3f	  m_Scale;
    float32        m_fYaw;
    float32        m_fPitch;
    float32        m_fRoll;
};

#endif //INC_CORE_H_