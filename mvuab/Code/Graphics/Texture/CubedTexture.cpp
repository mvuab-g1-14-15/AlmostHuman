#include "CubedTexture.h"
#include "Utils\Defines.h"
#include "Logger\Logger.h"

CCubedTexture::CCubedTexture()
{

}

CCubedTexture::~CCubedTexture()
{

}

bool CCubedTexture::LoadFile()
{
	CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CCubedTexture::LoadFile" );
	return false;
}