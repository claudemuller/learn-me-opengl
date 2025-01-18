#ifndef VBO_H
#define VBO_H

#include <GL/glew.h>

typedef struct {
    GLuint ID;
} vbo_t;

vbo_t *vbo_new(GLfloat *vertices, GLsizeiptr size);
void vbo_bind(vbo_t *vbo);
void vbo_unbind(void);
void vbo_delete(vbo_t **vbo);

#endif // !VBO_H
