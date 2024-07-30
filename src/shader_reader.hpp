#pragma once

#include <glad/glad.h>
#include <fstream>
#include <iostream>
#include <sstream>

struct ShaderProgramSources {
  std::string vertex_source;
  std::string fragment_source;
};
ShaderProgramSources parse_shader(const std::string& file_path);

class Shader
{
  public:
    Shader(const char* vertex_path, const char* fragment_path);
    void use();
    // for uniforms
    void set_bool(const std::string& name, bool value) const;
    void set_int(const std::string& name, int value) const;
    void set_float(const std::string& name, float value) const;
  private:
    unsigned int id;
};