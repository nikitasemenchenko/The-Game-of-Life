#include "Life.h"
#include <vector>
#include <iostream>
#include <ctime> // для рандомного заполнения
#include <chrono> // для milliseconds (указание времени) 
#include <thread> // для sleep_for (задержка)
#include <cstdlib> // очистка экрана

#include <raylib.h> // библиотека для отрисовки

Life::Life(){
    life.resize(Y);
    for(int i = 0; i < Y; ++i){
        life[i].resize(X);
    }
    // т.к конструктор, заполняем сначала false
    for(int i = 0; i < Y; ++i){
        for(int j = 0; j < X; ++j){
            life[i][j] = false;
        }
    }
}

void Life::print(){
    for(int i = 0; i < Y; ++i){
        for(int j = 0; j < X; ++j){
            Color color = life[i][j] ? GREEN : BLACK; // живая клетка - зелёная, мёртвая - чёрная

            Rectangle cell = { static_cast<float>(j * cellSize), static_cast<float>(i * cellSize),
                static_cast<float>(cellSize - 1), static_cast<float>(cellSize -1) }; // -1 чтобы было видно сетку

            DrawRectangleRec(cell, color); // рисуем клетку
        }
    }
}

int Life::getNeighbours(int x, int y){
    int c = 0;
    for (int i = x-1; i <= x+1; ++i) {
        for (int j = y-1; j <= y+1; ++j) {
            if (i == x && j == y) continue;
            
            if (i >= 0 && i < Y && 
                j >= 0 && j < X && 
                life[i][j]) {
                c++;
            }
        }
    }
    return c;
}

void Life::randomFill(){
    srand(time(0));
    for(int i = 0; i < Y; ++i){
        for(int j = 0; j < X; ++j){
            life[i][j] = rand() % 2;
        }
    }
}

void Life::generateLife() {
    const int panelWidth = 220; //ширина панели управления
    
    InitWindow(width + panelWidth, height, "The Game of Life");
    SetTargetFPS(60);

    //скорость
    const int speedLevels[5] = {1, 5, 10, 20, 30};
    int currentSpeedIndex = 2; //начальная скорость 10 fps
    float simulationTimer = 0.0f;
    bool isPaused = true; //начало с паузы

    for (auto& row : life) {
        std::fill(row.begin(), row.end(), false); //от начала до конца заполняем "мёртным" значением
    }

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime(); //замер времени между кадрами
        
        //рабочие кнопки
        if (IsKeyPressed(KEY_UP)) currentSpeedIndex = std::min(4, currentSpeedIndex + 1);
        if (IsKeyPressed(KEY_DOWN)) currentSpeedIndex = std::max(0, currentSpeedIndex - 1);
        if (IsKeyPressed(KEY_SPACE)) isPaused = !isPaused;

        //обновление поля
        if (!isPaused) {
            simulationTimer += deltaTime;
            float updateInterval = 1.0f / speedLevels[currentSpeedIndex];
            while (simulationTimer >= updateInterval) { //накопили нужный временной интервал - обновляем игру
                simulationTimer -= updateInterval;
                nextGen();
            }
        }

        //визуал
        BeginDrawing();

        ClearBackground(GRAY);
        DrawRectangle(0, 0, panelWidth, height, DARKGRAY); //панель управления
        DrawRectangle(panelWidth, 0, width, height, BLACK);  //игровое поле
        // print();
        
        //отрисовка клеток с учетом смещения (из-за панели)
        for (int row = 0; row < Y; ++row) {
        for (int col = 0; col < X; ++col) {
        //вычисление координаты клетки
        const float posX = panelWidth + col * cellSize;
        const float posY = row * cellSize;
        
        //клетка с границами
        DrawRectangle(posX, posY, cellSize-1, cellSize-1, life[row][col] ? GREEN : BLACK);
            }
        }

        //текст для навигации
        DrawText("CONTROLS", 20, 20, 20, WHITE);
        DrawText("SPACE: Pause/Resume", 20, 50, 15, WHITE);
        DrawText("UP/DOWN: Change speed", 20, 80, 15, WHITE);
            
        DrawText(TextFormat("Speed: %d FPS", speedLevels[currentSpeedIndex]), 20, 120, 20, WHITE);
        DrawText(isPaused ? "[PAUSED]" : "[RUNNING]", 20, 150, 20, isPaused ? RED : GREEN);
        
        //визуальный выбор скорости
        for (int i = 0; i < 5; i++) { 
            Color col = (i <= currentSpeedIndex) ? GREEN : DARKGREEN;
            DrawRectangle(20 + i*30, 180, 25, 25, col);
            }

        //кнопка Generate
        Rectangle generateBtn = {20, 220, 180, 40};
        bool generateHover = CheckCollisionPointRec(GetMousePosition(), generateBtn);
        DrawRectangleRec(generateBtn, generateHover ? LIGHTGRAY : GRAY); 

            //центрирование GENERATE
        const char* generateText = "GENERATE";
        int textWidth = MeasureText(generateText, 20);
        int textX = generateBtn.x + (generateBtn.width - textWidth) / 2;
        int textY = generateBtn.y + (generateBtn.height - 20) / 2;
        DrawText(generateText, textX, textY, 20, BLACK);

        //обработка клика
        if (generateHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            randomFill();
        }
        EndDrawing();
    }
    CloseWindow();
}

void Life::nextGen() {
    std::vector<std::vector<bool>> newLife(Y, std::vector<bool> (X,false));
    for (int i = 0; i < Y; ++i){
        for (int j = 0; j < X; ++j){
            int neighbs = getNeighbours(i,j);
            
            //Проверка на правила игры
            if (life[i][j]){
                //клетка живет если у неё 2 или 3 соседа
                //или умирает от перенаселения или одиночества
                newLife[i][j] = (neighbs == 2 || neighbs == 3);
            } else {
                //клетка рождается, если у неё ровно 3 соседа
                //иначе остаётся мертвой
                newLife[i][j] = (neighbs == 3);
            }
        }
    }
    life = newLife; // обновляем состояние клеток
}
