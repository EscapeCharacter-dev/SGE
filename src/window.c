#include "../lib/glad/glad.h"
#include <GLFW/glfw3.h>
#include "../include/sge.h"
#include "internal.h"
#include <pthread.h>
#include <stdio.h>

static GLFWwindow *window = 0;

static void resize(GLFWwindow *window, int w, int h) {
	(void)window;
    glViewport(0, 0, w, h);
}

static void on_error(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar *message,
    const void *userParam) {
    (void)length;
    (void)source;
    (void)userParam;

    putc('<', stdout);
    printf("%d: ", id);
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        printf("ERROR");
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        printf("DEPRECATED BEHAVIOUR");
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        printf("UNDEFINED BEHAVIOUR");
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        printf("PORTABILITY");
        break;
    case GL_DEBUG_TYPE_OTHER:
        printf("OTHER");
        break;
    default:
        printf("UNKNOWN");
        break;
    }
    printf("> ");
    switch (severity) {
    case GL_DEBUG_SEVERITY_NOTIFICATION: printf("{NOTIFICATION}"); break;
    case GL_DEBUG_SEVERITY_LOW: printf("{LOW}"); break;
    case GL_DEBUG_SEVERITY_MEDIUM: printf("{MEDIUM}"); break;
    case GL_DEBUG_SEVERITY_HIGH: printf("{HIGH}"); break;
    default: printf("UNKNOWN"); break;
    }
    putc(' ', stdout);
    printf("%s\n", message);
}

PUBLIC void sge_init(sge_init_struct init_struct) {
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

    glDebugMessageCallback(&on_error, 0);
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

    glfwDestroyWindow(window);
    glfwTerminate();
}
