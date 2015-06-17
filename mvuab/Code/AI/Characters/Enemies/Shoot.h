#ifndef _SHOOT_H
#define _SHOOT_H

#include "Object3D.h"
#include "Utils/Name.h"

class CBillboard;
class CLight;

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
    float mDamage;
    bool mImpacted;
    Math::Vect3f mDirection;

    CBillboard *mBillboard;
    CLight *mLight;
};

#endif

