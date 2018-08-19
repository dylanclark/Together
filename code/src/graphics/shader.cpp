
#include <glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <shader.hpp>

Shader::Shader(const char* vertex_path, const char* fragment_path)
{
    std::string vertex_code;
    std::string fragment_code;
    std::ifstream vertex_file;
    std::ifstream fragment_file;
    vertex_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fragment_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vertex_file.open(vertex_path);
        fragment_file.open(fragment_path);
        std::stringstream vertex_stream, file_stream;
        // read file's buffer contents into streams
        vertex_stream << vertex_file.rdbuf();
        file_stream << fragment_file.rdbuf();
        // close file handlers
        vertex_file.close();
        fragment_file.close();
        // convert stream into string
        vertex_code = vertex_stream.str();
        fragment_code = file_stream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "failure reading from shader files" << std::endl;
    }
    const char* vertex_shader_string = vertex_code.c_str();
    const char* fragment_shader_string = fragment_code.c_str();

    GLuint vertex_shader, fragment_shader;
    GLint success;
    GLchar info_log[512];

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_string, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        std::cout << "vertex shader compilation failed!\n" << info_log << std::endl;
    }

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_string, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        std::cout << "fragment shader compilation failed!\n" << info_log << std::endl;
    }

    m_id = glCreateProgram();
    glAttachShader(m_id, vertex_shader);
    glAttachShader(m_id, fragment_shader);
    glLinkProgram(m_id);
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(m_id, 512, NULL, info_log);
        std::cout << "shader program linking failed!\n" << info_log << std::endl;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Shader::set_bool(const std::string &name, bool value)
{
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int) value);
}

void Shader::set_float(const std::string &name, float value)
{
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::set_int(const std::string &name, int value)
{
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::set_float_mat4(const std::string &name, glm::mat4 matrix)
{
    int matr_idx = glGetUniformLocation(m_id, name.c_str());
    glUniformMatrix4fv(matr_idx, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::set_vec3(const std::string &name, glm::vec3 vector)
{
    int vec_idx = glGetUniformLocation(m_id, name.c_str());
    glUniform3f(vec_idx, vector.x, vector.y, vector.z);
}

void Shader::use()
{
    glUseProgram(m_id);
}
