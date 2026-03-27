#include "Background.h"
#include "../include/raylib.h"
#include "../include/Sensores.h"
#include "../include/Cargas.h"
#include "../include/Naves.h"
#include "../include/Monitoramento.h"
#include "../include/Logs.h"
#include "../include/Hangar.h"

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);

    InitWindow(1400, 900, "ESTACAO ESTELAR - Sol Girando");
    MaximizeWindow();
    SetTargetFPS(120);

    while (!WindowShouldClose()) {

        BeginDrawing();

        Background();
        Panel();

        DrawText(TextFormat("FPS: %d", GetFPS()), 20, 20, 24, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}