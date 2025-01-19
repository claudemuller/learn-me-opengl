#include "texture.h"
#include "shader.h"
#include "utils.h"
#include <stb/stb_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static void _unit(shader_t shader_program, const char *uniform, GLuint unit);
static void _bind(texture_t *texture);
static void _unbind(texture_t *texture);
static void _delete(texture_t **texture);

texture_t *texture_new(const char *image, GLenum tex_type, GLenum slot, GLenum format, GLenum pixel_type)
{
    texture_t *texture = (texture_t *)malloc(sizeof(texture_t));
    if (!texture) {
        fprintf(stderr, "Error allocating memory for texture\n");
        return NULL;
    }
    texture->id = 0;
    texture->type = tex_type;

    texture->unit = _unit;
    texture->bind = _bind;
    texture->unbind = _unbind;
    texture->delete = _delete;

    // Store the width, height, and the number of color channels of the image
    int img_w, img_h, img_num_col_chans;
    // Flip the image so it appears right side up
    stbi_set_flip_vertically_on_load(true);
    // Read the image from a file and stores it in bytes
    unsigned char *bytes = stbi_load(image, &img_w, &img_h, &img_num_col_chans, 0);
    if (!bytes) {
        fprintf(stderr, "Error loading texture\n");
        exit(1);
    }

    // Generate an OpenGL texture object
    glGenTextures(1, &texture->id);
    check_gl_error("texture->glGenTextures");

    // Assign the texture to a Texture Unit
    glActiveTexture(slot);
    check_gl_error("texture->glActiveTexture");
    glBindTexture(tex_type, texture->id);
    check_gl_error("texture->glBindTexture");

    // Configure the type of algorithm that is used to make the image smaller or bigger
    glTexParameteri(tex_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    check_gl_error("texture->glTexParameteri(GL_TEXTURE_MIN_FILTER)");
    glTexParameteri(tex_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    check_gl_error("texture->glTexParameteri(GL_TEXTURE_MAG_FILTER)");

    // Configure the way the texture repeats (if it does at all)
    glTexParameteri(tex_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    check_gl_error("texture->glTexParameteri(GL_TEXTURE_WRAP_S)");
    glTexParameteri(tex_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    check_gl_error("texture->glTexParameteri(GL_TEXTURE_WRAP_T)");

    // Extra lines in case you choose to use GL_CLAMP_TO_BORDER
    // float flat_colour[] = {1.0f, 1.0f, 1.0f, 1.0f};
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flat_colour);

    // Assign the image to the OpenGL Texture object
    // For .png
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_w, img_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    // For .jpg
    glTexImage2D(tex_type, 0, format, img_w, img_h, 0, format, pixel_type, bytes);
    check_gl_error("texture->glTexImage2D");

    glGenerateMipmap(tex_type);
    check_gl_error("texture->glGenerateMipmap");

    stbi_image_free(bytes);

    // Unbind the OpenGL Texture object so that it can't accidentally be modified
    glBindTexture(tex_type, 0);
    check_gl_error("texture->glBindTexture");
    // texture_unbind(*texture);

    return texture;
}

static void _unit(shader_t shader_program, const char *uniform, GLuint unit)
{
    // Get the location of the uniform
    GLuint uni_tex = glGetUniformLocation(shader_program, uniform);
    check_gl_error("texture->glGetUniformLocation()");

    // Shader needs to be activated before changing the value of a uniform
    shader_activate(shader_program);

    // Sets the value of the uniform
    glUniform1i(uni_tex, unit);
    check_gl_error("texture->glUniform1f()");
}

static void _bind(texture_t *texture) { glBindTexture(texture->type, texture->id); }

static void _unbind(texture_t *texture) { glBindTexture(texture->type, 0); }

static void _delete(texture_t **texture)
{
    glDeleteTextures(1, &((*texture)->id));
    free(*texture);
    *texture = NULL;
}
