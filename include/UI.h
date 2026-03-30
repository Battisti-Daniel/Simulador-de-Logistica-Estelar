//
// Created by danie on 29/03/2026.
// Interface gráfica para o sistema de estação estelar
//

#ifndef TRABALH_M1_UI_H
#define TRABALH_M1_UI_H

#include "raylib.h"
#include "Sensores.h"
#include "Cargas.h"
#include "Naves.h"
#include "Monitoramento.h"
#include "Logs.h"
#include "Hangar.h"
#include <string>
#include <queue>

// Estados da aplicação
enum ApplicationState {
    STATE_MENU,
    STATE_SENSORES,
    STATE_CARGAS,
    STATE_NAVES,
    STATE_MONITORAMENTO,
    STATE_LOGS,
    STATE_HANGAR
};

// Estrutura para armazenar mensagens de status
struct StatusMessage {
    std::string text;
    float duration;
    Color color;
};

// Estrutura principal da UI
struct UIManager {
    ApplicationState currentState;
    ApplicationState previousState;
    int selectedMenu;
    std::queue<StatusMessage> statusQueue;
    float statusDisplayTime;

    // Instâncias dos TADs
    Sensors *sensors;
    Loads *loads;
    Ships *ships;
    Monitoring *monitoring;
    Logs *logs;
    Hangar *hangar;

    // Dados de entrada
    char inputBuffer[50];
    int inputLength;
    bool isInputActive;
};

// Funções principais
UIManager* createUIManager();
void destroyUIManager(UIManager *ui);
void updateUI(UIManager *ui);
void renderUI(UIManager *ui);

// Funções de menu
void renderMainMenu(UIManager *ui);
void updateMainMenu(UIManager *ui);

// Funções de cada módulo
void renderSensoresModule(UIManager *ui);
void updateSensoresModule(UIManager *ui);

void renderCargasModule(UIManager *ui);
void updateCargasModule(UIManager *ui);

void renderNavesModule(UIManager *ui);
void updateNavesModule(UIManager *ui);

void renderMonitoramentoModule(UIManager *ui);
void updateMonitoramentoModule(UIManager *ui);

void renderLogsModule(UIManager *ui);
void updateLogsModule(UIManager *ui);

void renderHangarModule(UIManager *ui);
void updateHangarModule(UIManager *ui);

// Funções auxiliares de UI
void addStatusMessage(UIManager *ui, const std::string &text, float duration, Color color);
void renderStatusBar(UIManager *ui);
void renderInputBox(UIManager *ui, const std::string &prompt);
void handleInput(UIManager *ui);

// Funções de desenho
void drawBox(float x, float y, float width, float height, const std::string &title, Color color, bool selected = false);
void drawCircle(float x, float y, float radius, const std::string &title, Color color, bool selected = false);
void drawStack(float x, float y, float width, float height, const std::string &title, int itemCount, Color color);

#endif //TRABALH_M1_UI_H

