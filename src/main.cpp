#include "Background.h"
#include "../include/raylib.h"
#include "../include/UI.h"

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);

    InitWindow(1400, 900, "ESTACAO ESTELAR - Sistema de Gerenciamento Logístico");
    MaximizeWindow();
    SetTargetFPS(120);

    // Desabilitar qualquer tecla de saída - apenas fechar pelo botão X
    SetExitKey(KEY_NULL);

    UIManager *ui = createUIManager();

    while (!WindowShouldClose()) {
        updateUI(ui);

        BeginDrawing();
        ClearBackground({10, 10, 30, 255});

        renderUI(ui);

        DrawText(TextFormat("FPS: %d", GetFPS()), 20, 20, 20, {100, 200, 255, 200});

        EndDrawing();
    }

    destroyUIManager(ui);
    CloseWindow();
    return 0;
}