#include <glfw3/glfw3.h>
#include "glad.h"
#include "sge.h"
#include "internal.h"
#include <pthread.h>

static GLFWwindow *window = 0;

static void resize(GLFWwindow *window, int w, int h) {
    glViewport(0, 0, w, h);
}

PUBLIC void sge_init(struct sge_init_struct init_struct) {
    if (window) {
        return;
    }
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    window = glfwCreateWindow(
        init_struct.wW,
        init_struct.wH,
        init_struct.title,
        init_struct.fs ? glfwGetPrimaryMonitor() : 0,
        0
    );
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)&glfwGetProcAddress);
    glfwSetFramebufferSizeCallback(window, &resize);

    glViewport(0, 0, init_struct.vW, init_struct.vH);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1, 0, 1, 1); /* test color */
}

PUBLIC void sge_start(sge_update_callback callback) {
    uint64_t ticks = 0;
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0, 0, 0, 1);
        callback(ticks++);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

PUBLIC void sge_end(void) {
    if (!window) {
        return;
    }
    sge_free_shaders();

    glfwDestroyWindow(window);
    glfwTerminate();
}
