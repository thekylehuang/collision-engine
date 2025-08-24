#pragma once
#include <glad/glad.h>
#include <vector>

struct Buffer {
  GLuint VAO;
  GLuint VBO;
  GLsizei vertexCount;
};

Buffer createVertexBuffer(const float* vertices, size_t vertexCount, size_t vertexSize);