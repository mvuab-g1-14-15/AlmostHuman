#include "GUITexture.h"
#include "Utils\Defines.h"
#include "Logger\Logger.h"

CGUITexture::CGUITexture()
{

}

CGUITexture::~CGUITexture()
{

}

bool CGUITexture::LoadFile()
{
    CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CGUITexture::LoadFile" );
    return false;
}