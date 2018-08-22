#include <engine.hpp>

std::map<std::string, Shader>  ResourceManager::shaders;
std::map<std::string, Texture>  ResourceManager::textures;

Shader ResourceManager::load_shader(const GLchar* vertex_shader_file, const GLchar* frag_shader_file, std::string name)
{
    if (shaders.count(name) == 0) {
        shaders[name] = Shader(vertex_shader_file, frag_shader_file);
    }
    return shaders[name];
}

Shader ResourceManager::get_shader(std::string name)
{
    if (shaders.count(name) == 0) {
        printf("couldn't find that shader\n");
    }
    return shaders[name];
}

Texture ResourceManager::load_texture(const GLchar* file, std::string name)
{
    if (textures.count(name) == 0) {
        textures[name] = Texture(std::string(file));
    }
    return textures[name];
}

Texture ResourceManager::get_texture(std::string name)
{
    if (textures.count(name) == 0) {
        printf("couldn't find that texture\n");
    }
    return textures[name];
}
