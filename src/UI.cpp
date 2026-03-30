#include "../include/UI.h"
#include "../include/Background.h"
#include <iostream>
#include <cstring>
#include <cmath>

// Cores do sistema
const Color PRIMARY_COLOR = {0, 200, 255, 255};
const Color SECONDARY_COLOR = {100, 150, 200, 200};
const Color SUCCESS_COLOR = {0, 255, 100, 255};
const Color ERROR_COLOR = {255, 100, 100, 255};
const Color WARNING_COLOR = {255, 200, 50, 255};
const Color BG_COLOR = {20, 30, 60, 200};

UIManager* createUIManager() {
    UIManager *ui = new UIManager();

    ui->currentState = STATE_MENU;
    ui->previousState = STATE_MENU;
    ui->selectedMenu = 0;
    ui->statusDisplayTime = 0.0f;
    ui->isInputActive = false;
    ui->inputLength = 0;
    memset(ui->inputBuffer, 0, sizeof(ui->inputBuffer));

    // Inicializar TADs
    ui->sensors = createSensors();
    ui->loads = createLoads();
    ui->ships = createShips();
    ui->monitoring = createMonitoring();
    ui->logs = createLog();
    ui->hangar = createHangar();

    return ui;
}

void destroyUIManager(UIManager *ui) {
    if (ui) {
        delete ui->sensors;
        delete ui->loads;
        delete ui->ships;
        delete ui->monitoring;
        delete ui->logs;
        delete ui->hangar;
        delete ui;
    }
}

void addStatusMessage(UIManager *ui, const std::string &text, float duration, Color color) {
    // Limpa fila anterior para mostrar nova mensagem imediatamente
    while (!ui->statusQueue.empty()) {
        ui->statusQueue.pop();
    }
    ui->statusDisplayTime = 0.0f;

    StatusMessage msg;
    msg.text = text;
    msg.duration = duration;
    msg.color = color;
    ui->statusQueue.push(msg);
}

void renderStatusBar(UIManager *ui) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Desenhar barra de status fixa na base
    DrawRectangle(0, screenHeight - 60, screenWidth, 60, {20, 30, 60, 200});
    DrawLine(0, screenHeight - 60, screenWidth, screenHeight - 60, PRIMARY_COLOR);

    // Exibir mensagem de status se houver
    if (!ui->statusQueue.empty()) {
        StatusMessage &msg = ui->statusQueue.front();

        std::string displayText = "► " + msg.text;
        DrawText(displayText.c_str(), 20, screenHeight - 45, 20, msg.color);

        ui->statusDisplayTime += GetFrameTime();
        if (ui->statusDisplayTime >= msg.duration) {
            ui->statusQueue.pop();
            ui->statusDisplayTime = 0.0f;
        }
    } else {
        DrawText("Pressione [ESC] para voltar ao menu", 20, screenHeight - 45, 16, {150, 150, 150, 200});
    }
}

void renderInputBox(UIManager *ui, const std::string &prompt) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    int boxWidth = 400;
    int boxHeight = 150;
    int boxX = (screenWidth - boxWidth) / 2;
    int boxY = (screenHeight - boxHeight) / 2 + 100;

    DrawRectangleRounded({(float)boxX, (float)boxY, (float)boxWidth, (float)boxHeight}, 0.1f, 15, BG_COLOR);
    DrawRectangleRoundedLines({(float)boxX, (float)boxY, (float)boxWidth, (float)boxHeight}, 0.1f, 15, PRIMARY_COLOR);

    DrawText(prompt.c_str(), boxX + 20, boxY + 15, 18, PRIMARY_COLOR);
    DrawText("Input: ", boxX + 20, boxY + 50, 16, {200, 200, 200, 255});

    // Desenhar o input buffer
    DrawRectangle(boxX + 90, boxY + 45, 280, 30, {40, 50, 90, 200});
    DrawRectangleLines(boxX + 90, boxY + 45, 280, 30, PRIMARY_COLOR);
    DrawText(ui->inputBuffer, boxX + 100, boxY + 52, 16, WHITE);

    // Desenhar cursor
    float cursorX = MeasureText(ui->inputBuffer, 16) + boxX + 100;
    DrawLine((int)cursorX, boxY + 50, (int)cursorX, boxY + 75, PRIMARY_COLOR);

    DrawText("[ENTER] Confirmar | [ESC] Cancelar", boxX + 20, boxY + 110, 12, {150, 150, 150, 200});
}

