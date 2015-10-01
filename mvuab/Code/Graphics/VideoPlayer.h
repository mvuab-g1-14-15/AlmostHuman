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

        void BeginThread(void)
        {
            this->m_pThreadHandle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) WindowsThreadFunction, this, NULL, NULL);
        }

        void EndThread(void)
        {
            TerminateThread(m_pThreadHandle, 0);
            CloseHandle(m_pThreadHandle);
        }

        void threadProc()
        {
            if(m_pAviReader == NULL)
            { EndThread(); return; }

            int l_StartFrame = m_pAviReader->GetFirstFrame();
            int l_NumFrames = m_pAviReader->GetNumFrames();

            int l_FrameIndex = l_StartFrame;
            unsigned int l_LastTime = timeGetTime();
            AVIFILEINFO l_VideoInfo = m_pAviReader->GetFileInfo();

            while(l_FrameIndex < l_NumFrames && !m_StopPlay)
            {
                while(m_PausePlay && !m_StopPlay);

                unsigned int l_ActialTime = timeGetTime();
                float l_LoopTime = (l_ActialTime - l_LastTime) * 1000.0f;

                if(l_LastTime > m_FrameTime)
                {
                    char *f = m_pAviReader->GetBitmap(l_FrameIndex, l_StartFrame);
                    if(m_Texture.SetBitmap(f, l_VideoInfo.dwWidth, l_VideoInfo.dwHeight, 3))
                    {
                        uint32 width, height;
                        CEngineManagers::GetSingletonPtr()->GetGraphicsManager()->GetWidthAndHeight( width, height );

                        RECT l_Rect = { 0, 0, width, height };
                        Math::CColor l_Color = Math::colBLACK;
                        CEngineManagers::GetSingletonPtr()->GetGraphicsManager()->DrawColoredQuad2DTexturedInPixels(l_Rect, l_Color, &m_Texture, 0.0f, 0.0f, 1.0f, 1.0f);
                    }

                    free(f);
                    l_FrameIndex++;
                    l_LastTime = l_ActialTime;
                }
            }

            EndThread();
        }

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

        bool Play(char *l_File)
        {
            if(!m_pAviReader->OpenFile(l_File)) return false;
            m_FrameTime = 24.0f / 1.0f;
            m_StopPlay = false;

            AVIFILEINFO l_VideoInfo = m_pAviReader->GetFileInfo();
            CTexture::TFormatType l_TexFormat = CTexture::TFormatType::eRGB8;
            
            CTexture::TUsageType l_TexUsage = CTexture::TUsageType::eUsageDynamic;
            CTexture::TPoolType l_TexPool = CTexture::TPoolType::eDefaultPool;
            
            if(!m_Texture.Create("VideoPlayer", l_VideoInfo.dwWidth, l_VideoInfo.dwHeight, 1, l_TexUsage, l_TexPool, l_TexFormat, false)) return false;
            BeginThread(); return true;
        }
};

void WindowsThreadFunction(CVideoPlayer* theThread) 
{
    theThread->threadProc();
}

#endif //INC_CORE_H_