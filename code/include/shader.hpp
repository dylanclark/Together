#ifndef shader_hpp
#define shader_hpp

#include <glm/glm.hpp>
#include <string>

class Shader
{
public:
    Shader(const char* vertex_path, const char* fragment_path);
    void set_bool(const std::string &name, bool value);
    void set_float(const std::string &name, float value);
    void set_int(const std::string &name, int value);
    void set_float_mat4(const std::string &name, glm::mat4 matrix);
    void set_vec3(const std::string &name, glm::vec3 vector);
    void use();

private:
    unsigned int m_id;
};

#endif /* shader_hpp */
