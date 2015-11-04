#pragma once

#ifndef _PHYSICS_CONTROLLER_HIT_REPORT_H
#define _PHYSICS_CONTROLLER_HIT_REPORT_H

#include "Logger\Logger.h"
#include "NxPhysics.h"

class CPhysicsControllerHitReport : public NxUserControllerHitReport
{
    public:

        virtual NxControllerAction onShapeHit(const NxControllerShapeHit& hit)
        {
			/*
            if(hit.shape)
            {
                NxCollisionGroup group = hit.shape->getGroup();
                if ( group == ECG_DYNAMIC_OBJECTS )
                {
                    NxActor& actor = hit.shape->getActor();

                    if(hit.dir.y == 0.0f)
                    {
                        NxF32 coeff = actor.getMass() * hit.length * 10.0f;
                        actor.addForceAtLocalPos(hit.dir * coeff, NxVec3(0, 0, 0), NX_IMPULSE);
                    }
                }

                if ( group == ECG_PLAYER )
                {
                    LOG_INFO_APPLICATION("onShapeHit -> Hit ECG_PLAYER" );
                    return NX_ACTION_NONE;
                }

                if ( group == ECG_ENEMY )
                {
                    LOG_INFO_APPLICATION( "onShapeHit -> Hit ECG_ENEMY" );
                    return NX_ACTION_NONE;
                }
            }
			*/
            return NX_ACTION_NONE;
        }

        virtual NxControllerAction onControllerHit(const NxControllersHit& hit)
        {
            if(hit.controller)
            {
                NxCollisionGroup group = hit.controller->getActor()->getGroup();
                if ( group == ECG_DYNAMIC_OBJECTS )
                {
                    LOG_INFO_APPLICATION( "onControllerHit -> Hit ECG_DYNAMIC_OBJECTS" );
                    return NX_ACTION_NONE;
                }

                if ( group == ECG_PLAYER )
                {
                    LOG_INFO_APPLICATION( "onControllerHit -> Hit ECG_PLAYER" );
                    return NX_ACTION_NONE;
                }

                if ( group == ECG_ENEMY )
                {
                    LOG_INFO_APPLICATION( "onControllerHit -> Hit ECG_ENEMY" );
                    return NX_ACTION_NONE;
                }
            }

            return NX_ACTION_NONE;
        }
};

#endif