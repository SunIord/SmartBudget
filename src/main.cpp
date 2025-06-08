#include <GLFW/glfw3.h>
// #include <glad/glad.h> // Se estiver usando glad (opcional)
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <iostream>

int main() {
    // Inicializa GLFW
    if (!glfwInit()) {
        std::cerr << "Erro ao inicializar GLFW\n";
        return -1;
    }

    // Configura contexto OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "ImGui Exemplo", nullptr, nullptr);
    if (!window) {
        std::cerr << "Erro ao criar janela GLFW\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync

    // glad (opcional, só se estiver usando)
    // if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    //     std::cerr << "Erro ao inicializar GLAD\n";
    //     return -1;
    // }

    // Inicializa Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark(); // Tema escuro

    // Inicializa os backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Variáveis de estado
    bool show_bem_vindo = false;
    bool show_funciona = false;

    // Loop principal
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Minha Janela");

        if (ImGui::Button("Dizer Bem-vindo")) {
            show_bem_vindo = true;
        }

        if (ImGui::Button("Testar Botão")) {
            show_funciona = true;
        }

        if (show_bem_vindo)
            ImGui::Text("Bem-vindo!");

        if (show_funciona)
            ImGui::Text("Botão funciona.");

        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
