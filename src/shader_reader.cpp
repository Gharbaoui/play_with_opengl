#include "shader_reader.hpp"

ShaderProgramSources parse_shader(const std::string& file_path) {
  std::ifstream stream(file_path);
  enum class ShaderType {
    NONE = -1, VERTEX = 0, FRAGMENT = 1
  };

  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::NONE;

  while (getline(stream, line)) {
    if (line.find("#shader") != std::string::npos) {
      if (line.find("vertex") != std::string::npos) {
        type = ShaderType::VERTEX;
      } else if (line.find("fragment") != std::string::npos) {
        type = ShaderType::FRAGMENT;
      }
    } else {
      ss[(int)type] << line << '\n';
    }
  }
  return {ss[0].str(), ss[1].str()};
}


Shader::Shader(const char* vertex_path, const char* fragment_path)
{
  std::ifstream vsf(vertex_path);
  std::ifstream fsf(fragment_path);
  std::string vs_src;
  std::string fs_src;

  std::stringstream vs_stream;
  std::stringstream fs_stream;

  vs_stream << vsf.rdbuf();
  fs_stream << fsf.rdbuf();
  vs_src = vs_stream.str();
  fs_src = fs_stream.str();

  const char* c_vs_str = vs_src.c_str();
  const char* c_fs_str = fs_src.c_str();

  unsigned int vertex, fragment;
  int success;
  char infolog[512];
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &c_vs_str, NULL);
  glCompileShader(vertex);
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertex, 512, NULL, infolog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";
    std::cout << infolog << "\n";
    exit(1);
  }

  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &c_fs_str, NULL);
  glCompileShader(fragment);
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragment, 512, NULL, infolog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n";
    std::cout << infolog << "\n";
    exit(1);
  }

  id = glCreateProgram();
  glAttachShader(id, vertex);
  glAttachShader(id, fragment);
  glLinkProgram(id);
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(id, 512, NULL, infolog);
    std::cout << "ERROR::PROGRAM::LINK_FAILED\n";
    std::cout << infolog << "\n";
    exit(1);
  }
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::use()
{
  glUseProgram(id);
}

void Shader::set_bool(const std::string& name, bool value) const {
  glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::set_int(const std::string& name, int value) const {
  glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::set_float(const std::string& name, float value) const {
  glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}
