#include "CuboidMesh.hpp"

void CuboidMesh::createPoints() {
    float w = width / 2.0f;
    float h = height / 2.0f;
    float l = length / 2.0f;

    points[0] = glm::vec3(-w, -h, l);
    points[1] = glm::vec3(w, -h, l);
    points[2] = glm::vec3(w, -h, -l);
    points[3] = glm::vec3(-w, -h, -l);

    points[4] = glm::vec3(-w, h, l);
    points[5] = glm::vec3(w, h, l);
    points[6] = glm::vec3(w, h, -l);
    points[7] = glm::vec3(-w, h, -l);
}

void CuboidMesh::createSide(const std::array<unsigned int, 4>& indices) {
    
    glm::vec3 sideCenter = glm::vec3(0.0f);
    for (unsigned int i : indices)
        sideCenter += points[i];
    sideCenter /= static_cast<float>(indices.size());

    glm::vec3 normal = sideCenter - calculateCenter();

    vertexData.push_back({ points[indices[0]], normal, glm::vec2(0.0f, 0.0f) });
    vertexData.push_back({ points[indices[1]], normal, glm::vec2(0.0f, 1.0f) });
    vertexData.push_back({ points[indices[2]], normal, glm::vec2(1.0f, 1.0f) });

    vertexData.push_back({ points[indices[2]], normal, glm::vec2(1.0f, 1.0f) });
    vertexData.push_back({ points[indices[3]], normal, glm::vec2(1.0f, 0.0f) });
    vertexData.push_back({ points[indices[0]], normal, glm::vec2(0.0f, 0.0f) });
}

void CuboidMesh::setBufferData(const void* data, unsigned int size) {
    vertexBuffer.setData(data, size);
    vertexBufferLayout.push(GL_FLOAT, 3);
    vertexBufferLayout.push(GL_FLOAT, 3);
    vertexBufferLayout.push(GL_FLOAT, 2);

    vertexArray.addBuffer(vertexBuffer, vertexBufferLayout);
}

CuboidMesh::CuboidMesh(float _width, float _height, float _length) : Mesh() {
    width = _width;
    height = _height;
    length = _length;
}

void CuboidMesh::setResources(GpuProgram* _program, Material* _material, Texture* _texture) {
    program = _program;
    material = _material;
    texture = _texture;
}

glm::vec3 CuboidMesh::calculateCenter() const {
    glm::vec3 result = glm::vec3(0.0f);

    for (glm::vec3 p : points)
        result += p;

    return result / static_cast<float>(points.size());
}

void CuboidMesh::create() {
    createPoints(); 

    createSide({0, 1, 2, 3});
    createSide({4, 5, 6, 7});

    createSide({0, 3, 7, 4});
    createSide({1, 2, 6, 5});
    
    createSide({0, 1, 5, 4});
    createSide({3, 2, 6, 7});

    setBufferData((const void*)&vertexData[0], vertexData.size() * sizeof(VertexData));
}

void CuboidMesh::draw(const Camera& camera) const {
    vertexArray.bind();

    program->bind();
    program->setMat4("MI", transformation->makeModelInverseMatrix());
    program->setMat4("M", transformation->makeModelMatrix());
    program->setMat4("V", camera.getView());
    program->setMat4("P", camera.getProjection());
    program->setVec3("eye", camera.getEye());
    setAllUniforms();

    glPolygonMode(GL_FRONT_AND_BACK, primitiveType);
    glDrawArrays(GL_TRIANGLES, 0, vertexData.size());
}
