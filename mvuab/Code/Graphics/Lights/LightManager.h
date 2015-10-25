#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include "Utils/TemplatedVectorMapManager.h"
#include "Utils/Manager.h"
#include "LensFlare/LensFlare.h"
#include "Light.h"

#include <string>

class CGraphicsManager;

class CLightManager : public CTemplatedVectorMapManager<CLight>, public CManager
{
    public:
        CLightManager();
        CLightManager(CXMLTreeNode& atts);
        ~CLightManager();
        void    Init();
        bool    Load( const std::string& FileName );
        size_t  GetLightCount();
        CLight* GetLight( size_t at );
        void    Update() {}
        void    Render();
        void    GenerateShadowMap( CGraphicsManager* GM );
        bool    ReLoad();
        void    SetAmbientLight(const Math::Vect3f& );
        const Math::Vect3f& GetAmbientLight() const;
        Math::Vect3f& GetAmbientLight();
        void    SetActive( const std::string& aName, bool aActive );
        CLight* CreateLight( const std::string& aType, const std::string& aRoom );
    private:
        Math::Vect3f mAmbientLightColor;
        CTemplatedVectorMapManager<CLensFlare> mLensFlares;
};

inline void CLightManager::SetAmbientLight(const Math::Vect3f& aAmbientLight )
{
  mAmbientLightColor = aAmbientLight;
}

//-----------------------------------------------------------------------------
inline const Math::Vect3f& CLightManager::GetAmbientLight() const
{
    return mAmbientLightColor;
}

#endif // LIGHT_MANAGER_H
