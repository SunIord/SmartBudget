#include <GLFW/glfw3.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#ifdef _WIN32
#include <Windows.h>
#include <Dwmapi.h>
#include <windowsx.h>
#endif

#include <map>
#include "appUI.hpp"

int main()
{
    // Inicialização do GLFW
    if (!glfwInit())
    {
        std::cerr << "Erro ao inicializar GLFW\n";
        return -1;
    }

    // Configuração da janela
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(320, 460, "SmartBudget", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Variáveis de estado
    TransactionManager manager;
    BudgetAnalyzer analyzer;
    FileManager fileManager;
    std::string filename;
    bool showFileDialog = true;
    int fileDialogOption = 0;
    char newFilename[256] = "";
    int selectedMenu = 0;
    double currentBalance = 0.0;
    bool showBalance = false;
    static char amount[64] = "";
    static char category[64] = "";
    static char date[64] = "";
    static char description[256] = "";
    static int transactionType = 0;

    // Loop principal
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Diálogo de seleção de arquivo
        if (showFileDialog)
        {
            ImGui::OpenPopup("Seleção de Arquivo");
            if (ImGui::BeginPopupModal("Seleção de Arquivo", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Escolha uma opção:");
                
                if (ImGui::Button("Criar Novo Arquivo"))
                {
                    fileDialogOption = 1;
                    memset(newFilename, 0, sizeof(newFilename));
                }
                
                ImGui::SameLine();
                
                if (ImGui::Button("Carregar Arquivo Existente"))
                {
                    fileDialogOption = 2;
                }

                if (fileDialogOption == 1)
                {
                    ImGui::InputText("Nome do Arquivo", newFilename, IM_ARRAYSIZE(newFilename));
                    if (ImGui::Button("Criar") && strlen(newFilename) > 0)
                    {
                        filename = fileManager.generateUniqueFilename(newFilename);
                        showFileDialog = false;
                        ImGui::CloseCurrentPopup();
                    }
                }
                else if (fileDialogOption == 2)
                {
                    auto files = fileManager.listCSVFiles();
                    if (files.empty())
                    {
                        ImGui::Text("Nenhum arquivo CSV encontrado");
                    }
                    else
                    {
                        for (const auto& file : files)
                        {
                            if (ImGui::Selectable(file.c_str()))
                            {
                                filename = file;
                                std::vector<Transaction> loadedTransactions;
                                if (fileManager.loadFromFile(loadedTransactions, filename))
                                {
                                    manager = TransactionManager(); // Reset
                                    for (const auto& t : loadedTransactions)
                                    {
                                        manager.addTransaction(t);
                                    }
                                }
                                showFileDialog = false;
                                ImGui::CloseCurrentPopup();
                            }
                        }
                    }
                }

                ImGui::EndPopup();
            }
        }
        else
        {
            // Menus principais
            switch (selectedMenu)
            {
            case 0: // Menu Principal
                {
                    ImGui::Begin("Menu Principal", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
                    
                    if (!filename.empty())
                        ImGui::Text("Arquivo: %s", filename.c_str());
                    
                    if (ImGui::Button("Transações", ImVec2(200, 50)))
                        selectedMenu = 1;
                    
                    if (ImGui::Button("Relatórios", ImVec2(200, 50)))
                        selectedMenu = 2;
                    
                    if (ImGui::Button("Trocar Arquivo", ImVec2(200, 50)))
                        showFileDialog = true;

                    if (ImGui::Button("Excluir Arquivo", ImVec2(200, 50)))
                        selectedMenu = 12;

                    if (ImGui::Button("Sair", ImVec2(200, 50)))
                        glfwSetWindowShouldClose(window, true); 
                    
                    ImGui::End();
                }
                break;

            case 1: // Menu Transações
                {
                    ImGui::Begin("Transações", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
                    
                    if (ImGui::Button("Adicionar Transação"))
                    {
                        selectedMenu = 3;
                        memset(amount, 0, sizeof(amount));
                        memset(category, 0, sizeof(category));
                        memset(date, 0, sizeof(date));
                        memset(description, 0, sizeof(description));
                    }
                    
                    if (ImGui::Button("Listar Transações"))
                        selectedMenu = 4;

                    if (ImGui::Button("Editar Transação"))
                        selectedMenu = 7;

                    if (ImGui::Button("Remover Transação"))
                        selectedMenu = 8;

                    if (ImGui::Button("Voltar"))
                        selectedMenu = 0;
                    
                    ImGui::End();
                }
                break;

            case 2: // Menu Relatórios
            {
                ImGui::Begin("Relatórios", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

                if (ImGui::Button("Saldo Total"))
                {
                    currentBalance = analyzer.calculateBalance(manager.getTransactions());
                    selectedMenu = 6;
                }

                if (ImGui::Button("Total por Categoria"))
                    selectedMenu = 5;

                if (ImGui::Button("Filtrar por Data"))
                    selectedMenu = 10;

                if (ImGui::Button("Filtrar por Valor"))
                    selectedMenu = 9;

                if (ImGui::Button("Total por Tipo"))
                    selectedMenu = 11;

                if (ImGui::Button("Voltar"))
                    selectedMenu = 0;

                ImGui::End();
            }
            break;

            case 3: // Adicionar Transação
                {
                    ImGui::Begin("Nova Transação", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
                    
                    ImGui::InputText("Valor (R$)", amount, IM_ARRAYSIZE(amount), ImGuiInputTextFlags_CharsDecimal);
                    ImGui::RadioButton("Despesa", &transactionType, 0); ImGui::SameLine();
                    ImGui::RadioButton("Renda", &transactionType, 1);
                    ImGui::InputText("Categoria", category, IM_ARRAYSIZE(category));
                    ImGui::InputText("Data (AAAA-MM-DD)", date, IM_ARRAYSIZE(date));
                    ImGui::InputTextMultiline("Descrição", description, IM_ARRAYSIZE(description));
                    
                    if (ImGui::Button("Salvar") && strlen(amount) > 0)
                    {
                        Transaction newTrans(
                            std::stod(amount),
                            (transactionType == 0) ? "despesa" : "renda",
                            category,
                            date,
                            description
                        );
                        manager.addTransaction(newTrans);
                        fileManager.saveToFile(manager.getTransactions(), filename);
                        selectedMenu = 1;
                    }
                    
                    ImGui::SameLine();
                    if (ImGui::Button("Cancelar"))
                        selectedMenu = 1;
                    
                    ImGui::End();
                }
                break;

            case 4: // Listar Transações
                {
                    ImGui::Begin("Lista de Transações", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
                    
                    const auto& transactions = manager.getTransactions();
                    if (transactions.empty())
                    {
                        ImGui::Text("Nenhuma transação cadastrada.");
                    }
                    else
                    {
                        if (ImGui::BeginTable("Transações", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
                        {
                            ImGui::TableSetupColumn("ID");
                            ImGui::TableSetupColumn("Valor");
                            ImGui::TableSetupColumn("Tipo");
                            ImGui::TableSetupColumn("Data");
                            ImGui::TableSetupColumn("Categoria");
                            ImGui::TableHeadersRow();

                            for (size_t i = 0; i < transactions.size(); ++i)
                            {
                                const auto& t = transactions[i];
                                ImGui::TableNextRow();
                                ImGui::TableNextColumn(); ImGui::Text("%d", (int)i);
                                ImGui::TableNextColumn(); ImGui::Text("R$ %.2f", t.getAmount());
                                ImGui::TableNextColumn(); ImGui::Text("%s", t.getType().c_str());
                                ImGui::TableNextColumn(); ImGui::Text("%s", t.getDate().c_str());
                                ImGui::TableNextColumn(); ImGui::Text("%s", t.getCategory().c_str());
                            }
                            ImGui::EndTable();
                        }
                    }
                    
                    if (ImGui::Button("Voltar"))
                        selectedMenu = 1;
                    
                    ImGui::End();
                }
                break;

            case 5: // Relatório por Categoria
                {
                    ImGui::Begin("Totais por Categoria", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
                    
                    std::map<std::string, double> categoryTotals;
                    const auto& transactions = manager.getTransactions();
                    
                    for (const auto& t : transactions)
                    {
                        if (t.getType() == "despesa")
                            categoryTotals[t.getCategory()] -= t.getAmount();
                        else
                            categoryTotals[t.getCategory()] += t.getAmount();
                    }
                    
                    for (const auto& [cat, total] : categoryTotals)
                    {
                        ImGui::Text("%s: R$ %.2f", cat.c_str(), total);
                    }
                    
                    if (ImGui::Button("Voltar"))
                        selectedMenu = 2;
                    
                    ImGui::End();
                }
                break;

            case 6: // Exibir Saldo Total
                {
                    ImGui::Begin("Saldo Total", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

                    ImGui::Text("Saldo atual calculado:");
                    ImGui::Separator();
                    ImGui::Text("R$ %.2f", currentBalance);
                    ImGui::Separator();

                    if (ImGui::Button("Voltar"))
                        selectedMenu = 2;

                    ImGui::End();
                }
                break;

            case 7: // Editar Transação
            {
                ImGui::Begin("Editar Transação", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

                const auto& list = manager.getTransactions();
                static int indexToEdit = 0;

                if (list.empty()) {
                    ImGui::Text("Nenhuma transação cadastrada.");
                } else {
                    ImGui::InputInt("Índice da transação", &indexToEdit);
                    if (indexToEdit >= 0 && indexToEdit < static_cast<int>(list.size())) {
                        const auto& t = list[indexToEdit];

                        static char editAmount[64];
                        static char editCategory[64];
                        static char editDate[64];
                        static char editDescription[256];
                        static int editType = 0;

                        // Preencher campos na primeira entrada
                        if (ImGui::Button("Carregar dados")) {
                            snprintf(editAmount, 64, "%.2f", t.getAmount());
                            snprintf(editCategory, 64, "%s", t.getCategory().c_str());
                            snprintf(editDate, 64, "%s", t.getDate().c_str());
                            snprintf(editDescription, 256, "%s", t.getDescription().c_str());
                            editType = (t.getType() == "renda") ? 1 : 0;
                        }

                        ImGui::InputText("Novo Valor", editAmount, IM_ARRAYSIZE(editAmount), ImGuiInputTextFlags_CharsDecimal);
                        ImGui::RadioButton("Despesa", &editType, 0); ImGui::SameLine();
                        ImGui::RadioButton("Renda", &editType, 1);
                        ImGui::InputText("Nova Categoria", editCategory, IM_ARRAYSIZE(editCategory));
                        ImGui::InputText("Nova Data", editDate, IM_ARRAYSIZE(editDate));
                        ImGui::InputTextMultiline("Nova Descrição", editDescription, IM_ARRAYSIZE(editDescription));

                        if (ImGui::Button("Salvar Alterações")) {
                            Transaction updated(
                                std::stod(editAmount),
                                (editType == 0) ? "despesa" : "renda",
                                editCategory,
                                editDate,
                                editDescription
                            );
                            manager.updateTransaction(indexToEdit, updated);
                            fileManager.saveToFile(manager.getTransactions(), filename);
                            selectedMenu = 1;
                        }
                    }
                }

                if (ImGui::Button("Voltar"))
                    selectedMenu = 1;

                ImGui::End();
                }
                break;

            case 8: // Remover Transação
            {
                ImGui::Begin("Remover Transação", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

                const auto& list = manager.getTransactions();
                static int indexToRemove = 0;

                if (list.empty()) {
                    ImGui::Text("Nenhuma transação para remover.");
                } else {
                    ImGui::InputInt("Índice da transação", &indexToRemove);

                    if (indexToRemove >= 0 && indexToRemove < static_cast<int>(list.size())) {
                        const auto& t = list[indexToRemove];
                        ImGui::Text("Transação selecionada:");
                        ImGui::Text("Valor: R$ %.2f", t.getAmount());
                        ImGui::Text("Tipo: %s", t.getType().c_str());
                        ImGui::Text("Categoria: %s", t.getCategory().c_str());
                        ImGui::Text("Data: %s", t.getDate().c_str());
                        ImGui::TextWrapped("Descrição: %s", t.getDescription().c_str());

                        static bool confirm = false;
                        if (ImGui::Button("Confirmar Remoção")) {
                            manager.removeTransaction(indexToRemove);
                            fileManager.saveToFile(manager.getTransactions(), filename);
                            selectedMenu = 1;
                        }
                    }
                }

                if (ImGui::Button("Voltar"))
                    selectedMenu = 1;

                ImGui::End();
                }
                break;

                case 9: // Filtro por Valor
                {
                    ImGui::Begin("Filtrar por Valor", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

                    static char valorMinStr[64] = "";
                    static char valorMaxStr[64] = "";

                    ImGui::InputText("Valor mínimo", valorMinStr, IM_ARRAYSIZE(valorMinStr), ImGuiInputTextFlags_CharsDecimal);
                    ImGui::InputText("Valor máximo", valorMaxStr, IM_ARRAYSIZE(valorMaxStr), ImGuiInputTextFlags_CharsDecimal);

                    if (ImGui::Button("Filtrar") && strlen(valorMinStr) > 0 && strlen(valorMaxStr) > 0)
                    {
                        double minVal = std::stod(valorMinStr);
                        double maxVal = std::stod(valorMaxStr);
                        const auto& lista = manager.getTransactions();

                        for (const auto& t : lista)
                        {
                            double val = t.getAmount();
                            if (val >= minVal && val <= maxVal)
                            {
                                ImGui::Separator();
                                ImGui::Text("Valor: R$ %.2f", t.getAmount());
                                ImGui::Text("Tipo: %s", t.getType().c_str());
                                ImGui::Text("Categoria: %s", t.getCategory().c_str());
                                ImGui::Text("Data: %s", t.getDate().c_str());
                                ImGui::TextWrapped("Descrição: %s", t.getDescription().c_str());
                            }
                        }
                    }

                    if (ImGui::Button("Voltar"))
                        selectedMenu = 2;

                    ImGui::End();
                }
                break;

                case 10: // Filtro por Data
                {
                    ImGui::Begin("Filtrar por Data", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

                    static char dataFiltro[64] = "";
                    ImGui::InputText("Data (AAAA-MM-DD)", dataFiltro, IM_ARRAYSIZE(dataFiltro));

                    if (ImGui::Button("Filtrar") && strlen(dataFiltro) > 0)
                    {
                        const auto& lista = manager.getTransactions();
                        for (const auto& t : lista)
                        {
                            if (t.getDate() == dataFiltro)
                            {
                                ImGui::Separator();
                                ImGui::Text("Valor: R$ %.2f", t.getAmount());
                                ImGui::Text("Tipo: %s", t.getType().c_str());
                                ImGui::Text("Categoria: %s", t.getCategory().c_str());
                                ImGui::Text("Data: %s", t.getDate().c_str());
                                ImGui::TextWrapped("Descrição: %s", t.getDescription().c_str());
                            }
                        }
                    }

                    if (ImGui::Button("Voltar"))
                        selectedMenu = 2;

                    ImGui::End();
                }
                break;

                case 11: // Total por Tipo
                {
                    ImGui::Begin("Total por Tipo", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

                    static int tipoSelecionado = 0; // 0 = Despesa, 1 = Renda
                    ImGui::RadioButton("Despesas", &tipoSelecionado, 0); ImGui::SameLine();
                    ImGui::RadioButton("Rendas", &tipoSelecionado, 1);

                    std::string tipoStr = tipoSelecionado == 0 ? "despesa" : "renda";
                    double total = analyzer.calculateTotalByType(manager.getTransactions(), tipoStr);
                    ImGui::Text("Total de %s: R$ %.2f", tipoStr.c_str(), total);

                    if (ImGui::Button("Voltar"))
                        selectedMenu = 2;

                    ImGui::End();
                }
                break;

                case 12: // Excluir Arquivo
                {
                    ImGui::Begin("Excluir Arquivo", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

                    auto arquivos = fileManager.listCSVFiles();
                    static int selectedToDelete = -1;

                    if (arquivos.empty())
                    {
                        ImGui::Text("Nenhum arquivo .csv encontrado.");
                    }
                    else
                    {
                        for (int i = 0; i < (int)arquivos.size(); ++i)
                        {
                            if (ImGui::Selectable(arquivos[i].c_str(), selectedToDelete == i))
                                selectedToDelete = i;
                        }

                        if (selectedToDelete != -1)
                        {
                            ImGui::Separator();
                            ImGui::Text("Excluir '%s'?", arquivos[selectedToDelete].c_str());

                            if (ImGui::Button("Confirmar Exclusão"))
                            {
                                if (fileManager.deleteFile(arquivos[selectedToDelete]))
                                {
                                    ImGui::Text("Arquivo excluído com sucesso.");
                                    if (arquivos[selectedToDelete] == filename)
                                    {
                                        filename.clear();
                                        manager = TransactionManager();
                                    }
                                }
                                else
                                {
                                    ImGui::Text("Erro ao excluir arquivo.");
                                }
                                selectedToDelete = -1;
                            }
                        }
                    }

                    if (ImGui::Button("Voltar"))
                        selectedMenu = 0;

                    ImGui::End();
                }
                break;

 

            }
        }

        // Render
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
