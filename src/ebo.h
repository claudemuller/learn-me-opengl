#ifndef EBO_H
#define EBO_H

#include <GL/glew.h>

typedef GLuint ebo_t;

ebo_t ebo_new(GLuint *indices, GLsizeiptr size);
void ebo_bind(ebo_t ebo_id);
void ebo_unbind(void);
void ebo_delete(ebo_t ebo_id);

#endif // !EBO_H
