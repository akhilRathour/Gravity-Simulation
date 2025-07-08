#include "Renderer.h"
#include <glm/gtc/matrix_transform.hpp>

void Renderer::Submit(Body* body)
{
    bodies.push_back(body);
}

void Renderer::DrawAll( Shader& shader,  Camera& camera)
{
    for (Body* body : bodies)
    {    

        glm::mat4 model = glm::translate(glm::mat4(1.0f), body->position);

       /* std::cout << "Position: " << body->position.x << ", "
            << body->position.y << ", "
            << body->position.z << std::endl;*/

        Shader& s = shader; // to set uniforms (if not using const-ref)

        s.Activate();
        
        glUniformMatrix4fv(glGetUniformLocation(s.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        body->mesh.Draw(s, camera);
        body->UpdateTrail(); // Update trail with new position
    }
   

}



void Renderer::DrawTrails(Shader& shader, Camera& camera) {
    if(showTrails){
        shader.Activate();
        camera.Matrix(shader, "camMatrix");

        for (auto* body : bodies) {
            body->trail.Draw();
        }
    }
}

void Renderer::ResetSimulation() {
    for (auto* body : bodies) {
        body->Reset();
    }
}
