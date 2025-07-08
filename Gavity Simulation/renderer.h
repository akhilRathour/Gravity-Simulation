#pragma once

#include"shaderClass.h"
#include "Camera.h"
#include "Body.h"
#include <vector>

class Renderer
{
public:
    void Submit(Body* body);
    void DrawAll( Shader& shader,  Camera& camera);
    
    //trail fncns
    void DrawTrails(Shader& shader, Camera& camera);
    void SetShowTrails(bool value) { showTrails = value; }
    bool GetShowTrails() const { return showTrails; }
    void SetTrailLength(size_t len) {
        globalTrailLength = len;
        for (auto* body : bodies) {
            body->trail.setMaxLength(len);
        }
    }
private:
    std::vector<Body*> bodies;
	bool showTrails = true; // Default to true, can be toggled
    size_t globalTrailLength = 300;
};
