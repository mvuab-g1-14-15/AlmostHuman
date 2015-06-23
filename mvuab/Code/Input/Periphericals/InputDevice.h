//----------------------------------------------------------------------------------
// CInputDevice class
// Author: Enric Vergara
//
// Description:
// Esta clase gestiona el device del directInput comun a Mouse, Keyboard y Josytick
//----------------------------------------------------------------------------------
#pragma once

#ifndef _INPUT_DEVICE_H_
#define _INPUT_DEVICE_H_

// Note: Defines must be included before <dinptut> because it defines the DIRECTINPUT_VERSION
#include "Utils\Defines.h"
#include <dinput.h>

#include "InputDefs.h"
#include "Math\Vector2.h"

class CInputDevice
{
    public:
        CInputDevice() : m_bIsOk(false), m_pDevice(NULL), m_pDI(NULL) {}
        virtual ~CInputDevice()
        {
            Done();
        }

        // virtual methods for heirs
        bool                    Init              (LPDIRECTINPUT8, HWND);
        void                    Done            ();
        bool                    IsOk            () const
        {
            return m_bIsOk;
        }

        virtual HRESULT         Update            () = 0;

        // base class methods
        virtual HRESULT         CrankUp            ( REFGUID rguid, LPCDIDATAFORMAT lpdf, bool exclusiveMode = false);

        // Accessor-Methods
        virtual const Math::Vect2i&        GetPosition        ()
        {
            return m_Pos;
        }
        virtual void                SetPosition        ( const Math::Vect2i &pos )
        {
            m_Pos = pos;
        }


    protected:
        virtual HRESULT         GetData            ( EInputDeviceType Type, void *pData, DWORD *dwNum);
        void                    Release          ();

    protected:
        bool                              m_bIsOk;            // Initialization boolean control
        LPDIRECTINPUTDEVICE8      m_pDevice;
        LPDIRECTINPUT8                m_pDI;
        HWND                              m_hWnd;
        Math::Vect2i                                    m_Pos;
};

#endif //_INPUT_DEVICE_H_