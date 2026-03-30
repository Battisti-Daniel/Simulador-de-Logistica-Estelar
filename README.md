# Simulador de Logistica Estelar

## Pre-requisitos

- CMake 3.26 ou superior
- Compilador com suporte a C++17
- Git instalado (usado pelo CMake para baixar o Raylib automaticamente)
- Internet na primeira compilacao (para baixar dependencias)

## Compilacao (metodo padrao)

Na raiz do projeto, rode:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

## Executar

### Linux e macOS

```bash
./build/trabalh_m1
```

### Windows (MinGW Makefiles, Ninja ou geradores de configuracao unica)

```powershell
.\build\trabalh_m1.exe
```

### Windows (Visual Studio - gerador multi-config)

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
.\build\Release\trabalh_m1.exe
```

## Exemplo rapido por plataforma

### Windows (MinGW)

```powershell
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
.\build\trabalh_m1.exe
```

### Linux

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./build/trabalh_m1
```

### macOS

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./build/trabalh_m1
```

## Limpar e recompilar do zero

Se der erro de cache antigo, apague a pasta de build e gere de novo:

```bash
cmake -E rm -rf build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

## Problemas comuns

- Erro de CMake muito antigo: atualize para 3.26+
- Erro de compilador nao encontrado: instale GCC/Clang/MSVC e confirme no PATH
- Erro ao baixar Raylib: verifique internet, proxy/firewall e tente novamente

## Observacoes

- O executavel gerado se chama `trabalh_m1`.
- No Windows, o projeto usa a opcao `-mwindows`, entao a aplicacao abre sem console adicional.