#include "appUI.hpp"

AppUI::AppUI(GLFWwindow* window)
    : window(window), showFileDialog(true), fileDialogOption(0), selectedMenu(0),
    currentBalance(0.0), transactionType(0)
{
    memset(newFilename, 0, sizeof(newFilename));
    memset(amount, 0, sizeof(amount));
    memset(category, 0, sizeof(category));
    memset(date, 0, sizeof(date));
    memset(description, 0, sizeof(description));
}

void AppUI::render() {
    if (showFileDialog && !ImGui::IsPopupOpen("Seleção de Arquivo"))
        ImGui::OpenPopup("Seleção de Arquivo");

    if (showFileDialog) {
        renderFileDialog();
    } else {
        switch (selectedMenu) {
            case 0: renderMainMenu(); break;
            case 1: renderTransactionMenu(); break;
            case 2: renderReportsMenu(); break;
            case 3: renderAddTransaction(); break;
            case 4: renderListTransactions(); break;
            case 5: renderCategoryTotals(); break;
            case 6: renderBalanceScreen(); break;
            case 7: renderEditTransaction(); break;
            case 8: renderRemoveTransaction(); break;
            case 9: renderValueFilter(); break;
            case 10: renderDateFilter(); break;
            case 11: renderTypeTotals(); break;
            case 12: renderFileDeletion(); break;
        }
    }
}

// Variáveis para tamanho e posição da tela
int width = 320, height = 460;
int x = 0, y = 0;
// Definição do tamanho dos botões
ImVec2 buttonSize(200, 30);

