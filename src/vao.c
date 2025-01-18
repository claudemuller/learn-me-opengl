#include "vao.h"
#include "vbo.h"
#include <stdio.h>
#include <stdlib.h>

// vao_new creates a new Vertex Array Object in memory.
vao_t *vao_new(void)
{
    // The Vertex Array Object stores references to one or more VBOs and to quickly switch between VBOs
    vao_t *vao = malloc(sizeof(vao_t));
    if (!vao) {
        fprintf(stderr, "Error allocating memory for VAO\n");
        return NULL;
    }

    // The VAO must be generated *before* the VBO
    glGenVertexArrays(1, &vao->ID);

    return vao;
}

void vao_link_vbo(vbo_t *vbo, GLuint layout)
{
    vbo_bind(vbo);

    // A vertex attribute is a way to communicate to a vertex shader
    glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(layout);

    vbo_unbind();
}
void vao_bind(vao_t *vao) { glBindVertexArray(vao->ID); }

void vao_unbind(void)
{
    // Unbind the VAO *after* to not accidentally write to them
    glBindVertexArray(0);
}

void vao_delete(vao_t **vao)
{
    glDeleteVertexArrays(1, &((*vao)->ID));
    free(*vao);
    *vao = NULL;
}
