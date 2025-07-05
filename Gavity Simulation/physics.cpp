#include "Physics.h"
#include <glm/glm.hpp>

void Physics::ApplyGravitationalForces(std::vector<Body*>& bodies)
{
    //debug
   /* for (size_t i = 0; i < bodies.size(); ++i) {
        Body* b = bodies[i];
        std::cout << "Body " << i
            << " Mass: " << b->mass
            << " Pos: (" << b->position.x << ", " << b->position.y << ", " << b->position.z << ")"
            << " Vel: (" << b->velocity.x << ", " << b->velocity.y << ", " << b->velocity.z << ")"
            << std::endl;
    }*/
    for (size_t i = 0; i < bodies.size(); ++i)
    {
        Body* a = bodies[i];
        for (size_t j = i + 1; j < bodies.size(); ++j)
        {
            Body* b = bodies[j];

            glm::vec3 direction = b->position - a->position;
            float distance = glm::length(direction);
            if (distance < 1e-3f) continue; // Avoid divide-by-zero

            glm::vec3 forceDir = glm::normalize(direction);
            float forceMag = G * (a->mass * b->mass) / (distance * distance);
            glm::vec3 force = forceMag * forceDir;

            // Apply Newton's third law
            a->acceleration += force / a->mass;
            b->acceleration -= force / b->mass;

            std::cout << "Distance: " << distance << ", ForceMag: " << forceMag << std::endl;
        }
    }
}

void Physics::UpdateBodies(std::vector<Body*>& bodies, float deltaTime)
{
    for (Body* body : bodies)
    {
        body->velocity += body->acceleration * deltaTime;
        body->position += body->velocity * deltaTime;
        body->acceleration = glm::vec3(0.0f); // Reset for next frame
    }
}
