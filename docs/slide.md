---
author: Marcello Menezes e João Pedro
date: DD/MM/YYYY
---
### SmartBudget
_Controle suas finanças sem complicação._
---
# O que é o SmartBudget?

**SmartBudget** é uma aplicação desktop leve para monitoramento de despesas, desenvolvida em C++. O objetivo é fornecer uma ferramenta simples, eficiente e acessível para auxiliar no gerenciamento financeiro pessoal, com mínimo esforço do usuário.
---
## Funcionalidades Principais

- Adição, edição e remoção de transações (renda/despesa)
- Categorização das transações (ex: alimentação, aluguel, transporte)
- Exibição de saldo atual e transações recentes
- Salvamento e carregamento de dados via armazenamento local
---
## Tecnologias Utilizadas

- **Linguagem:** C++
- **Bibliotecas de Interface Gráfica (GUI):** [Dear ImGui](https://github.com/ocornut/imgui)
- **Armazenamento de Dados:**
  - JSON *(ou)*
  - CSV
- **Controle de Versão:** Git
---
## Classes Principais:
- **Transaction**
  - `amount`, `type`, `category`, `date`, `description`
- **TransactionManager**
  - CRUD das transações (criar, adicionar, remover, filtrar)
- **BudgetAnalyzer**
  - Cálculo do saldo, total por categoria, etc.
- **FileManager**
  - Leitura e gravação dos dados
---
# Pastas
```bash
tree ../ -d
````
---
### Equipe

- **Marcello Menezes** <meam<meam>>
- **João Pedro** <jpam<jpam>>
