#pragma once
#include <deque>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include<vector>
class TrailMesh {
public:
    std::deque<glm::vec3> points;
    size_t maxLength = 300;

    TrailMesh();
    ~TrailMesh();

    void RecordPoint(const glm::vec3& pos);
    void UpdateBuffer();
    void Draw() const;
	void setMaxLength(size_t length) { maxLength = length; }
    void reset() {
        points.clear();
        UpdateBuffer(); // Clear the VBO
	}
private:
    GLuint VAO, VBO;
};
