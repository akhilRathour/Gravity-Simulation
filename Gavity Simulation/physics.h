#pragma once

#include "Body.h"
#include <vector>

class Physics
{
public:
    void ApplyGravitationalForces(std::vector<Body*>& bodies);
    void UpdateBodies(std::vector<Body*>& bodies, float deltaTime);

    static constexpr float G = 6.67430e-11f; // Gravitational constant
};
