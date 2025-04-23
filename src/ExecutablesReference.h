
#pragma once

#include <NeftyRendererVK.h>

#ifndef EXECUTABLES_REFERENCE_H
#define EXECUTABLES_REFERENCE_H

namespace nefty {

    struct ExecutablesReference {
        NeftyContext context;
        GLFWwindow* window;


        ExecutablesReference();

        void update(NeftyContext * context, GLFWwindow* window);
        void render(NeftyContext * context);

        ~ExecutablesReference();

    };

    inline ExecutablesReference::ExecutablesReference() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        window = glfwCreateWindow(1200, 800, "Raytracing example", NULL, NULL);
    }

    inline ExecutablesReference::~ExecutablesReference() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }


    static void run(ExecutablesReference * renderer) {
        while (!glfwWindowShouldClose(renderer->window)) {
            renderer->update(&renderer->context, renderer->window);
            renderer->render(&renderer->context);
        }

    }
} // NEFTY



#endif //RAYTRACINGBASIC_H
