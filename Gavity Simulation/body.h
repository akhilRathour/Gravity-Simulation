#pragma once

#include "Mesh.h"
#include <glm/glm.hpp>

class Body
{
public:
    Body( Mesh& mesh, float mass,  glm::vec3& position, glm::vec3& velocity);

    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;

    float mass;
     Mesh& mesh;
};

