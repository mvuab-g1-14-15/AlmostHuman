#ifndef SCRIPTCONTOLLER_H
#define SCRIPTCONTOLLER_H

#include "RenderableObject\RenderableObject.h"

class CScriptedController : CRenderableObject
{
    private:

    public:
        CScriptedController     ();
        ~CScriptedController    ();

        void    Init    ();
        void    Update  ();
        void    Render  ();
};

#endif