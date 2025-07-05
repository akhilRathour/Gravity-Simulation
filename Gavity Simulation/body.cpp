#include "Body.h"

Body::Body( Mesh& mesh, float mass,  glm::vec3& position, glm::vec3& velocity)
    : mesh(mesh), mass(mass), position(position), velocity(velocity), acceleration(0.0f)
{
}
