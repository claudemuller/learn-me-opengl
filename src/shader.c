#include "shader.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

static void check_shader_compile(GLuint shader);
static void check_program_link(GLuint program);

// Creates a shader and returns its ID.
shader_t shader_new(const char *vertex_fname, const char *fragment_fname)
{
    char *vertex_code = get_file_contents(vertex_fname);
    if (!vertex_code) {
        fprintf(stderr, "Error reading vertex code\n");
        exit(1);
    }

    char *fragment_code = get_file_contents(fragment_fname);
    if (!fragment_code) {
        fprintf(stderr, "Error reading fragment code\n");
        free(vertex_code);
        exit(1);
    }

    // Create reference in which to store vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar *vertex_code_arr[] = {vertex_code};
    glShaderSource(vertex_shader, 1, vertex_code_arr, NULL);
    check_gl_error("shader->vertex_shader->glShaderSource");

    // Compile the shader
    glCompileShader(vertex_shader);
    check_shader_compile(vertex_shader);

    // Create reference in which to store fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar *fragment_code_arr[] = {fragment_code};
    glShaderSource(fragment_shader, 1, fragment_code_arr, NULL);
    check_gl_error("shader->fragment_shader->glShaderSource");

    // Compile the shader
    glCompileShader(fragment_shader);
    check_shader_compile(fragment_shader);

    // Create a shader program to wrap these shaders up in
    shader_t shader_id = glCreateProgram();

    // Attach shaders to the shader program
    glAttachShader(shader_id, vertex_shader);
    check_gl_error("shader->vertex_shader->glAttachShader");
    glAttachShader(shader_id, fragment_shader);
    check_gl_error("shader->fragment_shader->glAttachShader");

    // Finish off the shader program
    glLinkProgram(shader_id);
    check_program_link(shader_id);

    // Clean up the shaders as they're in the program
    glDeleteShader(vertex_shader);
    check_gl_error("shader->vertex_shader->glDeleteShader");
    glDeleteShader(fragment_shader);
    check_gl_error("shader->fragment_shader->glDeleteShader");

    free(fragment_code);
    free(vertex_code);

    return shader_id;
}

void shader_activate(shader_t shader_id) { glUseProgram(shader_id); }

void shader_delete(shader_t shader_id) { glDeleteProgram(shader_id); }

// Checks for shader compilation errors.
static void check_shader_compile(GLuint shader)
{
    GLint compiled;
    GLchar info_log[1024];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE) {
        glGetShaderInfoLog(shader, 1024, NULL, info_log);
        fprintf(stderr, "Shader compilation failed: %s\n", info_log);
        exit(1);
    }
}

// Checks for program linking errors.
static void check_program_link(GLuint program)
{
    GLint linked;
    GLchar info_log[1024];

    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE) {
        glGetProgramInfoLog(program, 1024, NULL, info_log);
        fprintf(stderr, "Program linking failed: %s\n", info_log);
        exit(1);
    }
}
