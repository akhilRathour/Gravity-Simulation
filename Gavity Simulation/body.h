#pragma once

#include "Mesh.h"
#include <glm/glm.hpp>
#include "TrailMesh.h"
class Body
{
public:
    Body( Mesh& mesh, float mass,float radius,  glm::vec3& position, glm::vec3& velocity);

    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float mass;
	float radius = 1.0f; // scale wrt to earth radius
     Mesh& mesh;

     //for trail
     TrailMesh trail;
     void UpdateTrail();

     // // Store initial state for reset
     glm::vec3 initialPosition;
     glm::vec3 initialVelocity;
     void Reset();
};

