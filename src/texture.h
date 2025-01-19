#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "shader.h"
#include <GL/glew.h>

typedef struct texture_t texture_t;
typedef struct texture_t {
    GLuint id;
    GLenum type;

    void (*unit)(shader_t shader_program, const char *uniform, GLuint unit);
    void (*bind)(texture_t *texture);
    void (*unbind)(texture_t *texture);
    void (*delete)(texture_t **texture);
} texture_t;

texture_t *texture_new(const char *image, GLenum tex_type, GLenum slot, GLenum format, GLenum pixel_type);

#endif // !TEXTURE_H_
