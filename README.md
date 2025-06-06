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

- **Marcello Menezes** <<meam>>
- **João Pedro** <<jpam>>

---

> _SmartBudget: Controle suas finanças sem complicação._
