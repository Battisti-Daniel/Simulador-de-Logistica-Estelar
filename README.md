# Simulador de Logistica Estelar

Projeto em C++17 com Raylib.
O Raylib e baixado automaticamente pelo CMake na primeira compilacao (FetchContent).

## Requisitos

- CMake 3.26 ou superior
- Git no PATH
- Compilador C/C++ instalado
- Internet na primeira compilacao

## Build recomendado (CMake Presets)

Este repositorio inclui `CMakePresets.json` com presets por plataforma.
Use os comandos abaixo na raiz do projeto.

### Windows (Visual Studio 2022, recomendado)

Requisitos:
- Visual Studio 2022 com workload Desktop development with C++

Comandos:

```powershell
cmake --preset windows-msvc-release
cmake --build --preset windows-msvc-release
.\build\windows-msvc-release\Release\trabalh_m1.exe
```

### Windows (MinGW/MSYS2)

Requisitos:
- `gcc`, `g++` e `mingw32-make` no PATH

Comandos:

```powershell
cmake --preset windows-mingw-release
cmake --build --preset windows-mingw-release
.\build\windows-mingw-release\trabalh_m1.exe
```

### Linux

Comandos:

```bash
cmake --preset linux-release
cmake --build --preset linux-release
./build/linux-release/trabalh_m1
```

### macOS

Comandos:

```bash
cmake --preset macos-release
cmake --build --preset macos-release
./build/macos-release/trabalh_m1
```

## Solucao para o erro "nmake -? no such file or directory"

Se aparecer esse erro, o CMake tentou usar o gerador NMake sem o nmake instalado.

Use um preset explicito (acima) ou rode com gerador explicito.
Exemplo MinGW:

```powershell
cmake -S . -B build/manual-mingw -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build/manual-mingw
```

## Limpar cache e recompilar do zero

Se voce trocou de gerador (por exemplo, NMake -> MinGW/Visual Studio), limpe a pasta de build:

```powershell
cmake -E rm -rf build
```

Depois configure novamente com um dos presets.

## Observacoes

- Nome do executavel: `trabalh_m1`
- No Windows, o projeto abre como app grafica sem console extra