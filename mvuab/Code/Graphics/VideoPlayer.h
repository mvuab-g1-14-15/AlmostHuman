#pragma once
#ifndef VIDEO_PLAYER_
#define VIDEO_PLAYER_

#include <Windows.h>
#include "Utils/AVIReader.h"
#include "EngineManagers.h"

#include "Texture/Texture.h"
#include "GraphicsManager.h"
#pragma comment (lib, "Winmm.lib")


class CVideoPlayer
{
    private:
        CAVIReader *m_pAviReader;
        CTexture m_Texture;

        float m_FrameTime;
        HANDLE m_pThreadHandle;

        bool m_StopPlay;
        bool m_PausePlay;

        void BeginThread(void);
        void EndThread(void);

    public:
        CVideoPlayer() : m_pThreadHandle(0), m_pAviReader(0), m_FrameTime(0), m_StopPlay(false), m_PausePlay(false)
        {
            m_pAviReader = new CAVIReader();
        }

        ~CVideoPlayer() 
        {
            if(m_pAviReader != NULL)
            {
                delete m_pAviReader;
                m_pAviReader = NULL;
            }
        };

        void Stop()
        {
            m_StopPlay = true;
        }

        void Pause(bool l_Play)
        {
            m_PausePlay = l_Play;
        }

        bool Play(char *l_File);
        void threadProc(void);
};

void WindowsThreadFunction(CVideoPlayer* theThread) ;

#endif //INC_CORE_H_