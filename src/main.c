#include "ebo.h"
#include "shader.h"
#include "utils.h"
#include "vao.h"
#include "vbo.h"
#include <GL/glew.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    // clang-format off
    GLfloat vertices[] = {
            // Co-ords are between -1 and 1 -      Coords               |        Colours
            -0.5f,   -0.5f * sqrtf(3.0f) / 3,      0.0f,  0.8f,  0.3f ,  0.02f, // Left corner
             0.5f,   -0.5f * sqrtf(3.0f) / 3,      0.0f,  0.8f, 0.3f,  0.02f, // Right corner
            0.0f,   0.5f * sqrtf(3.0f) * 2 / 3, 0.0f, 1.0f, 0.6f,  0.32f, // Top corner
           -0.25f,  0.5f * sqrtf(3.0f) / 6,     0.0f, 0.9f, 0.45f, 0.17f, // Left corner
            0.25f,  0.5f * sqrtf(3.0f) / 6,     0.0f, 0.9f, 0.45f, 0.17f, // Right corner
            0.0f,  -0.5f * sqrtf(3.0f) / 3,     0.0f, 0.8f, 0.3f,  0.02f, // Top corner
    };

    GLuint indices[] = {
        0, 3, 5, // Lower left triangle
        3, 2, 4, // Lower right triangle
        5, 4, 1  // Upper triangle
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
    vao_link_attr(vbo1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void *)0);
    vao_link_attr(vbo1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void *)(3 * sizeof(float)));

    // Unbind all to prevent accidentially modifying them
    vao_unbind();
    vbo_unbind();
    ebo_unbind();

    GLuint uni_id = glGetUniformLocation(shader_program, "scale");
    check_gl_error("main->glGetUniformLocation");

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Activate the shader program
        shader_activate(shader_program);

        glUniform1f(uni_id, 0.5f);
        check_gl_error("main->glUniform1f");

        // Bind to VAO to tell OpenGL which is the current VAO (optional here because we only have 1)
        vao_bind(vao1);

        // Draw using the array of vertices i.e. without the EBO
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        // Draw using the EBO
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        // Swap front and back buffers to update the image on each frame
        glfwSwapBuffers(window);

        // Poll for events
        glfwPollEvents();
    }

    // Delete objects
    vao_delete(vao1);
    vbo_delete(vbo1);
    ebo_delete(ebo1);
    shader_delete(shader_program);

    // Destroy the window
    glfwDestroyWindow(window);
    // Terminate GLFW window

    glfwTerminate();

    return 0;
}
