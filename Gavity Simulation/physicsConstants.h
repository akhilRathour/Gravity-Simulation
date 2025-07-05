
#pragma once
#include <glm/glm.hpp>

namespace Constants {
  constexpr float G = 0.1f;
    constexpr float SunMass = 1989.0f;  
    constexpr float EarthMass = 0.972f;

   
    glm::vec3 SunPos = glm::vec3(0.0f);
    glm::vec3 EarthPos = glm::vec3(50.0f, 0.0f, 0.0f); 

   
    glm::vec3 SunVel = glm::vec3(0.0f);
    glm::vec3 EarthVel = glm::vec3(0.0f, 3.78f, 0.0f); 

}