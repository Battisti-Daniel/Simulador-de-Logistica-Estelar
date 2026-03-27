#include <raylib.h>
#include <cmath>

int opcaoSelecionada = 0;
float tempoAnimacao = 0.0f;

void Background() {
    int w = GetScreenWidth();
    int h = GetScreenHeight();

    for(int y = 0; y < h; y++) {
        float progress = (float)y / h;

        int r1 = 30, g1 = 100, b1 = 200;
        int r2 = 255, g2 = 140, b2 = 30;

        int r = (int)(r1 + (r2 - r1) * progress);
        int g = (int)(g1 + (g2 - g1) * progress);
        int b = (int)(b1 + (b2 - b1) * progress);

        DrawLine(0, y, w, y, {(unsigned char)r, (unsigned char)g, (unsigned char)b, 255});
    }
}

void Panel() {
    int w = GetScreenWidth();
    int h = GetScreenHeight();

    int painelLargura = 1200;
    int painelAltura = 800;

    int x = (w - painelLargura) / 2;
    int y = (h - painelAltura) / 2;

    tempoAnimacao += GetFrameTime();
    if(tempoAnimacao > 6.28f) tempoAnimacao -= 6.28f; // Resetar após 2π

    DrawRectangleRounded({(float)x, (float)y, (float)painelLargura, (float)painelAltura}, 0.1f, 20, {20, 30, 60, 200});
    DrawRectangleRoundedLines({(float)x, (float)y, (float)painelLargura, (float)painelAltura}, 0.1f, 20, {0, 200, 255, 255});

    const char* titulo = "ESTACAO ESTELAR";
    int titleWidth = MeasureText(titulo, 50);
    DrawText(titulo, x + (painelLargura - titleWidth) / 2, y + 30, 50, {0, 200, 255, 255});

    const char* subtitulo = "Sistema de Gerenciamento Logístico";
    int subtitleWidth = MeasureText(subtitulo, 24);
    DrawText(subtitulo, x + (painelLargura - subtitleWidth) / 2, y + 90, 24, {150, 180, 255, 200});

    DrawLine(x + 50, y + 140, x + painelLargura - 50, y + 140, {0, 200, 255, 150});

    const char* bemvindo = "Bem-vindo ao Sistema de Monitoramento";
    int welcomeWidth = MeasureText(bemvindo, 24);
    DrawText(bemvindo, x + (painelLargura - welcomeWidth) / 2, y + 200, 24, {200, 220, 255, 255});

    int blocoLargura = 250;
    int blocoAltura = 120;
    int espacoEntreOpcoes = 100;

    int blocoEntrarX = x + (painelLargura / 2 - blocoLargura - espacoEntreOpcoes / 2);
    int blocoSairX = x + (painelLargura / 2 + espacoEntreOpcoes / 2);
    int blocoY = y + 380;

    Color corEntrar = (opcaoSelecionada == 0) ? Color{0, 255, 100, 255} : Color{100, 150, 200, 200};
    Color corBordaEntrar = (opcaoSelecionada == 0) ? Color{0, 255, 100, 255} : Color{100, 150, 200, 255};

    DrawRectangleRounded({(float)blocoEntrarX, (float)blocoY, (float)blocoLargura, (float)blocoAltura}, 0.08f, 15, corEntrar);
    DrawRectangleRoundedLines({(float)blocoEntrarX, (float)blocoY, (float)blocoLargura, (float)blocoAltura}, 0.08f, 15, corBordaEntrar);

    const char* textEntrar = "ENTRAR";
    int widthEntrar = MeasureText(textEntrar, 32);
    DrawText(textEntrar, blocoEntrarX + (blocoLargura - widthEntrar) / 2, blocoY + 20, 32, {20, 30, 60, 255});

    Color corSair = (opcaoSelecionada == 1) ? Color{255, 100, 0, 255} : Color{100, 150, 200, 200};
    Color corBordaSair = (opcaoSelecionada == 1) ? Color{255, 100, 0, 255} : Color{100, 150, 200, 255};

    DrawRectangleRounded({(float)blocoSairX, (float)blocoY, (float)blocoLargura, (float)blocoAltura}, 0.08f, 15, corSair);
    DrawRectangleRoundedLines({(float)blocoSairX, (float)blocoY, (float)blocoLargura, (float)blocoAltura}, 0.08f, 15, corBordaSair);

    const char* textSair = "SAIR";
    int widthSair = MeasureText(textSair, 32);
    DrawText(textSair, blocoSairX + (blocoLargura - widthSair) / 2, blocoY + 20, 32, {20, 30, 60, 255});

    Color corSetaAtiva = (opcaoSelecionada == 0) ? Color{0, 255, 100, 255} : Color{255, 100, 0, 255};

    if(opcaoSelecionada == 0) {
        int setaXEntrar = blocoEntrarX - 50;
        int setaYEntrar = blocoY + blocoAltura / 2;

        float offset = sin(tempoAnimacao * 3.0f) * 15.0f;
        setaXEntrar += (int)offset;

        DrawTriangle(
            {(float)(setaXEntrar - 20), (float)(setaYEntrar - 20)},
            {(float)(setaXEntrar - 20), (float)(setaYEntrar + 20)},
            {(float)setaXEntrar, (float)setaYEntrar},
            corSetaAtiva
        );
    } else {
        int setaXSair = blocoSairX - 50;
        int setaYSair = blocoY + blocoAltura / 2;

        float offset = sin(tempoAnimacao * 3.0f) * 15.0f;
        setaXSair += (int)offset;

        DrawTriangle(
            {(float)(setaXSair - 20), (float)(setaYSair - 20)},
            {(float)(setaXSair - 20), (float)(setaYSair + 20)},
            {(float)setaXSair, (float)setaYSair},
            corSetaAtiva
        );
    }

    const char* instrucoes1 = "Use ESQUERDA/DIREITA para selecionar";
    int w1 = MeasureText(instrucoes1, 18);
    DrawText(instrucoes1, x + (painelLargura - w1) / 2, y + 580, 18, {150, 180, 255, 200});

    const char* instrucoes2 = "ENTER para confirmar  |  ESC para fechar";
    int w2 = MeasureText(instrucoes2, 18);
    DrawText(instrucoes2, x + (painelLargura - w2) / 2, y + 620, 18, {150, 180, 255, 200});

    if(IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
        opcaoSelecionada = 0;
    }
    if(IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
        opcaoSelecionada = 1;
    }

    if(IsKeyPressed(KEY_ENTER)) {
        if(opcaoSelecionada == 0) {
            DrawText("Entrando no modulo...", x + 50, y + 720, 20, {0, 255, 100, 255});
        } else {
            CloseWindow();
        }
    }

    if(IsKeyPressed(KEY_ESCAPE)) {
        CloseWindow();
    }
}

