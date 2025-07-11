#include "Body.h"

Body::Body( Mesh& mesh, float mass,float radius,  glm::vec3& position, glm::vec3& velocity)
    : mesh(mesh), mass(mass),radius(radius), position(position), velocity(velocity), acceleration(0.0f), initialPosition(position), initialVelocity(velocity)
{
}
void Body::UpdateTrail() {
  

    trail.RecordPoint(position);   // ? Update trail with current position
    trail.UpdateBuffer();          // ? Update VBO
}
void Body::Reset() {
    position = initialPosition;
    velocity = initialVelocity;
    acceleration = glm::vec3(0.0f);
    trail.reset(); // reset trail completely
}