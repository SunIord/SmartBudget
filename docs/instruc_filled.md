# Instruções de como compilar o SmartBudget

## Linux

### Requisitos

O projeto depende de:

- CMake
- GLFW (via glfw-x11)
- OpenGL (via mesa)
- GLEW
- X11

### Instalação dos pacotes necessários no EndeavourOS ou qualquer Arch-based:

```bash
sudo pacman -Syu
sudo pacman -S cmake glfw-x11 glew libx11 base-devel
```

### Compilação

Dentro da raiz do projeto, execute os comandos abaixo:

```bash
mkdir -p build
cd build
cmake ..
make
```

### Execução

Após a compilação, o executável estará disponível em `build/SmartBudget`:

```bash
./SmartBudget
```

---

## Windows

### Requisitos

- VS Code (Visual Studio Code)
- MSYS2 com pacotes MinGW
- CMake
- GLFW
- GLEW

### Instalação do ambiente (via MSYS2)

1. Baixe e instale o MSYS2: https://www.msys2.org

2. Abra o terminal `MSYS2 MinGW 64-bit` e execute:

```bash
pacman -Syu
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-glfw mingw-w64-x86_64-glew
```

### Compilação no Windows

1. No terminal `MSYS2 MinGW 64-bit`, navegue até a raiz do projeto:

```bash
cd /c/Users/seu_usuario/Documents/SmartBudget
```

2. Compile usando o CMake:

```bash
cmake -B build -G "MinGW Makefiles"
cmake --build build
```

### Execução

O executável estará disponível em `build/SmartBudget.exe`. Você pode executá-lo diretamente pelo Explorer ou pelo terminal:

```bash
./build/SmartBudget.exe
```

---

### Observações

- O projeto utiliza `CMakeLists.txt` configurado para ser multiplataforma.
- A pasta `.vscode/` contém configurações recomendadas para quem utiliza o Visual Studio Code.
- O código fonte principal está localizado em `src/`.
- As dependências de interface (Dear ImGui) estão na pasta `lib/imgui/`.
