#ifndef _BLACKBOARD_H_
#define _BLACKBOARD_H_

#include "./Subscriber.h"
#include "./MessageDefines.h"

#include <vector>


class CBlackboard
{
    private:
        std::vector<CSubscriber *> m_Subscribers;
        std::vector<s_Messages> m_Messages;

        unsigned int GetSubscriberPos(CSubscriber *l_Subscriber);

    public:
        CBlackboard     ();
        ~CBlackboard    ();

        void Subscribe(CSubscriber *l_Subscriber);
        void UnSubscribe(CSubscriber *l_Subscriber);

        void PostMessage(CSubscriber *l_Subscriber, const s_Messages &l_Message);
};

#endif