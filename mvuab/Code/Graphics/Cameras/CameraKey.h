#ifndef INC_CAMERA_KEY_H_
#define INC_CAMERA_KEY_H_

#include "Utils\Defines.h"
#include "Utils\Types.h"
#include "CameraInfo.h"

class CCameraKey
{
public:
    CCameraKey(CCameraInfo &CameraInfo, float32 Time);
    GET_SET(CCameraInfo,CameraInfo);
    GET_SET(float32,Time);
protected:
    CCameraInfo m_CameraInfo;
    float32 m_Time;
};

#endif // INC_CAMERA_INFO_H_