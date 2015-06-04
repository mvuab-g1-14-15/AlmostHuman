#ifndef LENS_FLARE_H
#define LENS_FLARE_H

#include "XML/XMLTreeNode.h"
#include "Utils\Name.h"
#include <vector>

class CTexture;
class CEffectTechnique;

class CFlare
{
public:
	CFlare();
	~CFlare();

	bool Init(const CXMLTreeNode& node);

private:
	Math::Vect3f mColor;
	CTexture*	  mTexture;
	float		  mPos;
};

class CLensFlare : public CName
{
    public:
        CLensFlare();
        ~CLensFlare();

		bool Init(const CXMLTreeNode& node );

    protected:
        std::vector<CFlare*> mFlares;
		CEffectTechnique*    mTechnique;
};

#endif // LENS_FLARE_H
