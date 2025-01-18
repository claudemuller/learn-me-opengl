#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

typedef GLuint shader_t;

shader_t shader_new(const char *vertex_fname, const char *fragment_fname);
void shader_activate(shader_t shader_id);
void shader_delete(shader_t shader_id);

#endif // !SHADER_H
