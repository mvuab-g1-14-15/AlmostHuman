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
   bool SetWorldMatrix( const Math::Mat44f& Matrix );
   bool SetViewMatrix( const Math::Mat44f& Matrix );
   bool SetProjectionMatrix( const Math::Mat44f& Matrix );
   bool SetInverseWorldMatrix( const Math::Mat44f& Matrix );
   bool SetInverseViewMatrix( const Math::Mat44f& Matrix );
   bool SetInverseProjectionMatrix( const Math::Mat44f& Matrix );
   bool SetWorldViewMatrix( const Math::Mat44f& Matrix );
   bool SetWorldViewProjectionMatrix( const Math::Mat44f& Matrix );
   bool SetViewProjectionMatrix( const Math::Mat44f& Matrix );
   bool SetAmbientLightColor( const Math::Vect3f &aAmbienLightColor );

private:
   DECLARE_EFFECT_PARAMETER( WorldMatrix );
   DECLARE_EFFECT_PARAMETER( ViewMatrix );
   DECLARE_EFFECT_PARAMETER( ProjectionMatrix );
   DECLARE_EFFECT_PARAMETER( InverseWorldMatrix );
   DECLARE_EFFECT_PARAMETER( InverseViewMatrix );
   DECLARE_EFFECT_PARAMETER( InverseProjectionMatrix );
   DECLARE_EFFECT_PARAMETER( WVMatrix );
   DECLARE_EFFECT_PARAMETER( VPMatrix );
   DECLARE_EFFECT_PARAMETER( WVPMatrix );
   DECLARE_EFFECT_PARAMETER( CameraPosition );
   DECLARE_EFFECT_PARAMETER( AmbientLight );
};

extern CSharedEffect SharedEffect;

#endif // SHARED_EFFECT_H