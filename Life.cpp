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

            Rectangle cell = { static_cast<float>(j * cellSize + panelWidth), static_cast<float>(i * cellSize),
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

void Life::clear() {
    for(int i = 0; i < Y; ++i){
        for(int j = 0; j < X; ++j){
            life[i][j] = false;
        }
    }
} 
void Life::generateButton() {
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
    if (generateHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isPaused) {
        randomFill();
    }
}
void Life::clearButton() {
    Rectangle clearBtn = {20, 270, 180, 40};
        bool clearHover = CheckCollisionPointRec(GetMousePosition(), clearBtn);
        DrawRectangleRec(clearBtn, clearHover ? LIGHTGRAY : GRAY);
        //центрирование названия кнопки
        const char* clearText = "CLEAR";
        int textWidth = MeasureText(clearText, 20);
        int textX = clearBtn.x + (clearBtn.width - textWidth) / 2;
        int textY = clearBtn.y + (clearBtn.height - 20) / 2;
        DrawText(clearText, textX, textY, 20, BLACK);
        //обработка клика
        if (clearHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isPaused) {
            clear(); // очищаем поле
        }
}

void Life::information(){
    DrawText("CONTROLS", 20, 20, 20, WHITE);
    DrawText("SPACE: Pause/Resume", 20, 50, 15, WHITE);
    DrawText("UP/DOWN: Change speed", 20, 80, 15, WHITE);      
    DrawText(TextFormat("Speed: %d FPS", speedLevels[currentSpeedIndex]), 20, 120, 20, WHITE);
    DrawText(isPaused ? "[PAUSED]" : "[RUNNING]", 20, 150, 20, isPaused ? RED : GREEN);
}

void Life::draw() {
    if (!drawingMode || !isPaused) return;
    Vector2 mousePos = GetMousePosition();
    int posX = (mousePos.x - panelWidth) / cellSize;
    int posY = mousePos.y / cellSize;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isPaused && posX >= 0 && posX < X && posY >= 0 && posY < Y) {
        life[posY][posX] = !life[posY][posX];
    }
}

void Life::drawButtonClick() {
    drawingMode = !drawingMode;
}

void Life::drawButton() {
    Rectangle drawButton = {20, 320, 180, 40};
    bool drawHover = CheckCollisionPointRec(GetMousePosition(), drawButton);
    Color buttonColor;
    if(drawingMode) {
        buttonColor = drawHover ? GREEN : LIME;
    } else {
        buttonColor = drawHover ? LIGHTGRAY : GRAY;
    }
    DrawRectangleRec(drawButton, buttonColor);
    //центрирование названия кнопки
    const char* text = "DRAW";
    int textWidth = MeasureText(text, 20);
    int textX = drawButton.x + (drawButton.width - textWidth) / 2;
    int textY = drawButton.y + (drawButton.height - 20) / 2;
    DrawText(text, textX, textY, 20, BLACK);

    if (drawHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isPaused) {
        drawButtonClick();
    }
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


void Life::generateLife() {  
    InitWindow(width + panelWidth, height, "The Game of Life");

    float simulationTimer = 0.0f;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime(); //замер времени между кадрами

        //обновление поля
        if (!isPaused) {
            simulationTimer += deltaTime;
            float updateInterval = 1.0f / speedLevels[currentSpeedIndex];
            while (simulationTimer >= updateInterval) { //накопили нужный временной интервал - обновляем игру
                simulationTimer -= updateInterval;
                nextGen();
            }
        }

        //Горячие клавиши
        if (IsKeyPressed(KEY_UP)) currentSpeedIndex = std::min(4, currentSpeedIndex + 1);
        if (IsKeyPressed(KEY_DOWN)) currentSpeedIndex = std::max(0, currentSpeedIndex - 1);
        if (IsKeyPressed(KEY_SPACE)) isPaused = !isPaused;

        //визуал
        BeginDrawing();

        ClearBackground(GRAY);
        DrawRectangle(0, 0, panelWidth, height, DARKGRAY); //панель управления
        DrawRectangle(panelWidth, 0, width, height, DARKGRAY);  //игровое поле
        
        print(); // отрисовка клеток

        information();
        
        //визуальный выбор скорости
        for (int i = 0; i < 5; i++) { 
            Color col = (i <= currentSpeedIndex) ? GREEN : DARKGREEN;
            DrawRectangle(20 + i*30, 180, 25, 25, col);
            }

        generateButton();
        clearButton();
        drawButton();

        if (drawingMode) draw();

        EndDrawing();
    }
    CloseWindow();
}