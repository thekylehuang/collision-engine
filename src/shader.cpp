#include "shader.h"
#include <fstream>
#include <sstream>
#include <fmt/core.h>
#include <glad/glad.h>

std::string readShaderFile(const std::string& path) {
  std::ifstream file(path);
  if (!file.is_open()) {
      fmt::print(stderr, "Could not open file. Check if path is valid.\n");
      return "";
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

GLuint compileShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) {
  std::string vertexCode = readShaderFile(vertexPath);
  std::string fragmentCode = readShaderFile(fragmentPath);

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();
  glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
  glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);

  GLint success;
  GLchar infoLog[1024];

  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
      glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog);
      fmt::print(stderr, "Vertex Shader Compilation Failed:\n{}\n", infoLog);
  }

  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
      glGetShaderInfoLog(fragmentShader, 1024, NULL, infoLog);
      fmt::print(stderr, "Fragment Shader Compilation Failed:\n{}\n", infoLog);
  }

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
      glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
      fmt::print(stderr, "Shader Program Linking Failed:\n{}\n", infoLog);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}