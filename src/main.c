#include "ebo.h"
#include "shader.h"
#include "texture.h"
#include "utils.h"
#include "vao.h"
#include "vbo.h"
#include <GL/glew.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    // clang-format off
    GLfloat vertices[] = {
     //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f,  -0.5f,   0.0f,      1.0f,  0.0f,  0.0f,      0.0f,  0.0f, // Lower left corner
	-0.5f,  0.5f,   0.0f,     0.0f, 1.0f, 0.0f,     0.0f, 1.0f, // Upper left corner
	0.5f,  0.5f,  0.0f,     0.0f, 0.0f, 1.0f,     1.0f, 1.0f, // Upper right corner
	0.5f,  -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,     1.0f, 0.0f  // Lower right corner
    };

    GLuint indices[] = {
        0, 2, 1, // Upper triangle
        0, 3, 2  // Lower triangle
    };
    // clang-format on

    // Init GLFW window
    if (!glfwInit()) {
        fprintf(stderr, "Error initialising GLFW\n");
        return 1;
    }

    // Let GLFW know what version of OpenGL we want to use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Set the profile - 'core' gives the modern features
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 800, "Learn me OpenGL", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Error creating window\n");
        glfwTerminate();
        return 1;
    }

    // Set window into the current OpenGL context
    glfwMakeContextCurrent(window);

    // Init OpenGL
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Error initialising OpenGL\n");
        glfwTerminate();
        return 1;
    }

    // x, y of bottom left of window -> x, y of top right of the window
    glViewport(0, 0, 800, 800);

    shader_t shader_program = shader_new("res/shaders/default.vert", "res/shaders/default.frag");
    if (!shader_program) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

    vao_t vao1 = vao_new();
    vao_bind(vao1);

    // Create Vertex Buffer Object and link to vertices
    vbo_t vbo1 = vbo_new(vertices, sizeof(vertices));
    // Create Element Buffer Object and link to indices
    ebo_t ebo1 = ebo_new(indices, sizeof(indices));

    // Links VBO to VAO
    vao_link_attr(vbo1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0);
    vao_link_attr(vbo1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    vao_link_attr(vbo1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *)(6 * sizeof(float)));

    // Unbind all to prevent accidentially modifying them
    vao_unbind();
    vbo_unbind();
    ebo_unbind();

    // Gets ID of uniform called "scale"
    GLuint uni_id = glGetUniformLocation(shader_program, "scale");
    check_gl_error("main->glGetUniformLocation(scale)");

    // Create a texture
    texture_t *meme_thing = texture_new("res/image.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    meme_thing->unit(shader_program, "tex0", 0);

    while (!glfwWindowShouldClose(window)) {
        // Background colour
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT);

        // Activate the shader program i.e. tell OpenGL which Shader Program it should use
        shader_activate(shader_program);

        // Assigns a value to the uniform
        // NOTE: Must always be done after activating the Shader Program
        glUniform1f(uni_id, 0.5f);
        // check_gl_error("main->glUniform1f(scale)");

        // Binds texture so that is appears in rendering
        meme_thing->bind(meme_thing);

        // Bind to VAO to tell OpenGL which is the current VAO
        vao_bind(vao1);

        // Draw using the array of vertices i.e. without the EBO
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        // Draw primitives, number of indices, datatype of indices, index of indices using EBO
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap front and back buffers to update the image on each frame
        glfwSwapBuffers(window);

        // Poll for events
        glfwPollEvents();
    }

    // Delete objects
    vao_delete(vao1);
    vbo_delete(vbo1);
    ebo_delete(ebo1);
    meme_thing->delete(&meme_thing);
    shader_delete(shader_program);

    // Destroy the window
    glfwDestroyWindow(window);
    // Terminate GLFW window

    glfwTerminate();

    return 0;
}
