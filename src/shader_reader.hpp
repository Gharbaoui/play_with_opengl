#pragma once
struct ShaderProgramSources {
  std::string vertex_source;
  std::string fragment_source;
};
ShaderProgramSources parse_shader(const std::string& file_path);