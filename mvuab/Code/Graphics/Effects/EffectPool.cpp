#include "EffectPool.h"
#include "Effect.h"
#include "Utils/Defines.h"
#include "Utils/StringUtils.h"

namespace
{
    bool GetCustomMacros( const CXMLTreeNode & aEffectNode, std::vector<char*>& aNames, std::vector<char*>& aDescriptions,
                          std::vector<D3DXMACRO>& aDefines )
    {
        ASSERT( aEffectNode.IsOk(), "The node to extract the effect data is invalid");

        for ( int iMacro = 0, lMacrosCount = aEffectNode.GetNumChildren(); iMacro < lMacrosCount; iMacro++ )
        {
            CXMLTreeNode& l_CurrentSubNode = aEffectNode( iMacro );
            const std::string& l_TagName = l_CurrentSubNode.GetName();

            if ( l_TagName == "define" )
            {
                char* cstr_name = StringUtils::ToCharPtr( l_CurrentSubNode.GetAttribute<std::string>( "name", "no_name" ) );
                aNames.push_back( cstr_name );
                char* cstr_desc = StringUtils::ToCharPtr( l_CurrentSubNode.GetAttribute<std::string>( "description",
                                  "no_description" ) );
                aDescriptions.push_back( cstr_desc );
                D3DXMACRO macro = { cstr_name, cstr_desc };
                aDefines.push_back( macro );
            }
        }

        D3DXMACRO null = { NULL, NULL };
        aDefines.push_back( null );
        return true;
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------
CEffectPool::CEffectPool(void)
    : mD3DXEffectPool( 0 )
{
}

//-----------------------------------------------------------------------------------------------------------------------------------------
CEffectPool::~CEffectPool(void)
{
}

//-----------------------------------------------------------------------------------------------------------------------------------------
bool CEffectPool::Init()
{
    bool lOk( true );

    HRESULT lHR = D3DXCreateEffectPool(&mD3DXEffectPool);

    switch(lHR)
    {
        case D3DERR_INVALIDCALL:
            LOG_ERROR_APPLICATION("Error creating the effect pool:Invalid Arguments");
            lOk = false;
            return false;
            break;
        case E_FAIL:
            LOG_ERROR_APPLICATION("Error creating the effect pool:D3DXCreateEffectPool Failed");
            lOk = false;
            break;
        default:
            break;
    }

    return lOk;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
CEffect* CEffectPool::CreateEffect( const CXMLTreeNode& aEffectNode )
{
    //ASSERT( aEffectNode.IsOk(), "The node to extract the effect data is invalid");

    const std::string lEffectName = aEffectNode.GetAttribute<std::string>( "name", "invalid_name" );

    ASSERT( lEffectName != "invalid_name", "The effect has no name");

    // To avoid memory leaks
    std::vector<char*> lNames;
    std::vector<char*> lDescriptions;
    std::vector<D3DXMACRO> lDefines;
    GetCustomMacros( aEffectNode, lNames, lDescriptions, lDefines );

    CEffect * lNewEffect = new CEffect( lEffectName );



    return lNewEffect;
}
