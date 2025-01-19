#include "vao.h"
#include "utils.h"
#include "vbo.h"

// vao_new creates a new Vertex Array Object in memory.
vao_t vao_new(void)
{
    // The Vertex Array Object stores references to one or more VBOs and to quickly switch between VBOs
    vao_t vao_id;

    // The VAO must be generated *before* the VBO
    glGenVertexArrays(1, &vao_id);
    check_gl_error("vao->glGenVertexArrays");

    return vao_id;
}

void vao_link_attr(vbo_t vbo_id, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void *offset)
{
    vbo_bind(vbo_id);

    // A vertex attribute is a way to communicate to a vertex shader
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    check_gl_error("vao->glVertexAttribPointer");

    glEnableVertexAttribArray(layout);
    check_gl_error("vao->glEnableVertexAttribArray");

    vbo_unbind();
}
void vao_bind(vao_t vao_id) { glBindVertexArray(vao_id); }

void vao_unbind(void)
{
    // NOTE: Unbind the VAO *after* to not accidentally write to them
    glBindVertexArray(0);
}

void vao_delete(vao_t vao_id) { glDeleteVertexArrays(1, &vao_id); }
