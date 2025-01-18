#include "ebo.h"
#include "utils.h"

// ebo_new creates a new Vertex Buffer Object in memory and expects the caller to free said memory.
ebo_t ebo_new(GLuint *indices, GLsizeiptr size)
{
    ebo_t ebo_id;

    // The index buffer contains indices that tell OpenGL in which order to draw the indices
    glGenBuffers(1, &ebo_id);
    check_gl_error("ebo->glGenBuffers");

    // Bind the index buffer i.e. make it current
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    check_gl_error("ebo->glBindBuffer");

    // Link the EBO buffer to the indices array
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
    check_gl_error("ebo->glBufferData");

    return ebo_id;
}

void ebo_bind(ebo_t ebo_id) { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id); }

void ebo_unbind(void)
{
    // // Unbind the EBO *after* the VAO because the EBO is stored inside the VAO
    // If you accidentally unbind the EBO before the VAO you're telling OpenGL that you don't want the
    // VAO to use the EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ebo_delete(ebo_t ebo_id) { glDeleteBuffers(1, &ebo_id); }
