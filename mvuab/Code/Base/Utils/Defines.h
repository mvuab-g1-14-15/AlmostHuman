#ifndef _DEFINES_H_
#define _DEFINES_H_

#include "Types.h"
#include <assert.h>

// MACROS
#define GET_SET(TYPE, PARAMETER_NAME) \
	void Set##PARAMETER_NAME(TYPE PARAMETER_NAME) \
	{ \
		m_##PARAMETER_NAME=PARAMETER_NAME; \
	} \
	TYPE Get##PARAMETER_NAME() \
	{ \
		return m_##PARAMETER_NAME; \
	}

// MACROS
#define GET_SET_PTR(TYPE, PARAMETER_NAME) \
	void Set##PARAMETER_NAME(TYPE##* PARAMETER_NAME) \
	{ \
		m_p##PARAMETER_NAME=PARAMETER_NAME; \
	} \
	TYPE##* Get##PARAMETER_NAME() \
	{ \
		return m_p##PARAMETER_NAME; \
	}

#define GET_SET_REF(TYPE, PARAMETER_NAME) \
	void Set##PARAMETER_NAME(const TYPE &##PARAMETER_NAME) \
	{ \
		m_##PARAMETER_NAME=PARAMETER_NAME; \
	} \
	const TYPE & Get##PARAMETER_NAME() \
	{ \
		return m_##PARAMETER_NAME; \
	}

#define CHECKED_DELETE(x)				if(x!=NULL) delete x; x=NULL;
#define DIRECTINPUT_VERSION				0x0800
#define CHECKED_RELEASE(x)				if(x) x->Release(); x=NULL;
#define CHECKED_DELETE_ARRAY(x)			if(x) delete [] x; x=NULL;

#define CoreInstance		CCore::GetSingletonPtr()
#define EngineInstance		CEngine::GetSingletonPtr()
#define GraphicsInstance	CGraphicsManager::GetSingletonPtr()
#define SoundInstance		CSoundManager::GetSingletonPtr()

#endif
