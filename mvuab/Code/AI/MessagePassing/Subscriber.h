#ifndef _SUBSCRIBER_H_
#define _SUBSCRIBER_H_

class CSubscriber
{
    private:
        bool m_HasMessage;

    public:
        CSubscriber     ();
        ~CSubscriber    ();

        void NoMessage  ();
        void NewMessage ();

        bool HasMessage ();
};

#endif