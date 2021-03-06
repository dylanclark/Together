#ifndef shader_hpp
#define shader_hpp

#include <glm/glm.hpp>
#include <glad.h>
#include <string>

class Shader
{
public:
    Shader() { };
    Shader(const GLchar* vertex_path, const GLchar* fragment_path);
    void set_bool(const std::string &name, bool value);
    void set_float(const std::string &name, float value);
    void set_int(const std::string &name, int value);
    void set_float_mat4(const std::string &name, glm::mat4 matrix);
    void set_vec2(const std::string &name, glm::vec2 vector);
    void set_vec3(const std::string &name, glm::vec3 vector);
    void use();

private:
    unsigned int m_id;
};

#endif /* shader_hpp */
