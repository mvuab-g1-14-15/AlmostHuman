﻿#include "EditorsManager.h"
#include "EngineManagers.h"
#include "GraphicsManager.h"
#include "EngineConfig.h"
#include <AntTweakBar.h>
#include "Lights/LightManager.h"
#include "Ui/UiBar.h"
#include "Ui/UiLightsBar.h"

#include <iostream>

CEditorsManager::CEditorsManager( const CXMLTreeNode& atts )
    : CManager( atts )
{

}

CEditorsManager::~CEditorsManager()
{
    TwTerminate();
}

void CEditorsManager::Update()
{
}

void CEditorsManager::Render()
{
    // Draw tweak bars
    TwDraw();
}

void CEditorsManager::Init()
{
    return;
    TwInit(TW_DIRECT3D9, GraphicsInstance->GetDevice() );

    Math::Vect2i lScreenSize = CEngineConfig::GetSingleton().GetScreenSize();
    TwWindowSize(lScreenSize.x, lScreenSize.y );

    CUiLightsBar* lLightBar = new CUiLightsBar();

    bool lOk = lLightBar->Create();

    ASSERT(lOk, "Error creating light bar");

    //TwDefine(" TweakBar color='255 255 255' text=dark "); // Change TweakBar color and use dark text
}