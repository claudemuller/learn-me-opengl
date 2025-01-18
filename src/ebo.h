#ifndef EBO_H
#define EBO_H

#include <GL/glew.h>

typedef struct {
    GLuint ID;
} ebo_t;

ebo_t *ebo_new(GLuint *indices, GLsizeiptr size);
void ebo_bind(ebo_t *ebo);
void ebo_unbind(void);
void ebo_delete(ebo_t **ebo);

#endif // !EBO_H
