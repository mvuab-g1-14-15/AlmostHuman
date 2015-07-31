#include "./Subscriber.h"

CSubscriber::CSubscriber()
{
    m_HasMessage = false;
}

CSubscriber::~CSubscriber()
{
}

void CSubscriber::NoMessage()
{
    m_HasMessage = false;
}

void CSubscriber::NewMessage()
{
    m_HasMessage = true;
}

bool CSubscriber::HasMessage()
{
    return m_HasMessage;
}
