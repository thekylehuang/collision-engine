#include "buffer_utils.h"

Buffer createVertexBuffer(const float* vertices, size_t vertexCount, size_t vertexSize) {
  Buffer buffer{};
  buffer.vertexCount = static_cast<GLsizei>(vertexCount);
  glGenVertexArrays(1, &buffer.VAO);
  glGenBuffers(1, &buffer.VBO);
  glBindVertexArray(buffer.VAO);
  glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO);
  glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSize * sizeof(float), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, static_cast<GLint>(vertexSize), GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  return buffer;
}