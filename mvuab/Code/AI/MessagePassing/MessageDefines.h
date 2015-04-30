#ifndef _MESSAGE_DEFINES_
#define _MESSAGE_DEFINES_

#include "Math\Vector3.h"

typedef enum e_MessageType
{
    NO_MESSAGE      = 0,
    PLAYER_SEEN     = 1,
    PLAYER_HEARED   = 2
} e_MessageType;

typedef struct s_Messages
{
    e_MessageType   m_Messages;
    Math::Vect3f    m_PlayerPos;

    s_Messages()
    {
        m_Messages = NO_MESSAGE;
        m_PlayerPos = Math::Vect3f();
    }
} s_Messages;

#endif