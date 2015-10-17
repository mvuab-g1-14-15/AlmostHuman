#ifndef _SHOOT_H
#define _SHOOT_H

#include "Object3D.h"
#include "Utils/Name.h"

class CLight;
class CInstanceMesh;
class CBillboardInstance;

class CShoot : public CObject3D, public CName
{
public:
    CShoot();
    CShoot( float aSpeed, Math::Vect3f aDirection, Math::Vect3f aPosition, float aDamage );
    virtual ~CShoot();

    virtual bool Init();
    virtual void Update();
    virtual void Render();

    bool Impacted();

private:
    float mSpeed;
    CBillboardInstance* mInstance;
    float mDamage;
    bool mImpacted;
    Math::Vect3f mDirection;
    CLight *mLight;

    CInstanceMesh* mShoot;
    CInstanceMesh* mShootGlow;
};

#endif

