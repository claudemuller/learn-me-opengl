#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

typedef struct {
    GLuint ID;
} shader_t;

shader_t *shader_new(const char *vertex_fname, const char *fragment_fname);
void shader_activate(shader_t *shader);
void shader_delete(shader_t **shader);

#endif // !SHADER_H
