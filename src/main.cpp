#define GLFW_DLL
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <Dwmapi.h>
#include <windowsx.h>
#include "../include/transaction.hpp"
#include "../include/transactionManager.hpp"
#include "../include/budgetAnalyzer.hpp"

int main()
{
    smartbudget::TransactionManager manager;
    smartbudget::BudgetAnalyzer analyzer;
    // Inicializa GLFW
    if (!glfwInit())
    {
        std::cerr << "Erro ao inicializar GLFW\n";
        return -1;
    }

    // Configura contexto OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // Torna a janela sem borda

    GLFWwindow *window = glfwCreateWindow(320, 460, "SmartBudget", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Erro ao criar janela GLFW\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync

    // Inicializa Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark(); // Tema escuro

    // Inicializa os backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Variáveis de estado
    int selected_menu = 0; // Variável para controlar o menu selecionado
    /*
     * 0 - Menu Principal
     * 1 - Menu de Transações
     * 2 - Menu de Relatório
     * 3 - Adicionar Transação
     * 4 - Listar Transações
     * 5 - Visualizar Saldo
     * 6 - Calcular Saldo
     */
    // Loop principal
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;

#ifdef IMGUI_HAS_VIEWPORT
        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
#else
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
#endif

        // Janela principal
        switch (selected_menu)
        {
        case 0:
            ImGui::Begin("Menu Principal", nullptr, window_flags);
            // ImGui::ShowDemoWindow(); // Exibe a janela de demonstração do ImGui

            if (ImGui::Button("Transações"))
            {
                selected_menu = 1; // Muda para o menu de transações
            }
            ImGui::SameLine();
            if (ImGui::Button("Relatório"))
            {
                selected_menu = 2; // Muda para o menu de saldo
            }
            if(ImGui::Button("Gerenciar Conta"))
            {
                selected_menu = 3; // Muda para o menu de gerenciar conta
            }
            if (ImGui::Button("Sair"))
            {
                glfwSetWindowShouldClose(window, true);
            }
            ImGui::End();
            break;
        case 1:
            ImGui::Begin("Gerenciar Transações", nullptr, window_flags);
            ImGui::Text("Gerenciar suas transações financeiras.");
            if (ImGui::Button("Adicionar Transação"))
            {
                // Aqui você pode adicionar a lógica para adicionar uma transação
                selected_menu = 4; // Muda para o menu de adicionar transação
            }
            if (ImGui::Button("Listar Transações"))
            {
                // Muda para o menu de listar transações
            }
            if( ImGui::Button("Remover Transação"))
            {
                 // Muda para o menu de editar transação
            }
            if (ImGui::Button("Voltar"))
            {
                selected_menu = 0; // Volta ao menu principal
            }
            ImGui::End();
            break;
        case 2:
            ImGui::Begin("Relatório", nullptr, window_flags);
            ImGui::Text("Visualizar saldo total por categorias");
            if (ImGui::Button("Total por Categoria"))
            {
                // Aqui você pode adicionar a lógica para calcular o total por categoria
                ImGui::Text("Total por categoria: R$ 500,00"); // Exemplo estático
            }
            if (ImGui::Button("Total por tipo"))
            {
                // Aqui você pode adicionar a lógica para calcular o total por tipo (renda/despesa)
                ImGui::Text("Total por tipo: R$ 300,00"); // Exemplo estático
            }
            if{ (ImGui::Button("Filtrar por data"))
            {
                
            }
            if{ (ImGui::Button("Filtrar por valor"))
            {
                
            }
            if (ImGui::Button("Voltar"))
            {
                selected_menu = 0; // Volta ao menu principal
            }
            ImGui::End();
            break;
        case 3:
            ImGui::Begin("Gerenciar Conta", nullptr, window_flags);
            ImGui::Text("Gerenciar sua conta.");
            if (ImGui::Button("Deletar conta"))
            {
                // Aqui você pode adicionar a lógica para remover o arquivo de transações
                // Exemplo: manager.removeTransactionsFile();
                ImGui::Text("Arquivo de transações removido com sucesso!");
            }
            if( ImGui::Button("Trocar Conta"))
            {

            }
            if (ImGui::Button("Voltar"))
            {
                selected_menu = 0; // Volta ao menu principal
            }
            ImGui::End();
            break;
        case 4:
            ImGui::Begin("Adicionar Transação", nullptr, window_flags);
            ImGui::Text("Adicionar uma nova transação.");
            static char amount[64] = "";
            static char type[64] = "";
            static char category[64] = "";
            static char date[64] = "";
            static char description[256] = "";
            static int radio_selection = 0;

            ImGui::InputTextWithHint("Valor", "R$0,00", amount, IM_ARRAYSIZE(amount), ImGuiInputTextFlags_CharsDecimal);
            ImGui::RadioButton("Despesa", &radio_selection, 0);
            ImGui::SameLine();
            ImGui::RadioButton("Renda", &radio_selection, 1);
            // ImGui::InputText("Tipo (renda/despesa)", type, IM_ARRAYSIZE(type));
            ImGui::InputTextWithHint("Categoria", "Saúde, Lazer, Casa...", category, IM_ARRAYSIZE(category));
            ImGui::InputTextWithHint("Data", "(YYYY-MM-DD)", date, IM_ARRAYSIZE(date));
            ImGui::InputTextMultiline("Descrição", description, IM_ARRAYSIZE(description));
            static int clicked = 0;
            if (ImGui::Button("Salvar"))
            {
                // Aqui você pode adicionar a lógica para salvar a transação
                // smartbudget::Transaction nova(std::stod(amount), type, category, date, description);
                // manager.addTransaction(nova);

                clicked++;
                if (clicked & 1)
                {
                    ImGui::SameLine();
                    ImGui::Text("Transação adicionada com sucesso!");
                }

                // Limpa os campos após salvar
                memset(amount, 0, sizeof(amount));
                memset(type, 0, sizeof(type));
                memset(category, 0, sizeof(category));
                memset(date, 0, sizeof(date));
                memset(description, 0, sizeof(description));
            }
            if (ImGui::Button("Voltar"))
            {
                selected_menu = 1; // Volta ao menu de transações
            }
            ImGui::End();
            break;
        }

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
