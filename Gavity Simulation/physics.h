#pragma once

#include "Body.h"
#include <vector>
//#include"physicsConstants.h"
class Physics
{
public:
    void ApplyGravitationalForces(std::vector<Body*>& bodies);
    void UpdateBodies(std::vector<Body*>& bodies, float deltaTime);

    static constexpr float G = 0.1f; // Gravitational constant
};
