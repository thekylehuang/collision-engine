#pragma once
#include <glad/glad.h>
#include <string>

GLuint compileShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
std::string readShaderFile(const std::string& path);