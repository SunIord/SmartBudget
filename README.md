# SmartBudget

**SmartBudget** é uma aplicação desktop leve para monitoramento de despesas, desenvolvida em C++. O objetivo é fornecer uma ferramenta simples, eficiente e acessível para auxiliar no gerenciamento financeiro pessoal, com mínimo esforço do usuário.

## Funcionalidades Principais

- Adição, edição e remoção de transações (renda/despesa)
- Categorização das transações (ex: alimentação, aluguel, transporte)
- Exibição de saldo atual e transações recentes
- Salvamento e carregamento de dados via armazenamento local

## Tecnologias Utilizadas

- **Linguagem:** C++
- **Bibliotecas de Interface Gráfica (GUI):** [Dear ImGui](https://github.com/ocornut/imgui)
- **Armazenamento de Dados:**
  - JSON *(ou)*
  - CSV
- **Controle de Versão:** Git

## Compilação e Execução

### Linux (Arch, EndeavourOS, Manjaro, etc.)

**Requisitos:**

```bash
sudo pacman -Syu
sudo pacman -S cmake glfw-x11 glew libx11 base-devel
```

**Compilação:**

```bash
mkdir -p build
cd build
cmake ..
make
```

**Execução:**

```bash
./SmartBudget
```

---

### Windows (VS Code + MSYS2)

**Pré-requisitos:**

1. Baixe e instale o MSYS2: https://www.msys2.org
2. Abra o terminal `MSYS2 MinGW 64-bit`
3. Execute:

```bash
pacman -Syu
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-glfw mingw-w64-x86_64-glew
```

**Compilação:**

No terminal `MSYS2 MinGW 64-bit`, navegue até a raiz do projeto e execute:

```bash
cmake -B build -G "MinGW Makefiles"
cmake --build build
```

**Execução:**

```bash
./build/SmartBudget.exe
```

**Observação:** Use sempre o terminal `MSYS2 MinGW 64-bit` para compilar e rodar o programa. Não use o MSYS shell nem o PowerShell padrão.

---

## Arquitetura Orientada a Objetos

### Classes Principais:

- **Transaction**
  - `amount`, `type` (renda/despesa), `category`, `date`, `description`
- **TransactionManager**
  - CRUD das transações (criar, adicionar, remover, filtrar)
- **BudgetAnalyzer**
  - Cálculo do saldo, total por categoria, etc.
- **FileManager**
  - Leitura e gravação dos dados
- **AppUI**
  - Interface com o usuário e manipulação de inputs

## Metas Estendidas (Stretch Goals)

- Gráficos para análise de gastos
- Relatórios mensais exportáveis para CSV
- Tema escuro
- Suporte a múltiplas moedas

## Linha do Tempo

| Sprint | Período         | Entregas                                                            |
|--------|------------------|---------------------------------------------------------------------|
| 1      | 02/06 - 06/06     | Setup do projeto, escolha da biblioteca GUI, classes base          |
| 2      | 09/06 - 13/06     | Interface para adicionar, listar e visualizar transações           |
| 3      | 16/06 - 20/06     | Salvamento/carregamento de dados, cálculo de saldo e categorias    |
| 4      | 23/06 - 27/06     | Polimento, testes e melhorias visuais                              |
| 5      | 30/06 - 01/07     | Finalização da documentação                                        |

## Equipe

- **Marcello Menezes** <meam<meam>>
- **João Pedro** <jpam<jpam>>

---

_SmartBudget: Controle suas finanças sem complicação._
