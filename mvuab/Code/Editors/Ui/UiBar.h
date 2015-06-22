#ifndef UI_BAR_H
#define UI_BAR_H
#pragma once

#include "Utils\Defines.h"
#include "Utils\Name.h"
#include "XML/XMLTreeNode.h"
#include <AntTweakBar.h>

class CUiBar : public CName
{
    public:
        CUiBar( const std::string& aName );
        ~CUiBar();



        template<typename T> bool AddProperty( const char* aLabel, T& aPropery );
        template<typename T> bool AddProperty( const char* aLabel, T& aPropery, const T& aMinValue, const T& aMaxValue,
                                               const T& aStep );
        template<typename T> bool AddProperty( const char* aLabel, T& aPropery, const char* aGroup);
        template<typename T> bool AddProperty( const char* aLabel, T& aPropery, const T& aMinValue, const T& aMaxValue,
                                               const T& aStep, const char* aGroup );

        virtual bool Create() = 0;
    protected:

    private: // Members
        TwBar* mBar;
};

#endif // UI_BAR_H