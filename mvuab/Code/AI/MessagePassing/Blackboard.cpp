#include "./Blackboard.h"

CBlackboard::CBlackboard()
{
}

CBlackboard::~CBlackboard()
{
    m_Subscribers.clear();
}

unsigned int CBlackboard::GetSubscriberPos(CSubscriber *l_Subscriber)
{
    unsigned int i = 0;
    while(i < m_Subscribers.size() && l_Subscriber != m_Subscribers[i++]);

    return i;
}

void CBlackboard::Subscribe(CSubscriber *l_Subscriber)
{
    m_Subscribers.push_back(l_Subscriber);
}

void CBlackboard::UnSubscribe(CSubscriber *l_Subscriber)
{
    unsigned int l_PosInVec = GetSubscriberPos(l_Subscriber);
    if(l_PosInVec < m_Subscribers.size()) m_Subscribers.erase(m_Subscribers.begin() + l_PosInVec);
}

void CBlackboard::PostMessage(CSubscriber *l_Subscriber, const s_Messages &l_Message)
{
    m_Messages.push_back(l_Message);

    for(unsigned int i = 0; i < m_Subscribers.size(); ++i)
    {
        if(l_Subscriber != m_Subscribers[i]) m_Subscribers[i]->NewMessage();
    }
}
