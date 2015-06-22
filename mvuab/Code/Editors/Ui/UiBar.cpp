#include "UiBar.h"
#include "EngineManagers.h"
#include "GraphicsManager.h"
#include "Utils/StringUtils.h"

CUiBar::CUiBar( const std::string& aName )
    : CName(aName)
    , mBar( TwNewBar( aName.c_str() ) )
{

}

CUiBar::~CUiBar()
{
}

template<> bool CUiBar::AddProperty<float32>( const char* aLabel, float& aPropery, const float32& aMinValue,
        const float32& aMaxValue,
        const float32& aStep )
{
    bool lOk(true);

    if( aLabel)
    {
        std::string lOptions = "";
        StringUtils::Format(lOptions, " label='%s' min=%f max=%f step=%f", aLabel, aMinValue, aMaxValue, aStep );
        lOk = TwAddVarRW(mBar, aLabel, TW_TYPE_FLOAT, &aPropery, lOptions.c_str()) == 1;
        ASSERT(!lOk, "%s", TwGetLastError() );
    }
    else
    {
        lOk = false;
    }

    return lOk;
}