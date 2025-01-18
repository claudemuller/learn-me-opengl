#ifndef VAO_H
#define VAO_H

#include "vbo.h"
#include <GL/glew.h>

typedef struct {
    GLuint ID;
} vao_t;

vao_t *vao_new(void);
void vao_link_vbo(vbo_t *vbo, GLuint layout);
void vao_bind(vao_t *vao);
void vao_unbind(void);
void vao_delete(vao_t **vao);

#endif // !VAO_H
