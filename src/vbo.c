#include "vbo.h"
#include <stdio.h>
#include <stdlib.h>

// vbo_new creates a new Vertex Buffer Object in memory.
vbo_t *vbo_new(GLfloat *vertices, GLsizeiptr size)
{
    // // Create a Vertex Buffer Object in which to store vertex data to be sent to GPU
    vbo_t *vbo = malloc(sizeof(vbo_t));
    if (!vbo) {
        fprintf(stderr, "Error allocating memory for VBO\n");
        return NULL;
    }

    glGenBuffers(1, &vbo->ID);

    // Binding an object makes it the current context on which operations will execute
    glBindBuffer(GL_ARRAY_BUFFER, vbo->ID); // GL_ARRAY_BUFFER is the type needed for vertex buffer

    // Put the vertex data into the buffer
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    return vbo;
}

void vbo_bind(vbo_t *vbo) { glBindBuffer(GL_ARRAY_BUFFER, vbo->ID); }

void vbo_unbind(void)
{
    // Unbind the VBO *first*, to not accidentally write to them
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void vbo_delete(vbo_t **vbo)
{
    glDeleteBuffers(1, &((*vbo)->ID));
    free(*vbo);
    *vbo = NULL;
}
