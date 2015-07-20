#pragma once
#ifndef SHARED_EFFECT_H
#define SHARED_EFFECT_H

#include "Effects\Effect.h"

class CSharedEffect: public CEffect
{
    public:
        CSharedEffect();
        ~CSharedEffect();

        bool Load( CXMLTreeNode& EffectNode );

        virtual void Bind();
        virtual void SetNullParameters();
        virtual void LinkSemantics();

        bool SetCameraPosition( const Math::Vect3f &CameraPosition );
        bool SetViewMatrix( const Math::Mat44f& Matrix );
        bool SetProjectionMatrix( const Math::Mat44f& Matrix );
        bool SetInverseViewMatrix( const Math::Mat44f& Matrix );
        bool SetInverseProjectionMatrix( const Math::Mat44f& Matrix );
        bool SetViewProjectionMatrix( const Math::Mat44f& Matrix );
        bool SetAmbientLightColor( const Math::Vect3f &aAmbienLightColor );
        bool SetFBSize( const Math::Vect2u aSize );
        bool SetDeltaTime( const float dt );

    private:
        DECLARE_EFFECT_PARAMETER( ViewMatrix );
        DECLARE_EFFECT_PARAMETER( ProjectionMatrix );
        DECLARE_EFFECT_PARAMETER( InverseViewMatrix );
        DECLARE_EFFECT_PARAMETER( InverseProjectionMatrix );
        DECLARE_EFFECT_PARAMETER( VPMatrix );
        DECLARE_EFFECT_PARAMETER( CameraPosition );
        DECLARE_EFFECT_PARAMETER( AmbientLight );
        DECLARE_EFFECT_PARAMETER( FBWidth );
        DECLARE_EFFECT_PARAMETER( FBHeight );
        DECLARE_EFFECT_PARAMETER( DeltaTime );
};

#endif // SHARED_EFFECT_H