void handleInput(UIManager *ui) {
    int key = GetCharPressed();

    while (key > 0) {
        if ((key >= 32) && (key <= 125) && (ui->inputLength < 49)) {
            ui->inputBuffer[ui->inputLength] = (char)key;
            ui->inputLength += 1;
            ui->inputBuffer[ui->inputLength] = '\0';
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && ui->inputLength > 0) {
        ui->inputLength -= 1;
        ui->inputBuffer[ui->inputLength] = '\0';
    }
}

// Flag para ignorar primeira tecla após ativar input
bool skipNextInput = false;

void drawBox(float x, float y, float width, float height, const std::string &title, Color color, bool selected) {
    Color borderColor = selected ? Color{0, 255, 100, 255} : color;
    DrawRectangleRounded({x, y, width, height}, 0.08f, 15, BG_COLOR);
    DrawRectangleRoundedLines({x, y, width, height}, 0.08f, 15, borderColor);

    int titleWidth = MeasureText(title.c_str(), 18);
    DrawText(title.c_str(), (int)(x + (width - titleWidth) / 2), (int)(y + 10), 18, color);
}

void drawCircle(float x, float y, float radius, const std::string &title, Color color, bool selected) {
    DrawCircleLines((int)x, (int)y, (int)radius, selected ? Color{0, 255, 100, 255} : color);

    int titleWidth = MeasureText(title.c_str(), 16);
    DrawText(title.c_str(), (int)(x - titleWidth / 2), (int)(y + radius + 10), 16, color);
}

void drawStack(float x, float y, float width, float height, const std::string &title, int itemCount, Color color) {
    float blockHeight = 30;
    DrawRectangleRounded({x, y, width, height}, 0.08f, 15, BG_COLOR);
    DrawRectangleRoundedLines({x, y, width, height}, 0.08f, 15, color);

    int titleWidth = MeasureText(title.c_str(), 18);
    DrawText(title.c_str(), (int)(x + (width - titleWidth) / 2), (int)(y + 10), 18, color);

    for (int i = 0; i < itemCount && i < 5; i++) {
        float blockY = y + 50 + (i * blockHeight);
        DrawRectangleRounded({x + 10, blockY, width - 20, blockHeight - 5}, 0.05f, 10, color);
        DrawRectangleRoundedLines({x + 10, blockY, width - 20, blockHeight - 5}, 0.05f, 10, PRIMARY_COLOR);
    }
}

// ========== MENU PRINCIPAL ==========

void renderMainMenu(UIManager *ui) {
    int w = GetScreenWidth();
    int h = GetScreenHeight();

    Background();

    int panelWidth = 1000;
    int panelHeight = 700;
    int x = (w - panelWidth) / 2;
    int y = (h - panelHeight) / 2;

    DrawRectangleRounded({(float)x, (float)y, (float)panelWidth, (float)panelHeight}, 0.1f, 20, BG_COLOR);
    DrawRectangleRoundedLines({(float)x, (float)y, (float)panelWidth, (float)panelHeight}, 0.1f, 20, PRIMARY_COLOR);

    const char* titulo = "ESTACAO ESTELAR";
    int titleWidth = MeasureText(titulo, 50);
    DrawText(titulo, x + (panelWidth - titleWidth) / 2, y + 30, 50, PRIMARY_COLOR);

    const char* subtitulo = "Sistema de Gerenciamento Logístico";
    int subtitleWidth = MeasureText(subtitulo, 24);
    DrawText(subtitulo, x + (panelWidth - subtitleWidth) / 2, y + 90, 24, {150, 180, 255, 200});

    DrawLine(x + 50, y + 140, x + panelWidth - 50, y + 140, PRIMARY_COLOR);

    // Opções do menu
    const char* opcoes[] = {
        "1 - SENSORES",
        "2 - CARGAS",
        "3 - NAVES",
        "4 - MONITORAMENTO",
        "5 - LOGS",
        "6 - HANGAR"
    };

    int optionStartY = y + 200;
    for (int i = 0; i < 6; i++) {
        Color optionColor;
        if (ui->selectedMenu == i) {
            optionColor = SUCCESS_COLOR;
        } else {
            optionColor = {150, 180, 255, 200};
        }
        DrawText(opcoes[i], x + 100, optionStartY + (i * 70), 24, optionColor);
    }

    DrawLine(x + 50, y + panelHeight - 100, x + panelWidth - 50, y + panelHeight - 100, PRIMARY_COLOR);

    const char* info = "Use SETAS para navegar | ENTER para selecionar | Clique em X para fechar";
    int infoWidth = MeasureText(info, 16);
    DrawText(info, x + (panelWidth - infoWidth) / 2, y + panelHeight - 70, 16, {150, 150, 150, 200});
}

void updateMainMenu(UIManager *ui) {
    if (IsKeyPressed(KEY_UP)) {
        ui->selectedMenu = (ui->selectedMenu - 1 + 6) % 6;
    }
    if (IsKeyPressed(KEY_DOWN)) {
        ui->selectedMenu = (ui->selectedMenu + 1) % 6;
    }
    if (IsKeyPressed(KEY_ENTER)) {
        switch (ui->selectedMenu) {
            case 0: ui->currentState = STATE_SENSORES; break;
            case 1: ui->currentState = STATE_CARGAS; break;
            case 2: ui->currentState = STATE_NAVES; break;
            case 3: ui->currentState = STATE_MONITORAMENTO; break;
            case 4: ui->currentState = STATE_LOGS; break;
            case 5: ui->currentState = STATE_HANGAR; break;
        }
    }
}

// ========== MÓDULO SENSORES ==========

void renderSensoresModule(UIManager *ui) {
    Background();

    DrawText("SETOR DE SENSORES (Lista Estática)", 50, 50, 32, PRIMARY_COLOR);

    char buffer[256];
    sprintf(buffer, "MAX: 5 sensores | Atual: %d", ui->sensors->counter);
    DrawText(buffer, 50, 100, 18, {200, 200, 200, 255});

    // Desenhar lista de sensores
    int listX = 50;
    int listY = 150;
    DrawRectangle(listX, listY, 350, 300, {30, 40, 80, 150});
    DrawRectangleLines(listX, listY, 350, 300, PRIMARY_COLOR);

    DrawText("Lista:", listX + 10, listY + 10, 16, PRIMARY_COLOR);

    for (int i = 0; i < ui->sensors->counter; i++) {
        char numStr[16];
        sprintf(numStr, "%d", i + 1);
        DrawText(numStr, listX + 20, listY + 50 + (i * 50), 14, {150, 150, 150, 255});
        DrawText(ui->sensors->sensor[i].c_str(), listX + 60, listY + 50 + (i * 50), 14, SUCCESS_COLOR);
    }

    // Visualização de caixas representando sensores (Array Estático)
    int vizX = 420;
    int vizY = 150;
    DrawText("Visualizacao:", vizX, vizY - 30, 16, PRIMARY_COLOR);

    for (int i = 0; i < 5; i++) {
        int boxX = vizX + (i * 110);
        int boxY = vizY;
        Color boxColor;
        if (i < ui->sensors->counter) {
            boxColor = {100, 200, 100, 200};
        } else {
            boxColor = {100, 100, 100, 100};
        }

        DrawRectangleRounded({(float)boxX, (float)boxY, 100, 80}, 0.05f, 10, boxColor);
        DrawRectangleRoundedLines({(float)boxX, (float)boxY, 100, 80}, 0.05f, 10, PRIMARY_COLOR);

        if (i < ui->sensors->counter) {
            int textWidth = MeasureText(ui->sensors->sensor[i].c_str(), 12);
            int maxWidth = 90;
            if (textWidth > maxWidth) {
                char truncated[20];
                strncpy(truncated, ui->sensors->sensor[i].c_str(), 10);
                truncated[10] = '\0';
                strcat(truncated, "..");
                DrawText(truncated, boxX + 5, boxY + 25, 12, WHITE);
            } else {
                DrawText(ui->sensors->sensor[i].c_str(), boxX + (100 - textWidth) / 2, boxY + 25, 12, WHITE);
            }
        }

        char indexStr[8];
        sprintf(indexStr, "[%d]", i);
        int idxWidth = MeasureText(indexStr, 10);
        DrawText(indexStr, boxX + (100 - idxWidth) / 2, boxY + 55, 10, {200, 200, 200, 255});
    }

    // Instruções
    DrawText("OPERACOES:", 50, 480, 18, PRIMARY_COLOR);
    DrawText("[I] Inserir | [R] Remover Nome | [D] Remover Index | [ESC] Menu", 50, 520, 14, {200, 200, 200, 255});

    renderStatusBar(ui);
}

void updateSensoresModule(UIManager *ui) {
    static int removeMode = 0; // 0: none, 1: insert, 2: remove by name, 3: remove by index

    if (IsKeyPressed(KEY_I) && removeMode == 0) {
        ui->isInputActive = true;
        removeMode = 1;
        ui->inputLength = 0;
        memset(ui->inputBuffer, 0, sizeof(ui->inputBuffer));
        skipNextInput = true;
    }

    if (IsKeyPressed(KEY_R) && removeMode == 0) {
        ui->isInputActive = true;
        removeMode = 2;
        ui->inputLength = 0;
        memset(ui->inputBuffer, 0, sizeof(ui->inputBuffer));
        skipNextInput = true;
    }

    if (IsKeyPressed(KEY_D) && removeMode == 0) {
        ui->isInputActive = true;
        removeMode = 3;
        ui->inputLength = 0;
        memset(ui->inputBuffer, 0, sizeof(ui->inputBuffer));
        skipNextInput = true;
    }

    if (ui->isInputActive) {
        if (!skipNextInput) {
            handleInput(ui);
        } else {
            skipNextInput = false;
        }

        if (IsKeyPressed(KEY_ENTER)) {
            if (ui->inputLength > 0) {
                if (removeMode == 1) {
                    // Inserir sensor
                    if (insertSensor(ui->sensors, std::string(ui->inputBuffer))) {
                        char msg[256];
                        sprintf(msg, "Sensor '%s' inserido!", ui->inputBuffer);
                        addStatusMessage(ui, msg, 1.5f, SUCCESS_COLOR);
                    } else {
                        addStatusMessage(ui, "Erro: Lista cheia (MAX: 5)!", 1.5f, ERROR_COLOR);
                    }
                } else if (removeMode == 2) {
                    // Remover por nome
                    std::string result = removeSensor(ui->sensors, std::string(ui->inputBuffer));
                    if (result.length() > 0) {
                        char msg[256];
                        sprintf(msg, "Sensor '%s' removido!", result.c_str());
                        addStatusMessage(ui, msg, 1.5f, SUCCESS_COLOR);
                    } else {
                        addStatusMessage(ui, "Sensor não encontrado!", 1.5f, ERROR_COLOR);
                    }
                } else if (removeMode == 3) {
                    // Remover por índice
                    int index = atoi(ui->inputBuffer);
                    std::string result = removeSensor(ui->sensors, index);
                    if (result.length() > 0) {
                        char msg[256];
                        sprintf(msg, "Sensor '%s' removido!", result.c_str());
                        addStatusMessage(ui, msg, 1.5f, SUCCESS_COLOR);
                    } else {
                        addStatusMessage(ui, "Índice inválido!", 1.5f, ERROR_COLOR);
                    }
                }
            }
            ui->isInputActive = false;
            removeMode = 0;
            ui->inputLength = 0;
            memset(ui->inputBuffer, 0, sizeof(ui->inputBuffer));
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            ui->isInputActive = false;
            removeMode = 0;
            ui->inputLength = 0;
            memset(ui->inputBuffer, 0, sizeof(ui->inputBuffer));
        }
    }
}

// ========== MÓDULO CARGAS ==========

void renderCargasModule(UIManager *ui) {
    Background();

    char buffer[256];
    sprintf(buffer, "Total de Cargas: %d", ui->loads->counter);
    DrawText("INVENTARIO DE CARGAS (Lista Simplesmente Encadeada)", 50, 50, 32, PRIMARY_COLOR);
    DrawText(buffer, 50, 100, 18, {200, 200, 200, 255});

    // Desenhar lista de cargas
    int listX = 50;
    int listY = 150;
    DrawRectangle(listX, listY, 350, 300, {30, 40, 80, 150});
    DrawRectangleLines(listX, listY, 350, 300, PRIMARY_COLOR);

    DrawText("Lista:", listX + 10, listY + 10, 16, PRIMARY_COLOR);

    Node *current = ui->loads->head;
    int index = 0;
    while (current != NULL && index < 6) {
        char numStr[16];
        sprintf(numStr, "%d", index + 1);
        DrawText(numStr, listX + 20, listY + 50 + (index * 40), 14, {150, 150, 150, 255});
        DrawText(current->name, listX + 60, listY + 50 + (index * 40), 14, SUCCESS_COLOR);
        current = current->next;
        index++;
    }

    // Visualização de lista encadeada com setas - com quebra de linha
    int vizX = 420;
    int vizY = 180;
    DrawText("Visualizacao:", vizX, vizY - 40, 16, PRIMARY_COLOR);

    current = ui->loads->head;
    index = 0;
    int posX = vizX;
    int posY = vizY;
    int itemsPerRow = 7;

    while (current != NULL && index < 21) {
        // Desenhar caixa do nó
        DrawRectangleRounded({(float)posX, (float)posY, 70, 50}, 0.05f, 8, {100, 150, 200, 200});
        DrawRectangleRoundedLines({(float)posX, (float)posY, 70, 50}, 0.05f, 8, PRIMARY_COLOR);

        // Texto do nó
        int textWidth = MeasureText(current->name, 9);
        if (textWidth > 60) {
            char truncated[12];
            strncpy(truncated, current->name, 7);
            truncated[7] = '\0';
            strcat(truncated, ".");
            DrawText(truncated, posX + 5, posY + 15, 9, WHITE);
        } else {
            DrawText(current->name, posX + (70 - textWidth) / 2, posY + 15, 9, WHITE);
        }

        // Número da posição
        char posStr[4];
        sprintf(posStr, "#%d", index + 1);
        DrawText(posStr, posX + 25, posY + 30, 8, {200, 200, 200, 255});

        // Linha e seta para próximo nó
        if (current->next != NULL) {
            if ((index + 1) % itemsPerRow == 0) {
                // Próxima linha - NÃO desenhar linha abaixo, apenas seta diagonal
                // Seta para baixo-esquerda
                DrawTriangle(
                    {(float)(posX + 35), (float)(posY + 50)},
                    {(float)(vizX + 35), (float)(posY + 65)},
                    {(float)(posX + 25), (float)(posY + 60)},
                    PRIMARY_COLOR
                );
            } else {
                // Mesma linha - seta para direita
                // Desenhar linha horizontal
                DrawLine(posX + 70, posY + 25, posX + 85, posY + 25, PRIMARY_COLOR);
                // Seta para direita
                DrawTriangle(
                    {(float)(posX + 85), (float)(posY + 25)},
                    {(float)(posX + 78), (float)(posY + 21)},
                    {(float)(posX + 78), (float)(posY + 29)},
                    PRIMARY_COLOR
                );
            }
        } else {
            // Nó final - mostrar NULL
            DrawText("NULL", posX + 50, posY + 10, 8, ERROR_COLOR);
        }

        // Atualizar posição para próximo item
        if ((index + 1) % itemsPerRow == 0) {
            // Ir para próxima linha
            posX = vizX;
            posY += 80;
        } else {
            // Ir para próxima coluna
            posX += 100;
        }

        current = current->next;
        index++;
    }

    // Instruções
    DrawText("OPERACOES:", 50, 480, 18, PRIMARY_COLOR);
    DrawText("[I] Inserir | [R] Remover Nome | [D] Remover Index | [ESC] Menu", 50, 520, 14, {200, 200, 200, 255});

    sprintf(buffer, "Total: %d cargas", ui->loads->counter);
    DrawText(buffer, 50, 555, 12, {150, 150, 150, 255});

    renderStatusBar(ui);
}

void updateCargasModule(UIManager *ui) {
    static int removeMode = 0; // 0: none, 1: insert, 2: remove by name, 3: remove by index

    if (IsKeyPressed(KEY_I) && removeMode == 0) {
        ui->isInputActive = true;
        removeMode = 1;
        ui->inputLength = 0;
        memset(ui->inputBuffer, 0, sizeof(ui->inputBuffer));
        skipNextInput = true;
    }

    if (IsKeyPressed(KEY_R) && removeMode == 0) {
        ui->isInputActive = true;
        removeMode = 2;
        ui->inputLength = 0;
        memset(ui->inputBuffer, 0, sizeof(ui->inputBuffer));
        skipNextInput = true;
    }

    if (IsKeyPressed(KEY_D) && removeMode == 0) {
        ui->isInputActive = true;
        removeMode = 3;
        ui->inputLength = 0;
        memset(ui->inputBuffer, 0, sizeof(ui->inputBuffer));
        skipNextInput = true;
    }

    if (ui->isInputActive) {
        if (!skipNextInput) {
            handleInput(ui);
        } else {
            skipNextInput = false;
        }

        if (IsKeyPressed(KEY_ENTER)) {
            if (ui->inputLength > 0) {
                if (removeMode == 1) {
                    // Inserir carga
                    if (insertLoad(ui->loads, ui->inputBuffer)) {
                        char msg[256];
                        sprintf(msg, "Carga '%s' inserida!", ui->inputBuffer);
                        addStatusMessage(ui, msg, 1.5f, SUCCESS_COLOR);
                    } else {
                        addStatusMessage(ui, "Erro ao inserir carga!", 1.5f, ERROR_COLOR);
                    }
                } else if (removeMode == 2) {
                    // Remover por nome
                    char *result = removeLoad(ui->loads, ui->inputBuffer);
                    if (result != NULL && strlen(result) > 0) {
                        char msg[256];
                        sprintf(msg, "Carga '%s' removida!", result);
                        addStatusMessage(ui, msg, 1.5f, SUCCESS_COLOR);
                        free(result);
                    } else {
                        addStatusMessage(ui, "Carga não encontrada!", 1.5f, ERROR_COLOR);
                    }
                } else if (removeMode == 3) {
                    // Remover por índice
                    int index = atoi(ui->inputBuffer);
                    char *result = removeLoad(ui->loads, index);
                    if (result != NULL && strlen(result) > 0) {
                        char msg[256];
                        sprintf(msg, "Carga '%s' removida!", result);
                        addStatusMessage(ui, msg, 1.5f, SUCCESS_COLOR);
                        free(result);
                    } else {
                        addStatusMessage(ui, "Índice inválido!", 1.5f, ERROR_COLOR);
                    }
                }
            }
            ui->isInputActive = false;
            removeMode = 0;
            ui->inputLength = 0;
            memset(ui->inputBuffer, 0, sizeof(ui->inputBuffer));
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            ui->isInputActive = false;
            removeMode = 0;
            ui->inputLength = 0;
            memset(ui->inputBuffer, 0, sizeof(ui->inputBuffer));
        }
    }
}

// ========== MÓDULO NAVES ==========

void renderNavesModule(UIManager *ui) {
    Background();

    char buffer[256];
    sprintf(buffer, "Total de Naves: %d", ui->ships->count);
    DrawText("HISTORICO DE NAVES (Lista Duplamente Encadeada)", 50, 50, 32, PRIMARY_COLOR);
    DrawText(buffer, 50, 100, 18, {200, 200, 200, 255});

    int listX = 50;
    int listY = 150;
    DrawRectangle(listX, listY, 350, 300, {30, 40, 80, 150});
    DrawRectangleLines(listX, listY, 350, 300, PRIMARY_COLOR);
    DrawText("Lista:", listX + 10, listY + 10, 16, PRIMARY_COLOR);

    Node *current = ui->ships->head;
    int index = 0;
    while (current != NULL && index < 6) {
        char numStr[16];
        sprintf(numStr, "%d", index + 1);
        DrawText(numStr, listX + 20, listY + 50 + (index * 40), 14, {150, 150, 150, 255});
        DrawText(current->name, listX + 60, listY + 50 + (index * 40), 14, SUCCESS_COLOR);
        current = current->next;
        index++;
    }

    int vizX = 420;
    int vizY = 150;
    DrawText("Visualizacao:", vizX, vizY - 30, 16, PRIMARY_COLOR);

    Color colorNext = {0, 255, 100, 255};
    Color colorPrev = {255, 80, 80, 255};

    int itemsPerRow = 6;
    int nodeW = 70;
    int nodeH = 50;
    int stepX = 110;
    int stepY = 100;

    const int MAX_NODES = 18;
    int nodeX[MAX_NODES], nodeY[MAX_NODES];
    int nodeCount = 0;

    current = ui->ships->head;
    while (current != NULL && nodeCount < MAX_NODES) {
        int row = nodeCount / itemsPerRow;
        int col = nodeCount % itemsPerRow;
        nodeX[nodeCount] = vizX + col * stepX;
        nodeY[nodeCount] = vizY + row * stepY;
        nodeCount++;
        current = current->next;
    }

    // — Primeira passagem: desenhar os nós —
    current = ui->ships->head;
    for (int i = 0; i < nodeCount; i++) {
        int nx = nodeX[i];
        int ny = nodeY[i];

        DrawRectangleRounded({(float)nx, (float)ny, (float)nodeW, (float)nodeH}, 0.05f, 8, {100, 200, 150, 200});
        DrawRectangleRoundedLines({(float)nx, (float)ny, (float)nodeW, (float)nodeH}, 0.05f, 8, PRIMARY_COLOR);

        int textWidth = MeasureText(current->name, 9);
        if (textWidth > 60) {
            char truncated[12];
            strncpy(truncated, current->name, 7);
            truncated[7] = '\0';
            strcat(truncated, ".");
            DrawText(truncated, nx + 5, ny + 15, 9, WHITE);
        } else {
            DrawText(current->name, nx + (nodeW - textWidth) / 2, ny + 15, 9, WHITE);
        }

        char posStr[8];
        sprintf(posStr, "#%d", i + 1);
        DrawText(posStr, nx + 25, ny + 35, 8, {200, 200, 200, 255});

        current = current->next;
    }

    // — Segunda passagem: desenhar setas POR CIMA —
    for (int i = 0; i < nodeCount - 1; i++) {
        int cx = nodeX[i];
        int cy = nodeY[i];
        int nx = nodeX[i+1];
        int ny = nodeY[i+1];
        bool isLastInRow = ((i + 1) % itemsPerRow == 0);

        if (!isLastInRow) {
            // NEXT (verde →): topo
            int yNext = cy + 12;
            DrawLine(cx + nodeW, yNext, nx, yNext, colorNext);
            DrawTriangle(
                {(float)(nx),      (float)(yNext)},
                {(float)(nx - 10), (float)(yNext - 5)},
                {(float)(nx - 10), (float)(yNext + 5)},
                colorNext
            );

            // PREV (vermelho ←): base
            int yPrev = cy + 38;
            DrawLine(nx, yPrev, cx + nodeW, yPrev, colorPrev);
            DrawTriangle(
                {(float)(cx + nodeW),      (float)(yPrev)},
                {(float)(cx + nodeW - 10), (float)(yPrev - 5)},
                {(float)(cx + nodeW - 10), (float)(yPrev + 5)},
                colorPrev
            );

        } else {
            // Quebra de linha

            // NEXT (verde →): sai direita-baixo do atual, chega na borda esquerda do próximo
            int x0 = cx + nodeW;
            int y0 = cy + nodeH - 8;
            int x1 = nx - 1;
            int y1 = ny + nodeH / 2;
            DrawLine(x0, y0, x1, y1, colorNext);
            DrawTriangle(
                {(float)(x1 + 10), (float)(y1)},      // ponta →
                {(float)(x1),      (float)(y1 - 6)},
                {(float)(x1),      (float)(y1 + 6)},
                colorNext
            );

            // PREV (vermelho ←): sai borda esquerda do próximo, chega direita-cima do atual
            int xa = nx - 1;
            int ya = ny + nodeH / 2 + 12;
            int xb = cx + nodeW;
            int yb = cy + 8;
            DrawLine(xa, ya, xb, yb, colorPrev);
            DrawTriangle(
                {(float)(xb),      (float)(yb)},
                {(float)(xb - 12), (float)(yb - 6)},
                {(float)(xb - 12), (float)(yb + 6)},
                colorPrev
            );
        }
    }

    int legendY = vizY + ((nodeCount - 1) / itemsPerRow + 1) * stepY + 10;
    DrawText("Verde (→) = next",    vizX,       legendY, 11, colorNext);
    DrawText("Vermelho (←) = prev", vizX + 200, legendY, 11, colorPrev);

    DrawText("OPERACOES:", 50, 480, 18, PRIMARY_COLOR);
    DrawText("[I] Inserir | [R] Remove Nome | [D] Remove Index | [A] Remove Inicio | [T] Remove Fim | [ESC] Menu", 50, 520, 13, {200, 200, 200, 255});

    sprintf(buffer, "Total: %d naves", ui->ships->count);
    DrawText(buffer, 50, 555, 12, {150, 150, 150, 255});

    renderStatusBar(ui);
}

void updateNavesModule(UIManager *ui) {
    static int removeMode = 0; // 0: none, 1: insert, 2: remove by name, 3: remove by index

    if (IsKeyPressed(KEY_I) && removeMode == 0) {
        ui->isInputActive = true;
        removeMode = 1;
        ui->inputLength = 0;
        memset(ui->inputBuffer, 0, sizeof(ui->inputBuffer));
        skipNextInput = true;
    }

    if (IsKeyPressed(KEY_R) && removeMode == 0) {
        ui->isInputActive = true;
        removeMode = 2;
        ui->inputLength = 0;
        memset(ui->inputBuffer, 0, sizeof(ui->inputBuffer));
        skipNextInput = true;
    }

    if (IsKeyPressed(KEY_D) && removeMode == 0) {
        ui->isInputActive = true;
        removeMode = 3;
        ui->inputLength = 0;
        memset(ui->inputBuffer, 0, sizeof(ui->inputBuffer));
        skipNextInput = true;
    }

    if (IsKeyPressed(KEY_A) && removeMode == 0) {
        // Remover do início
        char *removed = deleteShip(ui->ships, false);
        if (removed != NULL && strlen(removed) > 0) {
            char msg[256];
            sprintf(msg, "Nave %s removida do inicio!", removed);
            addStatusMessage(ui, msg, 1.5f, WARNING_COLOR);
            free(removed);
        } else {
            addStatusMessage(ui, "Nenhuma nave para remover!", 1.5f, ERROR_COLOR);
        }
    }

    if (IsKeyPressed(KEY_T) && removeMode == 0) {
        // Remover do fim
        char *removed = deleteShip(ui->ships, true);
        if (removed != NULL && strlen(removed) > 0) {
            char msg[256];
            sprintf(msg, "Nave %s removida do fim!", removed);
            addStatusMessage(ui, msg, 1.5f, WARNING_COLOR);
            free(removed);
        } else {
            addStatusMessage(ui, "Nenhuma nave para remover!", 1.5f, ERROR_COLOR);
        }
    }

    if (ui->isInputActive) {
        if (!skipNextInput) {
            handleInput(ui);
        } else {
            skipNextInput = false;
        }

        if (IsKeyPressed(KEY_ENTER)) {
            if (ui->inputLength > 0) {
                if (removeMode == 1) {
                    // Inserir nave
                    if (insertShip(ui->ships, ui->inputBuffer, true)) {
                        char msg[256];
                        sprintf(msg, "Nave '%s' inserida!", ui->inputBuffer);
                        addStatusMessage(ui, msg, 1.5f, SUCCESS_COLOR);
                    } else {
                        addStatusMessage(ui, "Erro ao inserir nave!", 1.5f, ERROR_COLOR);
                    }
                } else if (removeMode == 2) {
                    // Remover por nome
                    char *result = deleteShip(ui->ships, ui->inputBuffer);
                    if (result != NULL && strlen(result) > 0) {
                        char msg[256];
                        sprintf(msg, "Nave '%s' removida!", result);
                        addStatusMessage(ui, msg, 1.5f, SUCCESS_COLOR);
                        free(result);
                    } else {
                        addStatusMessage(ui, "Nave não encontrada!", 1.5f, ERROR_COLOR);
                    }
                } else if (removeMode == 3) {
                    // Remover por índice
                    int index = atoi(ui->inputBuffer);
                    char *result = deleteShip(ui->ships, index);
                    if (result != NULL && strlen(result) > 0) {
                        char msg[256];
                        sprintf(msg, "Nave '%s' removida!", result);
                        addStatusMessage(ui, msg, 1.5f, SUCCESS_COLOR);
                        free(result);
                    } else {
                        addStatusMessage(ui, "Índice inválido!", 1.5f, ERROR_COLOR);
                    }
                }
            }
            ui->isInputActive = false;
            removeMode = 0;
            ui->inputLength = 0;
            memset(ui->inputBuffer, 0, sizeof(ui->inputBuffer));
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            ui->isInputActive = false;
            removeMode = 0;
            ui->inputLength = 0;
            memset(ui->inputBuffer, 0, sizeof(ui->inputBuffer));
        }
    }
}

// ========== MÓDULO MONITORAMENTO ==========

void renderMonitoramentoModule(UIManager *ui) {
    Background();

    char buffer[256];
    sprintf(buffer, "Total de Cameras: %d", ui->monitoring->count);
    DrawText("MONITORAMENTO (Lista Circular)", 50, 50, 32, PRIMARY_COLOR);
    DrawText(buffer, 50, 100, 18, {200, 200, 200, 255});

    int centerX = 300;
    int centerY = 350;
    int radius = 150;

    DrawCircleLines(centerX, centerY, radius, PRIMARY_COLOR);

    Node *current = ui->monitoring->head;
    int cameraCount = ui->monitoring->count;

    for (int i = 0; i < cameraCount && i < 8; i++) {
        float angle = (2.0f * 3.14159f * i) / (cameraCount > 0 ? cameraCount : 1);
        int camX = centerX + (int)(radius * cosf(angle));
        int camY = centerY + (int)(radius * sinf(angle));

        bool isFirst = (i == 0);
        bool isLast  = (i == cameraCount - 1);

        Color circleColor;
        if (isFirst) {
            circleColor = {255, 200, 50, 255};
        } else if (isLast) {
            circleColor = {255, 80, 80, 255};
        } else {
            circleColor = {100, 150, 200, 255};
        }

        DrawCircle(camX, camY, 20, circleColor);
        DrawCircleLines(camX, camY, 20, PRIMARY_COLOR);

        if (isFirst) {
            DrawText("S", camX - 5, camY - 8, 18, {20, 20, 20, 255});
        } else if (isLast) {
            DrawText("L", camX - 5, camY - 8, 18, {20, 20, 20, 255});
        }

        if (current != NULL) {
            DrawText(current->name, camX - 15, camY + 30, 12, {200, 200, 200, 255});
            current = current->next;
        }
    }

    // Legenda
    DrawCircle(550, 160, 8, {255, 200, 50, 255});
    DrawText("= Primeiro (S)", 565, 153, 13, {200, 200, 200, 255});
    DrawCircle(550, 185, 8, {255, 80, 80, 255});
    DrawText("= Ultimo (L)",   565, 178, 13, {200, 200, 200, 255});
    DrawCircle(550, 210, 8, {100, 150, 200, 255});
    DrawText("= Camera",       565, 203, 13, {200, 200, 200, 255});

    DrawText("OPERACOES:", 550, 240, 18, PRIMARY_COLOR);
    DrawText("[I] Inserir Camera",     550, 280, 16, {200, 200, 200, 255});
    DrawText("[R] Remover por Nome",   550, 315, 16, {200, 200, 200, 255});
    DrawText("[D] Remover por Indice", 550, 350, 16, {200, 200, 200, 255});
    DrawText("[A] Remover do Inicio",  550, 385, 16, {200, 200, 200, 255});
    DrawText("[T] Remover do Fim",     550, 420, 16, {200, 200, 200, 255});
    DrawText("[ESC] Menu Principal",   550, 460, 16, {150, 150, 150, 200});

    sprintf(buffer, "Total: %d cameras", ui->monitoring->count);
    DrawText(buffer, 550, 500, 14, {150, 150, 150, 255});

    renderStatusBar(ui);
}

void updateMonitoramentoModule(UIManager *ui) {
    static int removeMode = 0; // 0: none, 1: insert, 2: remove by name, 3: remove by index

    if (IsKeyPressed(KEY_I) && removeMode == 0) {
        ui->isInputActive = true;
        removeMode = 1;
        ui->inputLength = 0;
        memset(ui->inputBuffer, 0, sizeof(ui->inputBuffer));
        skipNextInput = true;
    }

    if (IsKeyPressed(KEY_R) && removeMode == 0) {
        ui->isInputActive = true;
        removeMode = 2;
        ui->inputLength = 0;
        memset(ui->inputBuffer, 0, sizeof(ui->inputBuffer));
        skipNextInput = true;
    }

    if (IsKeyPressed(KEY_D) && removeMode == 0) {
        ui->isInputActive = true;
        removeMode = 3;
        ui->inputLength = 0;
        memset(ui->inputBuffer, 0, sizeof(ui->inputBuffer));
        skipNextInput = true;
    }

    if (IsKeyPressed(KEY_A) && removeMode == 0) {
        // Remover primeira (head)
        if (removeCamera(ui->monitoring, false)) {
            addStatusMessage(ui, "Camera removida do inicio!", 1.5f, WARNING_COLOR);
        } else {
            addStatusMessage(ui, "Nenhuma camera para remover!", 1.5f, ERROR_COLOR);
        }
    }

    if (IsKeyPressed(KEY_T) && removeMode == 0) {
        // Remover última (tail)
        if (removeCamera(ui->monitoring, true)) {
            addStatusMessage(ui, "Camera removida do fim!", 1.5f, WARNING_COLOR);
        } else {
            addStatusMessage(ui, "Nenhuma camera para remover!", 1.5f, ERROR_COLOR);
        }
    }

    if (ui->isInputActive) {
        if (!skipNextInput) {
            handleInput(ui);
        } else {
            skipNextInput = false;
        }

        if (IsKeyPressed(KEY_ENTER)) {
            if (ui->inputLength > 0) {
                if (removeMode == 1) {
                    // Inserir camera
                    if (insertCamera(ui->monitoring, ui->inputBuffer)) {
                        char msg[256];
                        sprintf(msg, "Camera '%s' inserida!", ui->inputBuffer);
                        addStatusMessage(ui, msg, 1.5f, SUCCESS_COLOR);
                    } else {
                        addStatusMessage(ui, "Erro ao inserir camera!", 1.5f, ERROR_COLOR);
                    }
                } else if (removeMode == 2) {
                    // Remover por nome
                    if (removeCamera(ui->monitoring, ui->inputBuffer)) {
                        char msg[256];
                        sprintf(msg, "Camera '%s' removida!", ui->inputBuffer);
                        addStatusMessage(ui, msg, 1.5f, SUCCESS_COLOR);
                    } else {
                        addStatusMessage(ui, "Camera não encontrada!", 1.5f, ERROR_COLOR);
                    }
                } else if (removeMode == 3) {
                    // Remover por índice
                    int index = atoi(ui->inputBuffer);
                    if (removeCamera(ui->monitoring, index)) {
                        char msg[256];
                        sprintf(msg, "Camera indice %d removida!", index);
                        addStatusMessage(ui, msg, 1.5f, SUCCESS_COLOR);
                    } else {
                        addStatusMessage(ui, "Índice inválido!", 1.5f, ERROR_COLOR);
                    }
                }
            }
            ui->isInputActive = false;
            removeMode = 0;
            ui->inputLength = 0;
            memset(ui->inputBuffer, 0, sizeof(ui->inputBuffer));
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            ui->isInputActive = false;
            removeMode = 0;
            ui->inputLength = 0;
            memset(ui->inputBuffer, 0, sizeof(ui->inputBuffer));
        }
    }
}

// ========== MÓDULO LOGS ==========

void renderLogsModule(UIManager *ui) {
    Background();

    char buffer[256];
    sprintf(buffer, "Total de Logs: %d", ui->logs->counter);
    DrawText("CENTRAL DE LOGS (Pilha - LIFO)", 50, 50, 32, PRIMARY_COLOR);
    DrawText(buffer, 50, 100, 18, {200, 200, 200, 255});

    // ========== LISTA ==========
    int listX = 50;
    int listY = 150;
    DrawRectangleRounded({(float)listX, (float)listY, 300, 300}, 0.05f, 10, {20, 30, 60, 180});
    DrawRectangleRoundedLines({(float)listX, (float)listY, 300, 300}, 0.05f, 10, PRIMARY_COLOR);
    DrawText("Pilha (LIFO):", listX + 10, listY + 10, 16, PRIMARY_COLOR);
    DrawLine(listX + 10, listY + 35, listX + 290, listY + 35, PRIMARY_COLOR);

    Node *current = ui->logs->head;
    int index = 0;
    while (current != NULL && index < 6) {
        int itemY = listY + 50 + (index * 38);

        Color itemColor = (index == 0)
            ? Color{0, 180, 100, 180}
            : Color{40, 60, 120, 160};
        DrawRectangleRounded({(float)(listX + 10), (float)itemY, 280, 30}, 0.1f, 8, itemColor);
        DrawRectangleRoundedLines({(float)(listX + 10), (float)itemY, 280, 30}, 0.1f, 8,
            (index == 0) ? SUCCESS_COLOR : PRIMARY_COLOR);

        if (index == 0) {
            DrawText("TOP", listX + 15, itemY + 8, 12, WARNING_COLOR);
            DrawText(current->name, listX + 60, itemY + 8, 14, WHITE);
        } else {
            char numStr[8];
            sprintf(numStr, "%d", index + 1);
            DrawText(numStr, listX + 20, itemY + 8, 12, {150, 150, 150, 255});
            DrawText(current->name, listX + 60, itemY + 8, 14, {200, 200, 200, 255});
        }

        current = current->prev;  // ← corrigido
        index++;
    }

    // ========== PILHA VISUAL ==========
    int stackCenterX = 900;
    int stackBaseY   = 500;
    int blockW       = 260;
    int blockH       = 44;
    int maxVisible   = 7;
    int gap          = 4;

    DrawText("Visualizacao da Pilha:", stackCenterX - blockW / 2,
        stackBaseY - maxVisible * (blockH + gap) - 60, 18, PRIMARY_COLOR);

    // base da pilha
    DrawRectangleRounded({
        (float)(stackCenterX - blockW / 2 - 10),
        (float)(stackBaseY),
        (float)(blockW + 20), 16
    }, 0.2f, 8, PRIMARY_COLOR);

    // seta PUSH/POP
    int arrowX    = stackCenterX + blockW / 2 + 20;
    int arrowTopY = stackBaseY - maxVisible * (blockH + gap) - 20;
    int arrowBotY = stackBaseY - (blockH + gap);
    DrawLine(arrowX, arrowBotY, arrowX, arrowTopY, WARNING_COLOR);
    DrawTriangle(
        {(float)(arrowX),     (float)(arrowTopY - 10)},
        {(float)(arrowX - 8), (float)(arrowTopY)},
        {(float)(arrowX + 8), (float)(arrowTopY)},
        WARNING_COLOR
    );
    DrawText("PUSH", arrowX + 14, arrowTopY, 12, WARNING_COLOR);
    DrawText("POP",  arrowX + 14, arrowBotY - 14, 12, ERROR_COLOR);

    // — coletar nomes em array seguro percorrendo ->prev —
    int totalLogs = ui->logs->counter;
    int showCount = (totalLogs < maxVisible) ? totalLogs : maxVisible;

    char namesCopy[7][64];
    Node *tmp = ui->logs->head;
    for (int i = 0; i < showCount && tmp != NULL; i++) {
        strncpy(namesCopy[i], tmp->name, 63);
        namesCopy[i][63] = '\0';
        tmp = tmp->prev;  // ← corrigido
    }

    // — desenhar blocos de baixo para cima —
    for (int i = showCount - 1; i >= 0; i--) {
        int posInStack = showCount - 1 - i;
        int blockY = stackBaseY - (posInStack + 1) * (blockH + gap);
        int bx = stackCenterX - blockW / 2;
        bool isTop = (i == 0);

        float t = (showCount > 1)
            ? (float)posInStack / (float)(showCount - 1)
            : 1.0f;

        Color blockColor = {
            (unsigned char)(30  + (int)(t * 80)),
            (unsigned char)(80  + (int)(t * 120)),
            (unsigned char)(160 + (int)(t * 80)),
            220
        };
        Color borderColor = isTop ? SUCCESS_COLOR : PRIMARY_COLOR;

        // sombra
        DrawRectangleRounded({(float)(bx + 4), (float)(blockY + 4), (float)blockW, (float)blockH},
            0.08f, 8, {0, 0, 0, 80});

        // bloco
        DrawRectangleRounded({(float)bx, (float)blockY, (float)blockW, (float)blockH},
            0.08f, 8, blockColor);
        DrawRectangleRoundedLines({(float)bx, (float)blockY, (float)blockW, (float)blockH},
            0.08f, 8, borderColor);

        // label TOP
        if (isTop) {
            DrawText("<TOP", bx - 50, blockY + blockH / 2 - 7, 14, WARNING_COLOR);
        }

        // número
        char posStr[8];
        sprintf(posStr, "#%d", posInStack + 1);
        DrawText(posStr, bx + 8, blockY + blockH / 2 - 7, 13, {180, 180, 180, 255});

        // nome
        int textWidth = MeasureText(namesCopy[i], 14);
        DrawText(namesCopy[i], bx + (blockW - textWidth) / 2, blockY + blockH / 2 - 7, 14, WHITE);
    }

    if (totalLogs > maxVisible) {
        char moreStr[32];
        sprintf(moreStr, "+ %d logs nao exibidos", totalLogs - maxVisible);
        DrawText(moreStr, stackCenterX - blockW / 2,
            stackBaseY - maxVisible * (blockH + gap) - 80, 13, {150, 150, 150, 255});
    }

    DrawText("LIFO: Ultimo a entrar, primeiro a sair",
        stackCenterX - blockW / 2, stackBaseY + 30, 13, {150, 150, 150, 255});

    DrawText("OPERACOES:", 50, 480, 18, PRIMARY_COLOR);
    DrawText("[I] Push (Adicionar) | [R] Pop (Remover) | [ESC] Menu",
        50, 515, 14, {200, 200, 200, 255});

    renderStatusBar(ui);
}

void updateLogsModule(UIManager *ui) {
    static int removeMode = 0; // 0: none, 1: push

    if (IsKeyPressed(KEY_I) && removeMode == 0) {
        ui->isInputActive = true;
        removeMode = 1;
        ui->inputLength = 0;
        memset(ui->inputBuffer, 0, sizeof(ui->inputBuffer));
        skipNextInput = true;
    }

    if (IsKeyPressed(KEY_R) && removeMode == 0) {
        // Pop imediato
        char *removed = popLog(ui->logs);
        if (removed != NULL && strlen(removed) > 0) {
            char msg[256];
            sprintf(msg, "Log removido: %s", removed);
            addStatusMessage(ui, msg, 1.5f, WARNING_COLOR);
            free(removed);
        } else {
            addStatusMessage(ui, "Pilha vazia!", 1.5f, ERROR_COLOR);
        }
    }

    if (ui->isInputActive) {
        if (!skipNextInput) {
            handleInput(ui);
        } else {
            skipNextInput = false;
        }

        if (IsKeyPressed(KEY_ENTER)) {
            if (ui->inputLength > 0) {
                if (pushLog(ui->logs, ui->inputBuffer)) {
                    addStatusMessage(ui, "Log adicionado!", 1.5f, SUCCESS_COLOR);
                } else {
                    addStatusMessage(ui, "Erro ao adicionar log!", 1.5f, ERROR_COLOR);
                }
            }
            ui->isInputActive = false;
            removeMode = 0;
            ui->inputLength = 0;
            memset(ui->inputBuffer, 0, sizeof(ui->inputBuffer));
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            ui->isInputActive = false;
            removeMode = 0;
            ui->inputLength = 0;
            memset(ui->inputBuffer, 0, sizeof(ui->inputBuffer));
        }
    }
}

// ========== MÓDULO HANGAR ==========

void renderHangarModule(UIManager *ui) {
    Background();

    char buffer[256];
    sprintf(buffer, "Total de Naves em Fila: %d", ui->hangar->counter);
    DrawText("CONTROLE DE HANGAR (Fila - FIFO)", 50, 50, 32, PRIMARY_COLOR);
    DrawText(buffer, 50, 100, 18, {200, 200, 200, 255});

    // ========== LISTA ==========
    int listX = 50;
    int listY = 150;
    DrawRectangleRounded({(float)listX, (float)listY, 300, 300}, 0.05f, 10, {20, 30, 60, 180});
    DrawRectangleRoundedLines({(float)listX, (float)listY, 300, 300}, 0.05f, 10, PRIMARY_COLOR);
    DrawText("Fila (FIFO):", listX + 10, listY + 10, 16, PRIMARY_COLOR);
    DrawLine(listX + 10, listY + 35, listX + 290, listY + 35, PRIMARY_COLOR);

    Node *current = ui->hangar->head;
    int index = 0;
    while (current != NULL && index < 6) {
        int itemY = listY + 50 + (index * 38);

        Color itemColor = (index == 0)
            ? Color{180, 60, 60, 180}
            : Color{40, 60, 120, 160};
        Color borderCol = (index == 0) ? ERROR_COLOR : PRIMARY_COLOR;

        DrawRectangleRounded({(float)(listX + 10), (float)itemY, 280, 30}, 0.1f, 8, itemColor);
        DrawRectangleRoundedLines({(float)(listX + 10), (float)itemY, 280, 30}, 0.1f, 8, borderCol);

        if (index == 0) {
            DrawText("FRONT", listX + 12, itemY + 8, 11, WARNING_COLOR);
            DrawText(current->name, listX + 75, itemY + 8, 14, WHITE);
        } else {
            char numStr[8];
            sprintf(numStr, "%d", index + 1);
            DrawText(numStr, listX + 20, itemY + 8, 12, {150, 150, 150, 255});
            DrawText(current->name, listX + 60, itemY + 8, 14, {200, 200, 200, 255});
        }

        current = current->next;
        index++;
    }

    if (ui->hangar->counter > 6) {
        char moreStr[32];
        sprintf(moreStr, "+ %d naves nao exibidas", ui->hangar->counter - 6);
        DrawText(moreStr, listX + 10, listY + 270, 12, {150, 150, 150, 255});
    }

    // ========== FILA VISUAL ==========
    int maxVisible = 5;
    int blockW     = 110;
    int blockH     = 60;
    int gap        = 10;
    int totalW     = maxVisible * blockW + (maxVisible - 1) * gap;
    int startX     = (1280 - totalW) / 2 + 80;
    int blockY     = 320;

    DrawText("Visualizacao da Fila:", startX, blockY - 50, 18, PRIMARY_COLOR);

    int totalH    = ui->hangar->counter;
    int showCount = (totalH < maxVisible) ? totalH : maxVisible;
    int midY      = blockY + blockH / 2;

    // — seta ENTRADA (esquerda) —
    int entradaX = startX - 70;
    DrawLine(entradaX, midY, startX - 5, midY, SUCCESS_COLOR);
    DrawTriangle(
        {(float)(startX - 5),  (float)(midY)},
        {(float)(startX - 18), (float)(midY - 8)},
        {(float)(startX - 18), (float)(midY + 8)},
        SUCCESS_COLOR
    );
    DrawText("ENTRADA", entradaX - 10, midY - 28, 13, SUCCESS_COLOR);
    DrawText("enqueue", entradaX - 10, midY + 14, 11, {150, 200, 150, 255});

    // — coletar nomes —
    char namesCopy[5][64];
    bool isHiddenSlot[5] = {false, false, false, false, false};

    if (totalH <= maxVisible) {
        // menos que 5: mostra todos normalmente
        Node *tmp = ui->hangar->head;
        for (int i = 0; i < showCount && tmp != NULL; i++) {
            strncpy(namesCopy[i], tmp->name, 63);
            namesCopy[i][63] = '\0';
            tmp = tmp->next;
        }
    } else {
        // mais que 5: FRONT no slot 0, slot 1 = ocultos, slots 2-4 = ultimos 3 + BACK
        strncpy(namesCopy[0], ui->hangar->head->name, 63);
        namesCopy[0][63] = '\0';
        isHiddenSlot[1] = true;
        strncpy(namesCopy[1], "", 1);

        // pegar os ultimos 3 do tail para tras
        Node *tmp = ui->hangar->tail;
        for (int i = maxVisible - 1; i >= 2 && tmp != NULL; i--) {
            strncpy(namesCopy[i], tmp->name, 63);
            namesCopy[i][63] = '\0';
            tmp = tmp->prev;
        }
    }

    // — desenhar blocos —
    for (int i = 0; i < maxVisible; i++) {
        int bx       = startX + i * (blockW + gap);
        bool hasNave = (i < showCount);
        bool isFront = (i == 0 && hasNave);
        bool isBack  = (i == showCount - 1 && hasNave && showCount > 1);
        bool isHidden = isHiddenSlot[i];

        Color blockColor;
        if (!hasNave) {
            blockColor = {30, 40, 70, 120};
        } else if (isHidden) {
            blockColor = {40, 50, 80, 160};
        } else {
            float t = (showCount > 1) ? (float)i / (float)(showCount - 1) : 0.0f;
            blockColor = {
                (unsigned char)(180 - (int)(t * 120)),
                (unsigned char)(60  + (int)(t * 100)),
                (unsigned char)(60  + (int)(t * 140)),
                220
            };
        }

        Color borderColor = isFront  ? ERROR_COLOR
                          : isBack   ? SUCCESS_COLOR
                          : hasNave  ? PRIMARY_COLOR
                                     : Color{60, 70, 100, 150};

        // sombra
        DrawRectangleRounded({(float)(bx + 3), (float)(blockY + 3), (float)blockW, (float)blockH},
            0.08f, 8, {0, 0, 0, 80});

        // bloco
        DrawRectangleRounded({(float)bx, (float)blockY, (float)blockW, (float)blockH},
            0.08f, 8, blockColor);
        DrawRectangleRoundedLines({(float)bx, (float)blockY, (float)blockW, (float)blockH},
            0.08f, 8, borderColor);

        if (hasNave) {
            // label FRONT / BACK
            if (isFront) {
                DrawText("FRONT", bx + blockW / 2 - MeasureText("FRONT", 11) / 2,
                    blockY + 6, 11, WARNING_COLOR);
            } else if (isBack) {
                DrawText("BACK", bx + blockW / 2 - MeasureText("BACK", 11) / 2,
                    blockY + 6, 11, {150, 220, 150, 255});
            }

            if (isHidden) {
                char hiddenStr[16];
                sprintf(hiddenStr, "... +%d", totalH - maxVisible);
                int tw = MeasureText(hiddenStr, 13);
                DrawText(hiddenStr, bx + (blockW - tw) / 2, blockY + blockH / 2 - 6, 13, {180, 180, 180, 255});
            } else {
                int tw = MeasureText(namesCopy[i], 14);
                DrawText(namesCopy[i], bx + (blockW - tw) / 2, blockY + blockH / 2 - 5, 14, WHITE);

                // número da posição real
                char posStr[8];
                if (isFront) {
                    sprintf(posStr, "#1");
                } else if (isBack) {
                    sprintf(posStr, "#%d", totalH);
                } else {
                    sprintf(posStr, "#%d", totalH - (showCount - 1 - i));
                }
                DrawText(posStr, bx + 5, blockY + blockH - 16, 10, {180, 180, 180, 255});
            }
        } else {
            DrawText("---", bx + blockW / 2 - MeasureText("---", 13) / 2,
                blockY + blockH / 2 - 6, 13, {80, 80, 100, 180});
        }

        // seta entre blocos
        if (i < maxVisible - 1) {
            int ax = bx + blockW + 1;
            int ay = blockY + blockH / 2;
            Color arrowColor = hasNave ? PRIMARY_COLOR : Color{60, 70, 100, 150};
            DrawLine(ax, ay, ax + gap - 1, ay, arrowColor);
            DrawTriangle(
                {(float)(ax + gap - 1), (float)(ay)},
                {(float)(ax + gap - 9), (float)(ay - 5)},
                {(float)(ax + gap - 9), (float)(ay + 5)},
                arrowColor
            );
        }
    }

    // — seta SAIDA (direita) —
    int saidaX = startX + maxVisible * (blockW + gap);
    DrawLine(saidaX, midY, saidaX + 60, midY, ERROR_COLOR);
    DrawTriangle(
        {(float)(saidaX + 60), (float)(midY)},
        {(float)(saidaX + 48), (float)(midY - 8)},
        {(float)(saidaX + 48), (float)(midY + 8)},
        ERROR_COLOR
    );
    DrawText("SAIDA",   saidaX + 10, midY - 28, 13, ERROR_COLOR);
    DrawText("dequeue", saidaX + 10, midY + 14, 11, {200, 150, 150, 255});

    // — trilho —
    DrawLine(startX - 5, blockY + blockH + 8,
             startX + maxVisible * (blockW + gap) - gap, blockY + blockH + 8,
             PRIMARY_COLOR);

    // — legenda —
    DrawRectangleRounded({(float)startX, (float)(blockY + blockH + 20), 14, 14},
        0.2f, 4, {180, 60, 60, 220});
    DrawText("= FRONT (proximo a sair)", startX + 20, blockY + blockH + 20, 12, {200, 200, 200, 255});

    DrawRectangleRounded({(float)startX, (float)(blockY + blockH + 40), 14, 14},
        0.2f, 4, {60, 160, 200, 220});
    DrawText("= BACK (ultimo a entrar)", startX + 20, blockY + blockH + 40, 12, {200, 200, 200, 255});

    DrawText("FIFO: Primeiro a entrar, primeiro a sair",
        startX, blockY + blockH + 65, 13, {150, 150, 150, 255});

    // ========== INSTRUCOES ==========
    DrawText("OPERACOES:", 50, 480, 18, PRIMARY_COLOR);
    DrawText("[I] Enqueue (Inserir) | [R] Dequeue (Remover) | [ESC] Menu",
        50, 515, 14, {200, 200, 200, 255});

    renderStatusBar(ui);
}

void updateHangarModule(UIManager *ui) {
    static int removeMode = 0; // 0: none, 1: enqueue

    if (IsKeyPressed(KEY_I) && removeMode == 0) {
        ui->isInputActive = true;
        removeMode = 1;
        ui->inputLength = 0;
        memset(ui->inputBuffer, 0, sizeof(ui->inputBuffer));
        skipNextInput = true;
    }

    if (IsKeyPressed(KEY_R) && removeMode == 0) {
        // Dequeue imediato
        char *removed = dequeueHangar(ui->hangar);
        if (removed != NULL && strlen(removed) > 0) {
            char msg[256];
            sprintf(msg, "Nave decolada: %s", removed);
            addStatusMessage(ui, msg, 1.5f, WARNING_COLOR);
            free(removed);
        } else {
            addStatusMessage(ui, "Fila vazia!", 1.5f, ERROR_COLOR);
        }
    }

    if (ui->isInputActive) {
        if (!skipNextInput) {
            handleInput(ui);
        } else {
            skipNextInput = false;
        }

        if (IsKeyPressed(KEY_ENTER)) {
            if (ui->inputLength > 0) {
                if (enqueueHangar(ui->hangar, ui->inputBuffer)) {
                    char msg[256];
                    sprintf(msg, "Nave '%s' enfileirada!", ui->inputBuffer);
                    addStatusMessage(ui, msg, 1.5f, SUCCESS_COLOR);
                } else {
                    addStatusMessage(ui, "Erro ao enfileirar nave!", 1.5f, ERROR_COLOR);
                }
            }
            ui->isInputActive = false;
            removeMode = 0;
            ui->inputLength = 0;
            memset(ui->inputBuffer, 0, sizeof(ui->inputBuffer));
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            ui->isInputActive = false;
            removeMode = 0;
            ui->inputLength = 0;
            memset(ui->inputBuffer, 0, sizeof(ui->inputBuffer));
        }
    }
}

// ========== FUNÇÕES PRINCIPAIS ==========

void updateUI(UIManager *ui) {
    // ESC só funciona nos módulos para voltar ao menu
    // No menu principal, ESC é ignorado (só fechar com X)
    if (ui->currentState != STATE_MENU && IsKeyPressed(KEY_ESCAPE)) {
        ui->currentState = STATE_MENU;
        ui->selectedMenu = 0;
        return;
    }

    switch (ui->currentState) {
        case STATE_MENU:
            updateMainMenu(ui);
            break;
        case STATE_SENSORES:
            updateSensoresModule(ui);
            break;
        case STATE_CARGAS:
            updateCargasModule(ui);
            break;
        case STATE_NAVES:
            updateNavesModule(ui);
            break;
        case STATE_MONITORAMENTO:
            updateMonitoramentoModule(ui);
            break;
        case STATE_LOGS:
            updateLogsModule(ui);
            break;
        case STATE_HANGAR:
            updateHangarModule(ui);
            break;
    }
}

void renderUI(UIManager *ui) {
    switch (ui->currentState) {
        case STATE_MENU:
            renderMainMenu(ui);
            break;
        case STATE_SENSORES:
            if (ui->isInputActive) {
                renderSensoresModule(ui);
                renderInputBox(ui, "Digite o nome do sensor:");
            } else {
                renderSensoresModule(ui);
            }
            break;
        case STATE_CARGAS:
            if (ui->isInputActive) {
                renderCargasModule(ui);
                renderInputBox(ui, "Digite o nome da carga:");
            } else {
                renderCargasModule(ui);
            }
            break;
        case STATE_NAVES:
            if (ui->isInputActive) {
                renderNavesModule(ui);
                renderInputBox(ui, "Digite o nome da nave:");
            } else {
                renderNavesModule(ui);
            }
            break;
        case STATE_MONITORAMENTO:
            if (ui->isInputActive) {
                renderMonitoramentoModule(ui);
                renderInputBox(ui, "Digite o nome da camera:");
            } else {
                renderMonitoramentoModule(ui);
            }
            break;
        case STATE_LOGS:
            if (ui->isInputActive) {
                renderLogsModule(ui);
                renderInputBox(ui, "Digite o evento do log:");
            } else {
                renderLogsModule(ui);
            }
            break;
        case STATE_HANGAR:
            if (ui->isInputActive) {
                renderHangarModule(ui);
                renderInputBox(ui, "Digite o nome da nave para pouso:");
            } else {
                renderHangarModule(ui);
            }
            break;
    }
}