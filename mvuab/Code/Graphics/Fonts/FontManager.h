//----------------------------------------------------------------------------------
// CFontManager class
// Author: Enric Vergara
//
// Description:
// Esta clase se encargará de todo lo relacionado con fuentes y pintar texto
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_FONT_MANAGER_H_
#define INC_FONT_MANAGER_H_

#include "Math\MathTypes.h"
#include "Math\Color.h"
#include <vector>
#include <map>
#include "Utils\Manager.h"
#include "Utils\TemplatedVectorMapManagerObject.h"

#include "dx9.h"
//---Forward Declarations--
class CGraphicsManager;
//-------------------------

class CFontManager : public CManager
{
    public:
        //--- Init and End protocols------------------------------------------
        CFontManager(): CManager() {}
        CFontManager( CXMLTreeNode& atts);
        virtual ~CFontManager()
        {
            Release();
        }

        void Init();
        void Update() {}
        void Render() {}
        bool ReloadTTFs();
        bool LoadTTFs( const std::string& pathFile );
        int32 GetTTF_Id( const std::string& name );
        uint32 AddFont( uint32 size, bool bold, bool italica, const std::string& fontName, const std::string& fontFile, bool replaceDefault = false );
        uint32 DrawDefaultText( uint32 x, uint32 y, Math::CColor color, const char* format, ... );
        uint32 DrawText( uint32 x, uint32 y, Math::CColor color, uint32 idFont, const char* format, ... );
        uint32 DrawLiteral( uint32 x, uint32 y, const std::string& lit );
        uint32 SizeX( const char* format, uint32 idFont = 0 );
        uint32 SizeY( const char* format, uint32 idFont = 0 );

    private:
        void Release();

    private:
        LPD3DXFONT mDefaultFont;
        CTemplatedVectorMapManagerObject<LPD3DXFONT>    m_Fonts;
        std::map<std::string, uint32>             mTTFs;
        LPDIRECT3DDEVICE9                         m_pD3DDevice;                                // direct3d device
};

#endif //INC_FONT_MANAGER_H_