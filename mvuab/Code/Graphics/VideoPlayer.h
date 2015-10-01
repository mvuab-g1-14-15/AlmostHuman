#pragma once
#ifndef VIDEO_PLAYER_
#define VIDEO_PLAYER_

#include <Windows.h>
#include "Utils/AVIReader.h"
#include "Texture/Texture.h"
#pragma comment (lib, "Winmm.lib")

class CVideoPlayer
{
    private:
        CAVIReader *m_pAviReader;
        CTexture m_Texture;
        float m_FrameTime;

        HANDLE m_pThreadHandle;

    public:
        CVideoPlayer(unsigned int l_FPS, char *l_File) : m_pThreadHandle(0), m_pAviReader(0)
        {
            m_FrameTime = (float) 1.0f / l_FPS;
            m_pAviReader = new CAVIReader();

            if(m_pAviReader->OpenFile(l_File))
            {
                AVIFILEINFO l_VideoInfo = m_pAviReader->GetFileInfo();
                CTexture::TFormatType l_TexFormat = CTexture::TFormatType::eRGB8;

                CTexture::TUsageType l_TexUsage = CTexture::TUsageType::eUsageDynamic;
                CTexture::TPoolType l_TexPool = CTexture::TPoolType::eDefaultPool;

                if(!m_Texture.Create("VideoPlayer", l_VideoInfo.dwWidth, l_VideoInfo.dwHeight, 1, l_TexUsage, l_TexPool, l_TexFormat, false))
                {
                    delete m_pAviReader;
                    m_pAviReader = NULL;
                }
            }
        }

        ~CVideoPlayer() 
        {
            if(m_pAviReader != NULL)
            {
                delete m_pAviReader;
                m_pAviReader = NULL;
            }
        };

        void Begin(void)
        {
            this->m_pThreadHandle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) WindowsThreadFunction, this, NULL, NULL);
        }

        void End(void)
        {
            TerminateThread(m_pThreadHandle, 0);
            CloseHandle(m_pThreadHandle);
        }

        void threadProc()
        {
            if(m_pAviReader == NULL)
            { End(); return; }

            int l_StartFrame = m_pAviReader->GetFirstFrame();
            int l_NumFrames = m_pAviReader->GetNumFrames();

            int l_FrameIndex = l_StartFrame;
            unsigned int l_LastTime = timeGetTime();
            AVIFILEINFO l_VideoInfo = m_pAviReader->GetFileInfo();
            
            while(l_FrameIndex < l_NumFrames)
            {
                unsigned int l_ActialTime = timeGetTime();
                float l_LoopTime = (l_ActialTime - l_LastTime) * 1000.0f;

                if(l_LastTime > m_FrameTime)
                {
                    char *f = m_pAviReader->GetBitmap(l_FrameIndex, l_StartFrame);
                    if(m_Texture.SetBitmap(f, l_VideoInfo.dwWidth, l_VideoInfo.dwHeight, 3))
                    {
                        //render
                    }

                    free(f);

                    l_FrameIndex++;
                    l_LastTime = l_ActialTime;
                }
            }

            End();
        }
};

void WindowsThreadFunction(CVideoPlayer* theThread) 
{
    theThread->threadProc();
}

#endif //INC_CORE_H_