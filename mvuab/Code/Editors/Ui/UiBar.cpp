#include "UiBar.h"
#include "EngineManagers.h"
#include "GraphicsManager.h"
#include "Utils/StringUtils.h"
#include "Math/Vector3.h"

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
        ASSERT(lOk, "%s", TwGetLastError() );
    }
    else
    {
        lOk = false;
    }

    return lOk;
}

template<> bool CUiBar::AddProperty<Math::Vect3f>
(
    const char* aLabel, Math::Vect3f& aPropery,
    const Math::Vect3f& aMinValue,
    const Math::Vect3f& aMaxValue,
    const Math::Vect3f& aStep
)
{
    bool lOk(true);

    if( aLabel)
    {
        std::string lOptions = "";
        StringUtils::Format(lOptions, " label='x' min=%f max=%f step=%f group=%s", aMinValue
                            .x, aMaxValue.x, aStep.x, aLabel );
        lOk = TwAddVarRW(mBar, "x", TW_TYPE_FLOAT, &aPropery.x, lOptions.c_str()) == 1;
        ASSERT(lOk, "%s", TwGetLastError() );
        StringUtils::Format(lOptions, " label='y' min=%f max=%f step=%f group=%s", aMinValue
                            .y, aMaxValue.y, aStep.y, aLabel );
        lOk = TwAddVarRW(mBar, "y", TW_TYPE_FLOAT, &aPropery.y, lOptions.c_str()) == 1;
        ASSERT(lOk, "%s", TwGetLastError() );
        StringUtils::Format(lOptions, " label='z' min=%f max=%f step=%f group=%s", aMinValue
                            .z, aMaxValue.z, aStep.z, aLabel );
        lOk = TwAddVarRW(mBar, "z", TW_TYPE_FLOAT, &aPropery.z, lOptions.c_str()) == 1;
        ASSERT(lOk, "%s", TwGetLastError() );
    }
    else
    {
        lOk = false;
    }

    return lOk;
}