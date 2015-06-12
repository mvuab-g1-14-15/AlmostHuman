#ifndef LENS_FLARE_H
#define LENS_FLARE_H

#include "XML/XMLTreeNode.h"
#include "Utils\Name.h"
#include "Utils/Manager.h"
#include <vector>
#include "Utils/TemplatedVectorMapManager.h"

class CTexture;
class CEffectTechnique;

class CFlare
{
public:
	CFlare();
	~CFlare();

	bool Init(const CXMLTreeNode& node);

private:
	Math::Vect3f  mColor;
	CTexture*	  mTexture;
	float		  mPos;
	Math::Vect2f  mSize;
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

class CLensFlareManager: public CTemplatedVectorMapManager<CLensFlare>, public CManager
{
public:
	    CLensFlareManager(){}
        CLensFlareManager(const CXMLTreeNode &atts);
        virtual ~CLensFlareManager();
        
        virtual void Init();
        virtual void Update();
        virtual void Render();
};

#endif // LENS_FLARE_H
