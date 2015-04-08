#include "SoundManager.h"
#include "Utils\Defines.h"
#include "XML\XMLTreeNode.h"

#include "alc.h"
#include "alut.h"

CSoundManager::CSoundManager()
{
    m_XmlPath = "";
}

CSoundManager::~CSoundManager()
{
    m_XmlPath.clear();
}

bool CSoundManager::LoadSounds(const std::string &l_XmlPath)
{
    m_XmlPath = l_XmlPath;
    
    CXMLTreeNode newFile;
    if(!newFile.LoadFile(l_XmlPath.c_str()))
    {
        LOG_ERROR_APPLICATION("CSoundManager::Load Can't open %s\n", l_XmlPath.c_str());
        newFile.Done();
        return false;
    }

    CXMLTreeNode xmlNodes = newFile["sounds"];
    if(!xmlNodes.Exists())
    {
        LOG_ERROR_APPLICATION("CSoundManager::Load Can't read tag sounds\n");
        newFile.Done();
        return false;
    }

    for(int i = 0; i < xmlNodes.GetNumChildren(); ++i)
    {
        std::string l_SoundName(xmlNodes(i).GetPszProperty("name", ""));
        std::string l_SoundPath(xmlNodes(i).GetPszProperty("path", ""));

        if(l_SoundName != "" && l_SoundPath != "")
        {
            tIdBuffer buffer;
            if(_loadSound(l_SoundPath, buffer)) m_Buffers.insert(std::pair<tAction, tIdBuffer>(l_SoundName, buffer));
            else LOG_ERROR_APPLICATION("CSoundManager::Load Can't load sound: %s\n", l_SoundName.c_str());
        }
    }

    newFile.Done();
    return true;
}

//-----------GENERAL FUNCTIONS---------------------
void CSoundManager::Update(float delatTime)
{
}

void CSoundManager::Reset()
{
    Stop();
    _clear();
}

void CSoundManager::Pause()
{
    for(unsigned int i = 0; i < m_Sources.size(); ++i)
    {
        alSourcePause(m_Sources[i].m_uSource);
    }
}

void CSoundManager::Stop()
{
    for(unsigned int i = 0; i < m_Sources.size(); ++i)
    {
        alSourceStop(m_Sources[i].m_uSource);
    }
}

void CSoundManager::SetGain(float gain)
{
    alListenerf(AL_GAIN, gain);
}

float CSoundManager::GetGain()
{
    float l_Gain = 0.0f;
    alGetListenerf(AL_GAIN, &l_Gain);

    return l_Gain;
}

//---------ACTION FUNCTIONS-------------------------------
bool CSoundManager::PlayAction2D(const std::string& action)
{
    if(!m_bSoundON) return false;

    std::map<tAction, tIdBuffer>::iterator it = m_Buffers.find(action);

    if(it == m_Buffers.end())
    {
        LOG_ERROR_APPLICATION("CSoundManager::PlayAction2D Action %s not found\n", action.c_str());
        return false;
    }

    int l_SourceIndex = _getSource(false);
    if(l_SourceIndex < 0) return false;

    ALfloat l_SourcePosition[3] = { 0.0f, 0.0f, 0.0f };
    ALfloat l_SourceVelocity[3] = { 0.0f, 0.0f, 0.0f };

    alSourcei (m_Sources[l_SourceIndex].m_uSource, AL_BUFFER, it->second);
    alSourcei (m_Sources[l_SourceIndex].m_uSource, AL_SOURCE_RELATIVE, AL_TRUE);

    alSourcef (m_Sources[l_SourceIndex].m_uSource, AL_PITCH, 1.0);
    alSourcef (m_Sources[l_SourceIndex].m_uSource, AL_GAIN, 1.0);

    alSourcefv(m_Sources[l_SourceIndex].m_uSource, AL_POSITION, l_SourcePosition);
    alSourcefv(m_Sources[l_SourceIndex].m_uSource, AL_VELOCITY, l_SourceVelocity);

    alSourcei (m_Sources[l_SourceIndex].m_uSource, AL_LOOPING, AL_FALSE);
    alSourcePlay(m_Sources[l_SourceIndex].m_uSource);


    return true;
}

