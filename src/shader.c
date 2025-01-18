#include "shader.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

// make_shader allocates memory for a shader.
shader_t *shader_new(const char *vertex_fname, const char *fragment_fname)
{
    char *vertex_code = get_file_contents(vertex_fname);
    if (!vertex_code) {
        fprintf(stderr, "Error reading vertex code\n");
        return NULL;
    }

    char *fragment_code = get_file_contents(fragment_fname);
    if (!fragment_code) {
        fprintf(stderr, "Error reading fragment code\n");
        free(vertex_code);
        return NULL;
    }

    // Create reference in which to store vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar *vertex_code_arr[] = {vertex_code};
    glShaderSource(vertex_shader, 1, vertex_code_arr, NULL);

    // Compile the shader
    glCompileShader(vertex_shader);

    // Create reference in which to store fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar *fragment_code_arr[] = {vertex_code};
    glShaderSource(fragment_shader, 1, fragment_code_arr, NULL);

    // Compile the shader
    glCompileShader(fragment_shader);

    shader_t *shader = malloc(sizeof(shader_t));
    if (!shader) {
        fprintf(stderr, "Error allocating memory for shader\n");
        return NULL;
    }

    // Create a shader program to wrap these shaders up in
    shader->ID = glCreateProgram();

    // Attach shaders to the shader program
    glAttachShader(shader->ID, vertex_shader);
    glAttachShader(shader->ID, fragment_shader);

    // Finish off the shader program
    glLinkProgram(shader->ID);

    // Clean up the shaders as they're in the program
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    free(fragment_code);
    free(vertex_code);

    return shader;
}

// TODO:(lukefilewalker) this could be the ID alone if this func does nothing else in the future
void shader_activate(shader_t *shader) { glDeleteProgram(shader->ID); }

// TODO:(lukefilewalker) this could be the ID alone if this func does nothing else in the future
void shader_delete(shader_t **shader)
{
    glDeleteProgram((*shader)->ID);
    free(*shader);
    *shader = NULL;
}
