#ifndef APPUI_HPP
#define APPUI_HPP

#include "transaction.hpp"
#include "transactionManager.hpp"
#include "budgetAnalyzer.hpp"
#include "fileManager.hpp"

// libs referentes a interface gráfica
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <Windows.h>
#include <Dwmapi.h>
#include <windowsx.h>
#include <map>
#include <cstring>

class AppUI {
private:
    GLFWwindow* window;
    TransactionManager manager;
    BudgetAnalyzer analyzer;
    FileManager fileManager;
    std::string filename;

    bool showFileDialog;
    int fileDialogOption;
    int selectedMenu;
    double currentBalance;

    char newFilename[256];
    char amount[64];
    char category[64];
    char date[64];
    char description[256];
    int transactionType;

    void renderFileDialog();
    void renderMainMenu();
    void renderTransactionMenu();
    void renderReportsMenu();
    void renderAddTransaction();
    void renderListTransactions();
    void renderEditTransaction();
    void renderRemoveTransaction();
    void renderBalanceScreen();
    void renderCategoryTotals();
    void renderValueFilter();
    void renderDateFilter();
    void renderTypeTotals();
    void renderFileDeletion();

public:
    AppUI(GLFWwindow* window);
    void render();

};

#endif