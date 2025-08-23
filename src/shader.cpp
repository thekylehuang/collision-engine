#include "shader.h"
#include <fstream>
#include <sstream>
#include <fmt/core.h>

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