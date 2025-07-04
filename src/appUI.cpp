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

void AppUI::renderFileDialog() {
    ImGui::OpenPopup("Seleção de Arquivo");
    if (ImGui::BeginPopupModal("Seleção de Arquivo", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Escolha uma opção:");

        if (ImGui::Button("Criar Novo Arquivo")) {
            fileDialogOption = 1;
            memset(newFilename, 0, sizeof(newFilename));
        }

        ImGui::SameLine();

        if (ImGui::Button("Carregar Arquivo Existente")) {
            fileDialogOption = 2;
        }

        if (fileDialogOption == 1) {
            ImGui::InputText("Nome do Arquivo", newFilename, IM_ARRAYSIZE(newFilename));
            if (ImGui::Button("Criar") && strlen(newFilename) > 0) {
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

        ImGui::EndPopup();
    }
}

void AppUI::renderMainMenu() {
    ImGui::Begin("Menu Principal", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    if (!filename.empty())
        ImGui::Text("Arquivo: %s", filename.c_str());

    if (ImGui::Button("Transações", ImVec2(200, 50))) selectedMenu = 1;
    if (ImGui::Button("Relatórios", ImVec2(200, 50))) selectedMenu = 2;
    if (ImGui::Button("Trocar Arquivo", ImVec2(200, 50))) showFileDialog = true;
    if (ImGui::Button("Excluir Arquivo", ImVec2(200, 50))) selectedMenu = 12;
    if (ImGui::Button("Sair", ImVec2(200, 50))) glfwSetWindowShouldClose(window, true);
    ImGui::End();
}

void AppUI::renderTransactionMenu() {
    ImGui::Begin("Transações", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    if (ImGui::Button("Adicionar Transação")) {
        selectedMenu = 3;
        memset(amount, 0, sizeof(amount));
        memset(category, 0, sizeof(category));
        memset(date, 0, sizeof(date));
        memset(description, 0, sizeof(description));
        transactionType = 0;
    }

    if (ImGui::Button("Listar Transações")) selectedMenu = 4;
    if (ImGui::Button("Editar Transação")) selectedMenu = 7;
    if (ImGui::Button("Remover Transação")) selectedMenu = 8;
    if (ImGui::Button("Voltar")) selectedMenu = 0;

    ImGui::End();
}

void AppUI::renderAddTransaction() {
    ImGui::Begin("Nova Transação", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::InputText("Valor (R$)", amount, IM_ARRAYSIZE(amount), ImGuiInputTextFlags_CharsDecimal);
    ImGui::RadioButton("Despesa", &transactionType, 0); ImGui::SameLine();
    ImGui::RadioButton("Renda", &transactionType, 1);
    ImGui::InputText("Categoria", category, IM_ARRAYSIZE(category));
    ImGui::InputText("Data (AAAA-MM-DD)", date, IM_ARRAYSIZE(date));
    ImGui::InputTextMultiline("Descrição", description, IM_ARRAYSIZE(description));

    if (ImGui::Button("Salvar") && strlen(amount) > 0) {
        Transaction nova(std::stod(amount),
                         transactionType == 0 ? "despesa" : "renda",
                         category, date, description);
        manager.addTransaction(nova);
        fileManager.saveToFile(manager.getTransactions(), filename);
        selectedMenu = 1;
    }

    ImGui::SameLine();
    if (ImGui::Button("Cancelar")) selectedMenu = 1;

    ImGui::End();
}

void AppUI::renderListTransactions() {
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

    if (ImGui::Button("Voltar")) selectedMenu = 1;
    ImGui::End();
}

void AppUI::renderEditTransaction() {
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

            if (ImGui::Button("Carregar dados")) {
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

            if (ImGui::Button("Salvar Alterações")) {
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

    if (ImGui::Button("Voltar")) selectedMenu = 1;

    ImGui::End();
}

void AppUI::renderRemoveTransaction() {
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

            if (ImGui::Button("Confirmar Remoção")) {
                manager.removeTransaction(indexToRemove);
                fileManager.saveToFile(manager.getTransactions(), filename);
                selectedMenu = 1;
            }
        }
    }

    if (ImGui::Button("Voltar")) selectedMenu = 1;

    ImGui::End();
}

void AppUI::renderReportsMenu() {
    ImGui::Begin("Relatórios", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    if (ImGui::Button("Saldo Total")) {
        currentBalance = analyzer.calculateBalance(manager.getTransactions());
        selectedMenu = 6;
    }

    if (ImGui::Button("Total por Categoria")) selectedMenu = 5;
    if (ImGui::Button("Filtrar por Data")) selectedMenu = 10;
    if (ImGui::Button("Filtrar por Valor")) selectedMenu = 9;
    if (ImGui::Button("Total por Tipo")) selectedMenu = 11;
    if (ImGui::Button("Voltar")) selectedMenu = 0;

    ImGui::End();
}

void AppUI::renderBalanceScreen() {
    ImGui::Begin("Saldo Total", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    ImGui::Text("Saldo atual calculado:");
    ImGui::Separator();
    ImGui::Text("R$ %.2f", currentBalance);
    ImGui::Separator();

    if (ImGui::Button("Voltar")) selectedMenu = 2;
    ImGui::End();
}

void AppUI::renderCategoryTotals() {
    ImGui::Begin("Totais por Categoria", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    std::map<std::string, double> totals;
    for (const auto& t : manager.getTransactions()) {
        double sign = t.getType() == "despesa" ? -1.0 : 1.0;
        totals[t.getCategory()] += t.getAmount() * sign;
    }

    for (const auto& [cat, total] : totals) {
        ImGui::Text("%s: R$ %.2f", cat.c_str(), total);
    }

    if (ImGui::Button("Voltar")) selectedMenu = 2;

    ImGui::End();
}

void AppUI::renderValueFilter() {
    ImGui::Begin("Filtrar por Valor", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    static char minStr[64] = "";
    static char maxStr[64] = "";

    ImGui::InputText("Valor mínimo", minStr, IM_ARRAYSIZE(minStr), ImGuiInputTextFlags_CharsDecimal);
    ImGui::InputText("Valor máximo", maxStr, IM_ARRAYSIZE(maxStr), ImGuiInputTextFlags_CharsDecimal);

    if (ImGui::Button("Filtrar") && strlen(minStr) > 0 && strlen(maxStr) > 0) {
        double minVal = std::stod(minStr);
        double maxVal = std::stod(maxStr);
        for (const auto& t : manager.getTransactions()) {
            double val = t.getAmount();
            if (val >= minVal && val <= maxVal) {
                ImGui::Separator();
                ImGui::Text("Valor: R$ %.2f", val);
                ImGui::Text("Tipo: %s", t.getType().c_str());
                ImGui::Text("Categoria: %s", t.getCategory().c_str());
                ImGui::Text("Data: %s", t.getDate().c_str());
                ImGui::TextWrapped("Descrição: %s", t.getDescription().c_str());
            }
        }
    }

    if (ImGui::Button("Voltar")) selectedMenu = 2;

    ImGui::End();
}

void AppUI::renderDateFilter() {
    ImGui::Begin("Filtrar por Data", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    static char data[64] = "";
    ImGui::InputText("Data (AAAA-MM-DD)", data, IM_ARRAYSIZE(data));

    if (ImGui::Button("Filtrar") && strlen(data) > 0) {
        for (const auto& t : manager.getTransactions()) {
            if (t.getDate() == data) {
                ImGui::Separator();
                ImGui::Text("Valor: R$ %.2f", t.getAmount());
                ImGui::Text("Tipo: %s", t.getType().c_str());
                ImGui::Text("Categoria: %s", t.getCategory().c_str());
                ImGui::Text("Data: %s", t.getDate().c_str());
                ImGui::TextWrapped("Descrição: %s", t.getDescription().c_str());
            }
        }
    }

    if (ImGui::Button("Voltar")) selectedMenu = 2;

    ImGui::End();
}

void AppUI::renderTypeTotals() {
    ImGui::Begin("Total por Tipo", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    static int tipoSelecionado = 0;
    ImGui::RadioButton("Despesas", &tipoSelecionado, 0); ImGui::SameLine();
    ImGui::RadioButton("Rendas", &tipoSelecionado, 1);

    std::string tipoStr = tipoSelecionado == 0 ? "despesa" : "renda";
    double total = analyzer.calculateTotalByType(manager.getTransactions(), tipoStr);
    ImGui::Text("Total de %s: R$ %.2f", tipoStr.c_str(), total);

    if (ImGui::Button("Voltar")) selectedMenu = 2;

    ImGui::End();
}

void AppUI::renderFileDeletion() {
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

            if (ImGui::Button("Confirmar Exclusão")) {
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

    if (ImGui::Button("Voltar")) selectedMenu = 0;

    ImGui::End();
}