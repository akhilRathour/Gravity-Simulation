
#pragma once
#include <glm/glm.hpp>

namespace Constants {
  constexpr float G = 0.1f;
    constexpr float SunMass = 1000.0f;
    constexpr float EarthMass = 1.0f;

   
    glm::vec3 SunPos = glm::vec3(0.0f);
    glm::vec3 EarthPos = glm::vec3(100.0f, 0.0f, 0.0f);

   
    glm::vec3 SunVel = glm::vec3(0.0f,0.0f,0.0f);
    glm::vec3 EarthVel = glm::vec3( 0.0f, 0.0f, 1.0f );
    

}