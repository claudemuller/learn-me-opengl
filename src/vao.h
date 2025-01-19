#ifndef VAO_H_
#define VAO_H_

#include "vbo.h"
#include <GL/glew.h>

typedef GLuint vao_t;

vao_t vao_new(void);
void vao_link_attr(vbo_t vbo_id, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void *offset);
void vao_bind(vao_t vao_id);
void vao_unbind(void);
void vao_delete(vao_t vao_id);

#endif // !VAO_H_
