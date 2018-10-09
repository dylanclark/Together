
#include <loader.hpp>

GLuint
Loader::load_to_vao(float* vertices,        int num_vertices,
                    unsigned int* indices,  int num_indices)
{
    GLuint vao;

    GLuint vertex_buffer_obj;
    GLuint element_buffer_obj;

    // first, we create the 2d object in model space
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vertex_buffer_obj);
    glGenBuffers(1, &element_buffer_obj);
    // make sure the array obj is bound so it knows what's up with the buffer objs
    glBindVertexArray(vao);

    // set up the buffer objs
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices,
                 vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * num_indices,
                 indices, GL_STATIC_DRAW);

    // set vertex attributes
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    return vao;
}

GLuint Loader::load_to_postprocess_vao()
{
    GLuint vao;

    // set up vao
    float vertices[] = {
        -1.0, 1.0,
        1.0, 1.0,
        1.0, -1.0,
        -1.0, -1.0
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    GLuint vertex_buffer_obj;
    GLuint element_buffer_obj;

    // first, we create the 2d object in model space
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vertex_buffer_obj);
    glGenBuffers(1, &element_buffer_obj);
    // make sure the array obj is bound so it knows what's up with the buffer objs
    glBindVertexArray(vao);

    // set up the buffer objs
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_obj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_obj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // set vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    return vao;
}
