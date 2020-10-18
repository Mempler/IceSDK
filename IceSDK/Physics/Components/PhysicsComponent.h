#pragma once

#include <box2d/box2d.h>

namespace IceSDK::Physics::Components
{
    struct PhysicsComponent
    {
        b2BodyDef body_def;
        b2Body* body;
        b2PolygonShape shape;
    };
}  // namespace IceSDK::Physics::Components
