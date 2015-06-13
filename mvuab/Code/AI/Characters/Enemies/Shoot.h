#ifndef _SHOOT_H
#define _SHOOT_H

#include "Object3D.h"
#include "Utils/Name.h"

class CBillboard;

class CShoot : public CObject3D, public CName
{
public:
    CShoot();
    CShoot( float, Math::Vect3f, Math::Vect3f, float );
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

    CBillboard* mBillboard;
};

#endif