bool CSoundManager::PlayAction3D(const std::string& action, const Math::Vect3f& position)
{
    if(!m_bSoundON) return false;

    std::map<tAction, tIdBuffer>::iterator it = m_Buffers.find(action);

    if(it == m_Buffers.end())
    {
        LOG_ERROR_APPLICATION("CSoundManager::PlayAction3D Action %s not found\n", action.c_str());
        return false;
    }
    
    int l_SourceIndex = _getSource(false);
    if(l_SourceIndex < 0) return false;

     ALfloat l_SourcePosition[3] = { position.x, position.y, position.z };
     ALfloat l_SourceVelocity[3] = { 0.0f, 0.0f, 0.0f };

    alSourcei (m_Sources[l_SourceIndex].m_uSource, AL_BUFFER, it->second);
    alSourcei (m_Sources[l_SourceIndex].m_uSource, AL_SOURCE_RELATIVE, AL_TRUE);

    alSourcef (m_Sources[l_SourceIndex].m_uSource, AL_PITCH, 1.0);
    alSourcef (m_Sources[l_SourceIndex].m_uSource, AL_GAIN, 1.0);

    alSourcefv(m_Sources[l_SourceIndex].m_uSource, AL_POSITION, l_SourcePosition);
    alSourcefv(m_Sources[l_SourceIndex].m_uSource, AL_VELOCITY, l_SourceVelocity);

    alSourcei (m_Sources[l_SourceIndex].m_uSource, AL_LOOPING, AL_FALSE);
    alSourcePlay(m_Sources[l_SourceIndex].m_uSource);

    return true;
}
		
//-----SOURCE FUNCTIONS----------------------------
int CSoundManager::CreateSource()
{
    tInfoSource l_SourceInfo;
    alGenSources(1, &l_SourceInfo.m_uSource);

    if(alGetError() != AL_NO_ERROR)
    {
        LOG_ERROR_APPLICATION("CSoundManager::_getSource Can't create a new source\n");
        return -1;
    }

    l_SourceInfo.m_bReserved = true;
    m_Sources.push_back(l_SourceInfo);

    return m_Sources.size() - 1;
}

bool CSoundManager::DeleteSource(uint32 source)
{
    if(source >= m_Sources.size()) return false;

    StopSource(source);
    m_Sources[source].m_bReserved = false;

    return true;
}

bool CSoundManager::PlaySource2D(uint32 source, const std::string& action, bool loop)
{
    std::map<tAction, tIdBuffer>::iterator it = m_Buffers.find(action);

    if(it == m_Buffers.end())
    {
        LOG_ERROR_APPLICATION("CSoundManager::PlaySource2D Action %s not found\n", action.c_str());
        return false;
    }

    if(!((source < m_Sources.size()) ? m_Sources[source].m_bReserved : false))
    {
        LOG_ERROR_APPLICATION("CSoundManager::PlaySource2D Index out of bound or source not reserved\n", action.c_str());
        return false;
    }

    ALfloat l_SourcePosition[3] = { 0.0f, 0.0f, 0.0f };
    ALfloat l_SourceVelocity[3] = { 0.0f, 0.0f, 0.0f };
    ALboolean l_Loop = (loop) ? AL_TRUE : AL_FALSE;

    alSourcei (m_Sources[source].m_uSource, AL_BUFFER, it->second);
    alSourcei (m_Sources[source].m_uSource, AL_SOURCE_RELATIVE, AL_TRUE);

    alSourcef (m_Sources[source].m_uSource, AL_PITCH, 1.0);
    alSourcef (m_Sources[source].m_uSource, AL_GAIN, 1.0);

    alSourcefv(m_Sources[source].m_uSource, AL_POSITION, l_SourcePosition);
    alSourcefv(m_Sources[source].m_uSource, AL_VELOCITY, l_SourceVelocity);

    alSourcei (m_Sources[source].m_uSource, AL_LOOPING, AL_FALSE);
    alSourcePlay(m_Sources[source].m_uSource);

    return true;
}

