#include "TrailMesh.h"

TrailMesh::TrailMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW); // No initial data

    glEnableVertexAttribArray(0); // location = 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindVertexArray(0);
}

TrailMesh::~TrailMesh() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void TrailMesh::RecordPoint(const glm::vec3& pos) {
    points.push_back(pos);
    while (points.size() > maxLength)
        points.pop_front();
}


    void TrailMesh::UpdateBuffer() {
        if (points.empty()) return;

        std::vector<glm::vec3> temp(points.begin(), points.end()); // Copy to contiguous storage

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, temp.size() * sizeof(glm::vec3), temp.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


void TrailMesh::Draw() const {
    if (points.empty()) return;

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(points.size()));
    glBindVertexArray(0);
}
