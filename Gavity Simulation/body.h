#pragma once

#include "Mesh.h"
#include <glm/glm.hpp>
#include "TrailMesh.h"
class Body
{
public:
    Body( Mesh& mesh, float mass,  glm::vec3& position, glm::vec3& velocity);

    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float mass;
     Mesh& mesh;

     //for trail
     TrailMesh trail;
     void UpdateTrail();
};