bool CSoundManager::PlaySource3D(uint32 source, const std::string& action, bool loop)
{
    std::map<tAction, tIdBuffer>::iterator it = m_Buffers.find(action);

    if(it == m_Buffers.end())
    {
        LOG_ERROR_APPLICATION("CSoundManager::PlaySource3D Action %s not found\n", action.c_str());
        return false;
    }

    if(!((source < m_Sources.size()) ? m_Sources[source].m_bReserved : false))
    {
        LOG_ERROR_APPLICATION("CSoundManager::PlaySource3D Index out of bound or source not reserved\n", action.c_str());
        return false;
    }

    ALfloat l_SourcePosition[3] = { 0.0f, 0.0f, 0.0f };
    ALfloat l_SourceVelocity[3] = { 0.0f, 0.0f, 0.0f };
    ALboolean l_Loop = (loop) ? AL_TRUE : AL_FALSE;

    return true;
}

bool CSoundManager::PauseSource(uint32 source)
{
    if(!((source < m_Sources.size()) ? m_Sources[source].m_bReserved : false)) return false;
    alSourcePause(m_Sources[source].m_uSource);

    return true;
}

bool CSoundManager::StopSource(uint32 source)
{
    if(!((source < m_Sources.size()) ? m_Sources[source].m_bReserved : false)) return false;
    alSourceStop(m_Sources[source].m_uSource);

    return true;
}

bool CSoundManager::SetSourceGain(uint32 source, float inGain)
{
    if(!((source < m_Sources.size()) ? m_Sources[source].m_bReserved : false)) return false;
    alSourcef(m_Sources[source].m_uSource, AL_GAIN, inGain);

    return true;
}

bool CSoundManager::GetSourceGain(uint32 source, float& outGain)
{
    outGain = 0.0f;

    if(!((source < m_Sources.size()) ? m_Sources[source].m_bReserved : false)) return false;
    alGetSourcef(m_Sources[source].m_uSource, AL_GAIN, &outGain);

    return true;
}

bool CSoundManager::SetSourcePosition(uint32 source, const Math::Vect3f& inPosition)
{
    return true;
}

bool GetSourcePosition(uint32 source, Math::Vect3f& outPosition)
{
    return true;
}

bool CSoundManager::SetSourceVelocity(uint32 source, const Math::Vect3f& inVelocity)
{
    return true;
}

bool CSoundManager::GetSourceVelocity(uint32 source, Math::Vect3f& outVelocity)
{
    return true;
}	 
		
//-----LISTENER FUNCTIONS-------------------------
void CSoundManager::SetListenerPosition(const Math::Vect3f& inPosition)
{
}

void CSoundManager::GetListenerPosition(Math::Vect3f& outPosition)
{
}

void CSoundManager::SetListenerVelocity(const Math::Vect3f& inVelocity)
{
}

void CSoundManager::GetListenerVelocity(Math::Vect3f& outVelocity)
{
}

void CSoundManager::SetListenerOrientation(const Math::Vect3f& inAt, const Math::Vect3f& inUp)
{
}

void CSoundManager::GetListenerOrientation(Math::Vect3f& outAt, Math::Vect3f& outUp)
{
}

//-----PRIVATE FUNCTIONS-------------------------
bool CSoundManager::_initAL()
{
    ALenum error;
    ALCdevice* pDevice;
    ALCcontext* pContext;
    
    // Get handle to default device.
    pDevice = alcOpenDevice(NULL);
    
    // Get the device specifier.
    //const ALCubyte* deviceSpecifier = alcGetString(pDevice, ALC_DEVICE_SPECIFIER);
    
    // Create audio context.
    pContext = alcCreateContext(pDevice, NULL);
    
    // Set active context.
    alcMakeContextCurrent(pContext);
    
    // Check for an error.
    if ((error = alcGetError(pDevice)) != ALC_NO_ERROR)
    {
        std::string description = "Can't create openAL context (" + _getALErrorString(error) + ")";
        LOG_ERROR_APPLICATION("CSoundManager::_initAL %s\n", description.c_str());
        return false;
    }
    
    return true;
}

