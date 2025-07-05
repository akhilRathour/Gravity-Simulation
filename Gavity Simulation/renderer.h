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

private:
    std::vector<Body*> bodies;
};
