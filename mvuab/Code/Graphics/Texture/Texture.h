#pragma once
#ifndef INC_TEXTURE_H_
#define INC_TEXTURE_H_

#include <d3dx9.h>
#include <string>
#include "Utils\Name.h"

class CTexture: public CName
{
    public:
        enum TPoolType
        {
            eDefaultPool = 0,
            eSystemMemPool,
            eTexturePoolCount
        };

        enum TUsageType
        {
            eUsageDynamic = 0,
            eUsageRenderTarget,
            eTextureUsageCount
        };

        enum TTextureType
        {
            TGA = 0,
            JPG,
            BMP,
            eTextureTypeCount
        };

        enum TFormatType
        {
            eRGBA8 = 0,
            eRGB8,
            eRGBX8,
            eR32F,
            eTextureFormatCount
        };

    protected:
        //Members
        LPDIRECT3DTEXTURE9 m_Texture;
        std::string m_FileName;
        uint32      m_Width;
        uint32      m_Height;
        uint32      mMips;
        TPoolType   mPoolType;
        TUsageType  mUsage;
        TFormatType mFormat;
        bool m_CreateDepthStencilSurface;

        IDirect3DSurface9* m_OldRenderTarget;
        IDirect3DSurface9* m_DepthStencilRenderTargetTexture;
        IDirect3DSurface9* m_OldDepthStencilRenderTarget;
        IDirect3DSurface9* m_RenderTargetTexture;

        //Methods
        virtual bool LoadFile();
        void Unload();

    public:
        CTexture();
        virtual ~CTexture();

        const std::string& GetFileName() const;

        LPDIRECT3DTEXTURE9  GetDXTexture() const
        {
            return m_Texture;
        }

        uint32  GetWidth() const
        {
            return m_Width;
        }

        uint32  GetHeight() const
        {
            return m_Height;
        }

        bool Load( const std::string& FileName );
        bool Save( const std::string& FileName );

        bool Reload();
        virtual void Activate( size_t StageId );
        bool Create( const std::string& Name, size_t Width, size_t Height,
                     size_t MipMaps, TUsageType UsageType, TPoolType PoolType,
                     TFormatType FormatType, bool CreateDepthStencilBuffer = true);
        bool Clone( CTexture** aTexture );

        void Deactivate( size_t Stage );
        bool SetAsRenderTarget( size_t IdStage = 0 );
        void UnsetAsRenderTarget( size_t IdStage = 0, bool UnsetDepthStencilBuffer = true );
        void CaptureFrameBuffer( size_t IdStage );
        CTexture::TFormatType GetFormatTypeFromString( const std::string& FormatType );
};

#endif //INC_TEXTURE_H_