void CSoundManager::_finalizeAL()
{
    _clear();
    
    ALCcontext* pCurContext;
    ALCdevice* pCurDevice;
    
    // Get the current context.
    pCurContext = alcGetCurrentContext();
    
    // Get the device used by that context.
    pCurDevice = alcGetContextsDevice(pCurContext);
    
    // Reset the current context to NULL.
    alcMakeContextCurrent(NULL);
    
    // Release the context and the device.
    alcDestroyContext(pCurContext);
    alcCloseDevice(pCurDevice);
}

bool CSoundManager::_loadSound(const std::string& file, tIdBuffer& buffer)
{
    FILE *fd;

    ALenum format;
    ALenum error;

    ALsizei size;
    ALsizei freq;

    ALvoid* data;
    ALboolean loop;
    
    // Load wav data into buffers.
    alGenBuffers(1, &buffer);
    if((error = alGetError()) != AL_NO_ERROR)
    {    
        alDeleteBuffers(1,&buffer);
        std::string description = "Error: Can't create openAL Buffer (" + _getALErrorString(error)  + ")";
        LOG_ERROR_APPLICATION("CSoundManager::_loadSound %s\n", description.c_str());

        return false;
    }
    
    // Check if the file exists
    fopen_s(&fd, file.c_str(), "r");
    if (fd == NULL)
    {
        alDeleteBuffers(1, &buffer);
        std::string description = "Error: Can't open file " + file;
        LOG_ERROR_APPLICATION("CSoundManager::_loadSound %s\n", description.c_str());
        
        return false;
    }
    else
    {
        fclose(fd);
    }

    #pragma warning( disable : 4996 )
    alutLoadWAVFile((ALbyte*)file.c_str(), &format, &data, &size, &freq, &loop);
    alBufferData(buffer, format, data, size, freq);
    alutUnloadWAV(format, data, size, freq);
    #pragma warning( default : 4996 )
    
    if ((error=alGetError()) != AL_NO_ERROR)
    {   	 
        alDeleteBuffers(1, &buffer);
        std::string description = "Error: Can't load sound file " + file + " (" + _getALErrorString(error)  + ")";
        LOG_ERROR_APPLICATION("CSoundManager::_loadSound %s\n", description.c_str());
        
        return false;
    }   	 
    
    return true;
}

void CSoundManager::_clear()
{
    ALuint aux;    
    std::map<tAction, tIdBuffer>::iterator itBuffer;
    
    for(itBuffer = m_Buffers.begin(); itBuffer != m_Buffers.end(); ++itBuffer)
    {
        aux = (*itBuffer).second;
        alDeleteBuffers (1,&aux);
    }
    
    std::vector<tInfoSource>::iterator itSource;
    for( itSource = m_Sources.begin(); itSource != m_Sources.end(); ++itSource )
    {
        aux = (*itSource).m_uSource;
        alDeleteSources (1,&aux);
    }
    
    m_Buffers.clear();
    m_Sources.clear();
}

int CSoundManager::_getSource(bool reserved)
{
    for(unsigned int i = 0; i < m_Sources.size(); ++i)
    {
        if(m_Sources[i].m_bReserved) continue;

        ALint l_SourceState = 0;
        alGetSourcei(m_Sources[i].m_uSource, AL_SOURCE_STATE, &l_SourceState);

        if(l_SourceState == AL_PLAYING) continue;

        m_Sources[i].m_bReserved = reserved;
        return i;
    }

    return -1;
}

void Release()
{
}

std::string	_getALErrorString(ALenum err)
{
    return "";
}

