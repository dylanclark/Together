#ifndef LOADER_HPP
#define LOADER_HPP

#include <glad.h>

class Loader
{
public:
    static GLuint load_to_vao(float* vertices, int num_vertices,
                              unsigned int* indices, int num_indices);
    static GLuint load_to_postprocess_vao();
};

#endif /* LOADER_HPP */
