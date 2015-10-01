#pragma once
#ifndef AVI_READER_H_
#define AVI_READER_H_

// IMPORTANT: Install ffdshow code pack
// http://sourceforge.net/projects/ffdshow-tryout/

#pragma comment (lib, "Vfw32.lib")
#include <Windows.h>
#include <Vfw.h>

class CAVIReader
{
    private:
        PAVIFILE    m_pAVIFile;
        AVIFILEINFO m_AVIInfo;

        PAVISTREAM  m_pStream;
        PGETFRAME   m_pFrame;

    public:
        CAVIReader() : m_pAVIFile(0), m_pStream(0), m_pFrame(0)
        {
            ZeroMemory(&m_AVIInfo, sizeof(AVIFILEINFO));
            AVIFileInit();
        }

        ~CAVIReader()
        {
            if(m_pFrame != NULL)   AVIStreamGetFrameClose(m_pFrame);
            if(m_pStream != NULL)  AVIStreamRelease(m_pStream);
            if(m_pAVIFile != NULL) AVIFileRelease(m_pAVIFile);

            AVIFileExit();
        }

        bool OpenFile(char *l_File)
        {
            int l_AVIDescriptor = AVIFileOpen(&m_pAVIFile, l_File, OF_READ, NULL);
            if (l_AVIDescriptor != AVIERR_OK) return false;

            l_AVIDescriptor = AVIFileGetStream(m_pAVIFile, &m_pStream, streamtypeVIDEO,  0);
            if (l_AVIDescriptor != AVIERR_OK) return false;
           
            m_pFrame = AVIStreamGetFrameOpen(m_pStream,  NULL);
            if(m_pFrame == NULL) return false;
            
            AVIFileInfo(m_pAVIFile, &m_AVIInfo, sizeof(AVIFILEINFO));
            return true;
        }

        AVIFILEINFO &GetFileInfo()
        {
            return m_AVIInfo;
        }

        int GetNumFrames()
        {
            return AVIStreamLength(m_pStream);
        }

        int GetFirstFrame()
        {
            return AVIStreamStart(m_pStream);
        }

        char *GetBitmap(int l_NumFrame, int l_FirstFrame)
        {
            return (char *) AVIStreamGetFrame(m_pFrame, l_NumFrame - l_FirstFrame);
        }
};

#endif 