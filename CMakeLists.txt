cmake_minimum_required(VERSION 3.26)
project(trabalh_m1)

set(CMAKE_CXX_STANDARD 17)

# ── Baixa e compila o Raylib automaticamente ──────────────────────────
include(FetchContent)
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG        5.5
        GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(raylib)

# ── Executável ────────────────────────────────────────────────────────
add_executable(trabalh_m1
        src/main.cpp
        src/Sensores.cpp
        src/Cargas.cpp
        src/Naves.cpp
        src/Monitoramento.cpp
        src/Logs.cpp
        src/Hangar.cpp
        src/SetorSensores.cpp
        src/Background.cpp
        src/UI.cpp
        include/Background.h
        include/UI.h
        include/ModulosScreen.h
        include/ModulosOperacoes.h
)

target_include_directories(trabalh_m1 PRIVATE include)

# ── Linka o Raylib ────────────────────────────────────────────────────
target_link_libraries(trabalh_m1 PRIVATE raylib)

# ── Windows: evita abrir console junto com a janela ──────────────────
if(WIN32)
    target_link_options(trabalh_m1 PRIVATE -mwindows)
endif()