void AppUI::renderFileDialog() {
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);

    if (ImGui::BeginPopupModal("Seleção de Arquivo", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
        ImGui::Text("Escolha uma opção:");

        if (ImGui::Button("Criar Novo Arquivo", buttonSize)) {
            fileDialogOption = 1;
            memset(newFilename, 0, sizeof(newFilename));
        }

        if (ImGui::Button("Carregar Arquivo Existente", buttonSize)) {
            fileDialogOption = 2;
        }

        if (fileDialogOption == 1) {
            ImGui::InputText("Nome do Arquivo", newFilename, IM_ARRAYSIZE(newFilename));
            if (ImGui::Button("Criar", buttonSize) && strlen(newFilename) > 0) {
                filename = fileManager.generateUniqueFilename(newFilename);
                showFileDialog = false;
                ImGui::CloseCurrentPopup();
            }
        } else if (fileDialogOption == 2) {
            auto files = fileManager.listCSVFiles();
            if (files.empty()) {
                ImGui::Text("Nenhum arquivo CSV encontrado");
            } else {
                for (const auto& file : files) {
                    if (ImGui::Selectable(file.c_str())) {
                        filename = file;
                        std::vector<Transaction> loadedTransactions;
                        if (fileManager.loadFromFile(loadedTransactions, filename)) {
                            manager = TransactionManager();
                            for (const auto& t : loadedTransactions) {
                                manager.addTransaction(t);
                            }
                        }
                        showFileDialog = false;
                        ImGui::CloseCurrentPopup();
                    }
                }
            }
        }
        
        if (ImGui::Button("Sair do Programa", buttonSize)) {
            glfwSetWindowShouldClose(window, true);
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void AppUI::renderMainMenu() {
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
    ImGui::Begin("Menu Principal", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    if (!filename.empty())
        ImGui::Text("Arquivo: %s", filename.c_str());

    if (ImGui::Button("Transações", buttonSize)) selectedMenu = 1;
    if (ImGui::Button("Relatórios", buttonSize)) selectedMenu = 2;
    if (ImGui::Button("Trocar Arquivo", buttonSize)) showFileDialog = true;
    if (ImGui::Button("Excluir Arquivo", buttonSize)) selectedMenu = 12;
    if (ImGui::Button("Sair", buttonSize)) glfwSetWindowShouldClose(window, true);
    ImGui::End();
}

void AppUI::renderTransactionMenu() {
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
    ImGui::Begin("Transações", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    if (ImGui::Button("Adicionar Transação", buttonSize)) {
        selectedMenu = 3;
        memset(amount, 0, sizeof(amount));
        memset(category, 0, sizeof(category));
        memset(date, 0, sizeof(date));
        memset(description, 0, sizeof(description));
        transactionType = 0;
    }

    if (ImGui::Button("Listar Transações", buttonSize)) selectedMenu = 4;
    if (ImGui::Button("Editar Transação", buttonSize)) selectedMenu = 7;
    if (ImGui::Button("Remover Transação", buttonSize)) selectedMenu = 8;
    if (ImGui::Button("Voltar", buttonSize)) selectedMenu = 0;

    ImGui::End();
}

void AppUI::renderAddTransaction() {
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
    ImGui::Begin("Nova Transação", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::InputText("Valor (R$)", amount, IM_ARRAYSIZE(amount), ImGuiInputTextFlags_CharsDecimal);
    ImGui::RadioButton("Despesa", &transactionType, 0); ImGui::SameLine();
    ImGui::RadioButton("Renda", &transactionType, 1);
    ImGui::InputText("Categoria", category, IM_ARRAYSIZE(category));
    ImGui::InputText("Data (AAAA-MM-DD)", date, IM_ARRAYSIZE(date));
    ImGui::InputTextMultiline("Descrição", description, IM_ARRAYSIZE(description));

    if (ImGui::Button("Salvar", buttonSize) && strlen(amount) > 0) {
        Transaction nova(std::stod(amount),
                        transactionType == 0 ? "despesa" : "renda",
                        category, date, description);
        manager.addTransaction(nova);
        fileManager.saveToFile(manager.getTransactions(), filename);
        selectedMenu = 1;
    }

    ImGui::SameLine();
    if (ImGui::Button("Cancelar", buttonSize)) selectedMenu = 1;

    ImGui::End();
}

void AppUI::renderListTransactions() {
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
    ImGui::Begin("Lista de Transações", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    const auto& list = manager.getTransactions();

    if (list.empty()) {
        ImGui::Text("Nenhuma transação cadastrada.");
    } else {
        if (ImGui::BeginTable("Transações", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            ImGui::TableSetupColumn("ID");
            ImGui::TableSetupColumn("Valor");
            ImGui::TableSetupColumn("Tipo");
            ImGui::TableSetupColumn("Data");
            ImGui::TableSetupColumn("Categoria");
            ImGui::TableHeadersRow();

            for (size_t i = 0; i < list.size(); ++i) {
                const auto& t = list[i];
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

    if (ImGui::Button("Voltar", buttonSize)) selectedMenu = 1;
    ImGui::End();
}

void AppUI::renderEditTransaction() {
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
    ImGui::Begin("Editar Transação", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    const auto& list = manager.getTransactions();
    static int indexToEdit = 0;

    if (list.empty()) {
        ImGui::Text("Nenhuma transação cadastrada.");
    } else {
        ImGui::InputInt("Índice da transação", &indexToEdit);
        if (indexToEdit >= 0 && indexToEdit < (int)list.size()) {
            const auto& t = list[indexToEdit];
            static char editAmount[64];
            static char editCategory[64];
            static char editDate[64];
            static char editDescription[256];
            static int editType = 0;

            if (ImGui::Button("Carregar dados", buttonSize)) {
                std::snprintf(editAmount, 64, "%.2f", t.getAmount());
                strncpy(editCategory, t.getCategory().c_str(), sizeof(editCategory));
                strncpy(editDate, t.getDate().c_str(), sizeof(editDate));
                strncpy(editDescription, t.getDescription().c_str(), sizeof(editDescription));
                editType = (t.getType() == "renda") ? 1 : 0;
            }

            ImGui::InputText("Novo Valor", editAmount, IM_ARRAYSIZE(editAmount), ImGuiInputTextFlags_CharsDecimal);
            ImGui::RadioButton("Despesa", &editType, 0); ImGui::SameLine();
            ImGui::RadioButton("Renda", &editType, 1);
            ImGui::InputText("Nova Categoria", editCategory, IM_ARRAYSIZE(editCategory));
            ImGui::InputText("Nova Data", editDate, IM_ARRAYSIZE(editDate));
            ImGui::InputTextMultiline("Nova Descrição", editDescription, IM_ARRAYSIZE(editDescription));

            if (ImGui::Button("Salvar Alterações", buttonSize)) {
                Transaction updated(
                    std::stod(editAmount),
                    editType == 0 ? "despesa" : "renda",
                    editCategory, editDate, editDescription
                );
                manager.updateTransaction(indexToEdit, updated);
                fileManager.saveToFile(manager.getTransactions(), filename);
                selectedMenu = 1;
            }
        }
    }

    if (ImGui::Button("Voltar", buttonSize)) selectedMenu = 1;

    ImGui::End();
}

void AppUI::renderRemoveTransaction() {
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
    ImGui::Begin("Remover Transação", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    const auto& list = manager.getTransactions();
    static int indexToRemove = 0;

    if (list.empty()) {
        ImGui::Text("Nenhuma transação para remover.");
    } else {
        ImGui::InputInt("Índice da transação", &indexToRemove);
        if (indexToRemove >= 0 && indexToRemove < (int)list.size()) {
            const auto& t = list[indexToRemove];
            ImGui::Text("Transação selecionada:");
            ImGui::Text("Valor: R$ %.2f", t.getAmount());
            ImGui::Text("Tipo: %s", t.getType().c_str());
            ImGui::Text("Categoria: %s", t.getCategory().c_str());
            ImGui::Text("Data: %s", t.getDate().c_str());
            ImGui::TextWrapped("Descrição: %s", t.getDescription().c_str());

            if (ImGui::Button("Confirmar Remoção", buttonSize)) {
                manager.removeTransaction(indexToRemove);
                fileManager.saveToFile(manager.getTransactions(), filename);
                selectedMenu = 1;
            }
        }
    }

    if (ImGui::Button("Voltar", buttonSize)) selectedMenu = 1;

    ImGui::End();
}

void AppUI::renderReportsMenu() {
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
    ImGui::Begin("Relatórios", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    if (ImGui::Button("Saldo Total", buttonSize)) {
        currentBalance = analyzer.calculateBalance(manager.getTransactions());
        selectedMenu = 6;
    }

    if (ImGui::Button("Total por Categoria", buttonSize)) selectedMenu = 5;
    if (ImGui::Button("Filtrar por Data", buttonSize)) selectedMenu = 10;
    if (ImGui::Button("Filtrar por Valor", buttonSize)) selectedMenu = 9;
    if (ImGui::Button("Total por Tipo", buttonSize)) selectedMenu = 11;
    if (ImGui::Button("Voltar", buttonSize)) selectedMenu = 0;

    ImGui::End();
}

void AppUI::renderBalanceScreen() {
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
    ImGui::Begin("Saldo Total", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Saldo atual calculado:");
    ImGui::Separator();
    ImGui::Text("R$ %.2f", currentBalance);
    ImGui::Separator();

    if (ImGui::Button("Voltar", buttonSize)) selectedMenu = 2;
    ImGui::End();
}

void AppUI::renderCategoryTotals() {
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
    ImGui::Begin("Totais por Categoria", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    std::map<std::string, double> totals;
    for (const auto& t : manager.getTransactions()) {
        double sign = t.getType() == "despesa" ? -1.0 : 1.0;
        totals[t.getCategory()] += t.getAmount() * sign;
    }

    for (const auto& [cat, total] : totals) {
        ImGui::Text("%s: R$ %.2f", cat.c_str(), total);
    }

    if (ImGui::Button("Voltar", buttonSize)) selectedMenu = 2;

    ImGui::End();
}

void AppUI::renderValueFilter() {
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
    ImGui::Begin("Filtrar por Valor", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    static char minStr[64] = "";
    static char maxStr[64] = "";
    static std::vector<Transaction> filtered;
    static bool showResults = false;

    ImGui::InputText("Valor mínimo", minStr, IM_ARRAYSIZE(minStr), ImGuiInputTextFlags_CharsDecimal);
    ImGui::InputText("Valor máximo", maxStr, IM_ARRAYSIZE(maxStr), ImGuiInputTextFlags_CharsDecimal);

    if (ImGui::Button("Filtrar", buttonSize) && strlen(minStr) > 0 && strlen(maxStr) > 0) {
        double minVal = std::stod(minStr);
        double maxVal = std::stod(maxStr);
        filtered.clear();
        for (const auto& t : manager.getTransactions()) {
            double val = t.getAmount();
            if (val >= minVal && val <= maxVal) {
                filtered.push_back(t);
            }
        }
        showResults = true;
    }

    if (showResults) {
        if (filtered.empty()) {
            ImGui::Text("Nenhuma transação encontrada.");
        } else {
            for (const auto& t : filtered) {
                ImGui::Separator();
                ImGui::Text("Valor: R$ %.2f", t.getAmount());
                ImGui::Text("Tipo: %s", t.getType().c_str());
                ImGui::Text("Categoria: %s", t.getCategory().c_str());
                ImGui::Text("Data: %s", t.getDate().c_str());
                ImGui::TextWrapped("Descrição: %s", t.getDescription().c_str());
            }
        }
    }

    if (ImGui::Button("Voltar", buttonSize)) {
        selectedMenu = 2;
        showResults = false;
    }

    ImGui::End();
}

void AppUI::renderDateFilter() {
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
    ImGui::Begin("Filtrar por Intervalo de Datas", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    static char startDate[64] = "";
    static char endDate[64] = "";
    static std::vector<Transaction> filtered;
    static bool showResults = false;

    ImGui::InputText("Data inicial (AAAA-MM-DD)", startDate, IM_ARRAYSIZE(startDate));
    ImGui::InputText("Data final (AAAA-MM-DD)", endDate, IM_ARRAYSIZE(endDate));

    if (ImGui::Button("Filtrar", buttonSize) && strlen(startDate) > 0 && strlen(endDate) > 0) {
        std::string start(startDate), end(endDate);
        filtered.clear();
        for (const auto& t : manager.getTransactions()) {
            const std::string& date = t.getDate();
            if (date >= start && date <= end) {
                filtered.push_back(t);
            }
        }
        showResults = true;
    }

    if (showResults) {
        if (filtered.empty()) {
            ImGui::Text("Nenhuma transação encontrada.");
        } else {
            for (const auto& t : filtered) {
                ImGui::Separator();
                ImGui::Text("Valor: R$ %.2f", t.getAmount());
                ImGui::Text("Tipo: %s", t.getType().c_str());
                ImGui::Text("Categoria: %s", t.getCategory().c_str());
                ImGui::Text("Data: %s", t.getDate().c_str());
                ImGui::TextWrapped("Descrição: %s", t.getDescription().c_str());
            }
        }
    }

    if (ImGui::Button("Voltar", buttonSize)) {
        selectedMenu = 2;
        showResults = false;
    }

    ImGui::End();
}

void AppUI::renderTypeTotals() {
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
    ImGui::Begin("Total por Tipo", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    static int tipoSelecionado = 0;
    ImGui::RadioButton("Despesas", &tipoSelecionado, 0); ImGui::SameLine();
    ImGui::RadioButton("Rendas", &tipoSelecionado, 1);

    std::string tipoStr = tipoSelecionado == 0 ? "despesa" : "renda";
    double total = analyzer.calculateTotalByType(manager.getTransactions(), tipoStr);
    ImGui::Text("Total de %s: R$ %.2f", tipoStr.c_str(), total);

    if (ImGui::Button("Voltar", buttonSize)) selectedMenu = 2;

    ImGui::End();
}

void AppUI::renderFileDeletion() {
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
    ImGui::Begin("Excluir Arquivo", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    auto arquivos = fileManager.listCSVFiles();
    static int selectedToDelete = -1;

    if (arquivos.empty()) {
        ImGui::Text("Nenhum arquivo .csv encontrado.");
    } else {
        for (int i = 0; i < (int)arquivos.size(); ++i) {
            if (ImGui::Selectable(arquivos[i].c_str(), selectedToDelete == i))
                selectedToDelete = i;
        }

        if (selectedToDelete != -1) {
            ImGui::Separator();
            ImGui::Text("Excluir '%s'?", arquivos[selectedToDelete].c_str());

            if (ImGui::Button("Confirmar Exclusão", buttonSize)) {
                if (fileManager.deleteFile(arquivos[selectedToDelete])) {
                    if (arquivos[selectedToDelete] == filename) {
                        filename.clear();
                        manager = TransactionManager();
                    }
                }
                selectedToDelete = -1;
            }
        }
    }

    if (ImGui::Button("Voltar", buttonSize)) selectedMenu = 0;

    ImGui::End();
}