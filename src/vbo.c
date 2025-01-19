#include "vbo.h"
#include "utils.h"

// vbo_new creates a new Vertex Buffer Object in memory.
vbo_t vbo_new(GLfloat *vertices, GLsizeiptr size)
{
    // Create a Vertex Buffer Object in which to store vertex data to be sent to GPU
    vbo_t vbo_id;

    glGenBuffers(1, &vbo_id);
    check_gl_error("vbo->glGenBuffers");

    // Binding an object makes it the current context on which operations will execute
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id); // GL_ARRAY_BUFFER is the type needed for vertex buffer
    check_gl_error("vbo->glBindBuffer");

    // Put the vertex data into the buffer
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    check_gl_error("vbo->glBufferData");

    return vbo_id;
}

void vbo_bind(vbo_t vbo_id) { glBindBuffer(GL_ARRAY_BUFFER, vbo_id); }

void vbo_unbind(void)
{
    // NOTE: Unbind the VBO *first*, to not accidentally write to them
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void vbo_delete(vbo_t vbo_id) { glDeleteBuffers(1, &vbo_id); }
