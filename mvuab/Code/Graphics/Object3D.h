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
#include "Math\Matrix44.h"
#include "Math\Vector3.h"

class CXMLTreeNode;

class CObject3D
{
    protected:
        Math::Mat44f   m_Transform;
        Math::Vect3f   m_Position;
        Math::Vect3f   m_Scale;

        float32       m_fYaw;
        float32       m_fPitch;
        float32       m_fRoll;
        bool          mIsVisible;
    public:
        CObject3D();
        CObject3D( const CXMLTreeNode& atts );
        CObject3D( const Math::Vect3f& pos, float32 yaw, float32 pitch, float32 roll = 0,
                   Math::Vect3f scale = Math::Vect3f( 1.0f, 1.0f, 1.0f ) );

        bool IsVisible() const { return mIsVisible; }
        void ChangeVisibility( bool aVisibility ) { mIsVisible = aVisibility;}

        virtual ~CObject3D( void )
        {
            /*Nothing*/;
        }

        bool Init( const CXMLTreeNode& atts );

        //---Get Functions
        float32 GetYaw() const
        {
            return m_fYaw;
        }
        float32 GetRoll() const
        {
            return m_fRoll;
        }
        float32 GetPitch() const
        {
            return m_fPitch;
        }

        const Math::Vect3f GetDirection() const;

        const Math::Vect3f& GetPosition() const
        {
            return m_Position;
        }
        const Math::Vect3f& GetScale() const
        {
            return m_Scale;
        }

        const Math::Mat44f& GetTransform()
        {
            return m_Transform;
        }

        //---Set Functions
        void SetPosition( const Math::Vect3f& pos )
        {
            m_Position = pos;
        }
        void SetYaw( float32 yaw )
        {
            m_fYaw = yaw;
        }
        void SetPitch( float32 pitch )
        {
            m_fPitch = pitch;
        }
        void SetRoll( float32 roll )
        {
            m_fRoll = roll;
        }
        void SetScale( const Math::Vect3f& scale )
        {
            m_Scale = scale;
        }
		void SetDirection( const Math::Vect3f& );
		void SetTransform( const Math::Mat44f& transform )
		{
			m_Position = transform.GetPos();
			m_fYaw = transform.GetYaw();
			m_fPitch = transform.GetPitch();
			m_fRoll = transform.GetRoll();
			m_Scale = transform.GetScale();

			m_Transform = transform;
		}

        void MakeTransform();
};

#endif //INC_CORE_H_