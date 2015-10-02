#include "VideoPlayer.h"


void CVideoPlayer::BeginThread(void)
{
    m_pThreadHandle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) WindowsThreadFunction, this, NULL, NULL);
}

void CVideoPlayer::EndThread(void)
{
    TerminateThread(m_pThreadHandle, 0);
    CloseHandle(m_pThreadHandle);
}

void CVideoPlayer::threadProc()
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

        if(l_LoopTime > m_FrameTime)
        {
            LPBITMAPINFOHEADER l_Header = (LPBITMAPINFOHEADER) m_pAviReader->GetBitmap(l_FrameIndex, l_StartFrame);
            char *l_pData = (char *) (l_Header + l_Header->biSize + l_Header->biClrUsed * sizeof(RGBQUAD));

            if(m_Texture.SetBitmap(l_pData, l_VideoInfo.dwWidth, l_VideoInfo.dwHeight, l_Header->biBitCount == 24 ? 3 : 4))
            {
                uint32 width, height;
                CEngineManagers::GetSingletonPtr()->GetGraphicsManager()->GetWidthAndHeight( width, height );

                RECT l_Rect = { 0, 0, width, height };
                Math::CColor l_Color = Math::colBLACK;
                CEngineManagers::GetSingletonPtr()->GetGraphicsManager()->DrawColoredQuad2DTexturedInPixels(l_Rect, l_Color, &m_Texture, 0.0f, 0.0f, 1.0f, 1.0f);
            }

            l_FrameIndex++;
            l_LastTime = l_ActialTime;
        }
    }

    EndThread();
}

bool CVideoPlayer::Play(char *l_File)
{
    if(!m_pAviReader->OpenFile(l_File)) return false;
    m_FrameTime = 1.0f / 24.0f;
    m_StopPlay = false;

    AVIFILEINFO l_VideoInfo = m_pAviReader->GetFileInfo();
    CTexture::TFormatType l_TexFormat = CTexture::eRGBA8;

    CTexture::TUsageType l_TexUsage = CTexture::eUsageDynamic;
    CTexture::TPoolType l_TexPool = CTexture::eDefaultPool;

    if(!m_Texture.Create("VideoPlayer", l_VideoInfo.dwWidth, l_VideoInfo.dwHeight, 1, l_TexUsage, l_TexPool, l_TexFormat, false)) return false;
    BeginThread(); return true;
}

void WindowsThreadFunction(CVideoPlayer* theThread) 
{
    theThread->threadProc();
}
