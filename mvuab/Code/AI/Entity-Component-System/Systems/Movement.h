#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "Entity-Component-System/Components.h"

class CMovementSystem : public artemis::EntityProcessingSystem
{
private:
    artemis::ComponentMapper<CTransform> positionMapper;
    artemis::ComponentMapper<CVelocity> velocityMapper;

public:
    CMovementSystem()
    {
        addComponentType<CTransform>();
        addComponentType<CVelocity>();
    };

    virtual void initialize()
    {
        velocityMapper.init( *world );
        positionMapper.init( *world );
    };

    virtual void processEntity( artemis::Entity& e )
    {
        positionMapper.get( e )->m_Position += velocityMapper.get( e )->m_Direction * velocityMapper.get( e )->m_Speed * world->getDelta();
    };

};

#endif //MOVEMENT_H