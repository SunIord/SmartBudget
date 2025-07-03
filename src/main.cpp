#include <GLFW/glfw3.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <iostream>
#include <string>

#ifdef _WIN32
#include <Windows.h>
#include <windowsx.h>
#include <Dwmapi.h>
#endif

#include "../include/transaction.hpp"
#include "../include/transactionManager.hpp"
#include "../include/budgetAnalyzer.hpp"
#include "../include/fileManager.hpp"

int main()
{
    TransactionManager manager;
    BudgetAnalyzer analyzer;
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
        static std::string filename;
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
            ImGui::Spacing();
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
                selected_menu = 5; // Muda para o menu de listar transações
            }
            if( ImGui::Button("Remover Transação"))
            {
                selected_menu = 6;  // Muda para o menu de editar transação
            }
            ImGui::Spacing();
            if (ImGui::Button("Voltar"))
            {
                selected_menu = 0; // Volta ao menu principal
            }
            ImGui::End();
            break;
        case 2:
            static double total = 0;
            static int balance_type = 0;
            static char input_balance_type[42] = "";
            static char balance_name[2][10] = {"categoria", "tipo"};
            ImGui::Begin("Relatório", nullptr, window_flags);
            ImGui::Text("Visualizar saldo total por categorias");
            ImGui::RadioButton("Categoria", &balance_type, 0);
            ImGui::SameLine();
            ImGui::RadioButton("Tipo", &balance_type, 1);
            ImGui::InputText("Tipo/Categoria", input_balance_type, IM_ARRAYSIZE(input_balance_type));
            if(ImGui::Button("Gerar saldo total")){
                if(balance_type == 0)
                {
                    //total =  BudgetAnalyzer::analyzer.calculateTotalByCategory(TransactionManager::manager.getTransactions(), input_balance_type);
                }else if(balance_type == 1) {
                    //total = BudgetAnalyzer::analyzer.calculateTotalByType(TransactionManager::manager.getTransactions(), input_balance_type);
                }    
            }
            ImGui::Text("Total por %s: %f", balance_name[balance_type], total);
            ImGui::Spacing();
            if (ImGui::Button("Voltar"))
            {
                selected_menu = 0; // Volta ao menu principal
            }
            ImGui::End();
            break;
        case 3:
            ImGui::Begin("Gerenciar Conta", nullptr, window_flags);
            ImGui::Text("Gerenciar sua conta.");
            if(ImGui::Button("Criar conta"))
            {
                selected_menu = 7; // menu de criação de conta
            }
            if (ImGui::Button("Deletar conta"))
            {
                selected_menu = 8;
            }
            if( ImGui::Button("Trocar Conta"))
            {
                selected_menu = 9;
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
            ImGui::Spacing();
            if (ImGui::Button("Voltar"))
            {
                selected_menu = 1; // Volta ao menu de transações
            }
            ImGui::End();
            break;
        case 5:
            static int transaction_type = 0;
            static char input_initial_date[10] = "";
            static char input_final_date[10] = "";
            static char input_minimum_value[30] = "";
            static char input_maximum_value[30] = "";
            // char transaction_type_name[2][10] = {"data", "valor"}
            
            ImGui::Begin("Listar Transações", nullptr, window_flags);
            ImGui::Text("Listar Transações");
            ImGui::RadioButton("Data", &transaction_type, 0);
            ImGui::SameLine();
            ImGui::RadioButton("Quantidade", &transaction_type, 1);
            if(transaction_type == 0)
            {
                ImGui::InputText("Data inicial", input_initial_date, IM_ARRAYSIZE(input_initial_date));
                ImGui::InputText("Data final", input_final_date, IM_ARRAYSIZE(input_final_date));
                if(ImGui::Button("Listar Transações")){
                    //auto results = manager.filterByDateRange(input_initial_date, input_final_date);
                }
            }
            if(transaction_type == 1)
            {
                ImGui::InputText("Valor mínimo", input_minimum_value, IM_ARRAYSIZE(input_minimum_value));
                ImGui::InputText("Valor máximo", input_maximum_value, IM_ARRAYSIZE(input_maximum_value));
                if(ImGui::Button("Listar Transações")){
                    //auto results = manager.filterByDateRange(input_minimum_value,input_maximum_value);
                }
            }
            ImGui::Spacing();
            if (ImGui::Button("Voltar"))
            {
                selected_menu = 1; // Volta ao menu anterior
            }
            ImGui::End();
            break;
        case 6:
            ImGui::Begin("Remover Transações", nullptr, window_flags);
            ImGui::Spacing();
            if (ImGui::Button("Voltar"))
            {
                selected_menu = 1; // Volta ao menu principal
            }
            ImGui::End();
            break;
        case 7:
            static char filename_input[20] = "";
            ImGui::Begin("Criar Conta", nullptr, window_flags);
            ImGui::InputTextWithHint("Nome", "Nome", filename_input,IM_ARRAYSIZE(filename_input));
            if(ImGui::Button("Gerar Arquivo")){
                filename = filename_input;
                filename = FileManager::generateUniqueFilename(filename);
                ImGui::OpenPopup("confirm_archive_generation");
            }
            if(ImGui::BeginPopup("confirm_archive_generation")){
                ImGui::TextWrapped("Arquivo gerado com sucesso!");
                if (ImGui::Button("Fechar"))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }
            ImGui::Spacing();
            if (ImGui::Button("Voltar"))
            {
                selected_menu = 3; // Volta ao Gerenciamento de conta
            }
            ImGui::End();
            break;
        case 8:
            ImGui::Begin("Deletar Conta", nullptr, window_flags);
            ImGui::Spacing();
            if (ImGui::Button("Voltar"))
            {
                selected_menu = 3; // Volta ao Gerenciamento de conta
            }
            ImGui::End();
            break;
        case 9:
            ImGui::Begin("Trocar Conta", nullptr, window_flags);
            
            ImGui::Spacing();
            if (ImGui::Button("Voltar"))
            {
                selected_menu = 3; // Volta ao Gerenciamento de conta
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
