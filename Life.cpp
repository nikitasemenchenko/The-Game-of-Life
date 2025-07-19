#include "Life.h"
#include <vector>
#include <string>
#include <iostream>
#include <ctime> // для рандомного заполнения
#include <chrono> // для milliseconds (указание времени) 
#include <thread> // для sleep_for (задержка)
#include <cstdlib> // очистка экрана

#include <raylib.h> // библиотека для отрисовки

Life::Life() {
    cellsInRow = availableSizes[selectedSize];
    cellSize = fieldSize / cellsInRow;
    
    std::srand(static_cast<unsigned>(std::time(nullptr))); 

    // Создаем сетку заданного размера
    life.resize(cellsInRow);
    for(int i = 0; i < cellsInRow; ++i) {
        life[i].resize(cellsInRow, false); // Инициализируем все клетки как false
    }
}

void Life::print(){
    for(int i = 0; i < cellsInRow; ++i){
        for(int j = 0; j < cellsInRow; ++j){
            Color color = life[i][j] ? GREEN : BLACK; // живая клетка - зелёная, мёртвая - чёрная

            Rectangle cell = { static_cast<float>(j * cellSize + panelWidth), static_cast<float>(i * cellSize),
                static_cast<float>(cellSize - 1), static_cast<float>(cellSize - 1) }; // -1 чтобы было видно сетку

            DrawRectangleRec(cell, color); // рисуем клетку
        }
    }
}

int Life::getNeighbours(int x, int y) {
    int c = 0;
    for (int i = x-1; i <= x+1; ++i) {
        for (int j = y-1; j <= y+1; ++j) {
            if (i == x && j == y) continue;  // пропускаем саму клетку
            if (i >= 0 && i < cellsInRow && 
                j >= 0 && j < cellsInRow && 
                life[i][j]) {
                c++;
            }
        }
    }
    return c;
}

void Life::randomFill(){
    srand(time(0));
    for(int i = 0; i < cellsInRow; ++i){
        for(int j = 0; j < cellsInRow; ++j){
            life[i][j] = rand() % 2;
        }
    }
}

void Life::clear() {
    for(int i = 0; i < cellsInRow; ++i){
        for(int j = 0; j < cellsInRow; ++j){
            life[i][j] = false;
        }
    }
} 
void Life::generateButton() {
    Rectangle generateBtn = {20, 220, 180, 40};
    bool isInGenerateBtnArea = CheckCollisionPointRec(GetMousePosition(), generateBtn);
    DrawRectangleRec(generateBtn, isInGenerateBtnArea ? LIGHTGRAY : GRAY); 

    //центрирование GENERATE
    const char* generateText = "GENERATE";
    int textWidth = MeasureText(generateText, 20);
    int textX = generateBtn.x + (generateBtn.width - textWidth) / 2;
    int textY = generateBtn.y + (generateBtn.height - 20) / 2;
    DrawText(generateText, textX, textY, 20, BLACK);

    //обработка клика
    if (isInGenerateBtnArea && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isPaused) {
        randomFill();
    }
}
void Life::clearButton() {
    Rectangle clearBtn = {20, 270, 180, 40};
        bool isInClearBtnArea = CheckCollisionPointRec(GetMousePosition(), clearBtn);
        DrawRectangleRec(clearBtn, isInClearBtnArea ? LIGHTGRAY : GRAY);
        //центрирование названия кнопки
        const char* clearText = "CLEAR";
        int textWidth = MeasureText(clearText, 20);
        int textX = clearBtn.x + (clearBtn.width - textWidth) / 2;
        int textY = clearBtn.y + (clearBtn.height - 20) / 2;
        DrawText(clearText, textX, textY, 20, BLACK);
        //обработка клика
        if (isInClearBtnArea && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isPaused) {
            clear(); // очищаем поле
        }
}

void Life::information(){
    DrawText("CONTROLS", 20, 20, 20, WHITE);
    DrawText("SPACE: Pause/Resume", 20, 50, 15, WHITE);
    DrawText("UP/DOWN: Change speed", 20, 80, 15, WHITE);      
    DrawText(TextFormat("Speed: %d GPS", speedLevels[currentSpeedIndex]), 20, 120, 20, WHITE);
    DrawText(isPaused ? "[PAUSED]" : "[RUNNING]", 20, 150, 20, isPaused ? RED : GREEN);
}

void Life::draw() {
    if (!drawingMode || !isPaused) return;
    Vector2 mousePos = GetMousePosition();
    int posX = (mousePos.x - panelWidth) / cellSize;
    int posY = mousePos.y / cellSize;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isPaused && posX >= 0 && posX < cellsInRow && posY >= 0 && posY < cellsInRow) {
        life[posY][posX] = !life[posY][posX];
    }
}

void Life::drawButtonClick() {
    drawingMode = !drawingMode;
}

void Life::drawButton() {
    Rectangle drawButton = {20, 320, 180, 40};
    bool isInDrawBtnArea = CheckCollisionPointRec(GetMousePosition(), drawButton);
    Color drawButtonColor;
    if( drawingMode ) {
        drawButtonColor = GREEN;
    } else {
        drawButtonColor = isInDrawBtnArea? LIGHTGRAY : GRAY;
    }
    DrawRectangleRec(drawButton, drawButtonColor);
    //центрирование названия кнопки
    const char* text = "DRAW";
    int textWidth = MeasureText(text, 20);
    int textX = drawButton.x + (drawButton.width - textWidth) / 2;
    int textY = drawButton.y + (drawButton.height - 20) / 2;
    DrawText(text, textX, textY, 20, BLACK);

    if (isInDrawBtnArea && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isPaused) {
        drawButtonClick();
    }
}

void Life::nextGen() {
    std::vector<std::vector<bool>> newLife(cellsInRow, std::vector<bool> (cellsInRow,false));
    for (int i = 0; i < cellsInRow; ++i){
        for (int j = 0; j < cellsInRow; ++j){
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
    life.swap(newLife); // обновляем состояние клеток
}

void Life::generateLife() {  
    InitWindow(800 + panelWidth, 800, "The Game of Life");
    float simulationTimer = 0.0f;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime(); //замер времени между кадрами

        //обновление поля
        if (!isPaused) {
            simulationTimer += deltaTime;
            float updateInterval = 1.0f / speedLevels[currentSpeedIndex];
            while (simulationTimer >= updateInterval) {
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
        
        // Панель управления
        DrawRectangle(0, 0, panelWidth, 800, DARKGRAY);
        
        // Игровое поле фиксированный размер 800x800
        DrawRectangle(panelWidth, 0, 800, 800, DARKGRAY);
        
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
        drawDropdown();  

        if (drawingMode) draw();
        
        EndDrawing();
    }
    CloseWindow();
}

void Life::drawDropdown() {
    Rectangle dropdownMenu = {20, 370, 180, 30};
    bool isInDropdownArea = CheckCollisionPointRec(GetMousePosition(), dropdownMenu);
    DrawRectangleRec(dropdownMenu, (isInDropdownArea || isDropdownOpen)? LIGHTGRAY : GRAY);
    // центрируем
    int textWidth = MeasureText(sizeOptions[selectedSize], 20);
    int textX = dropdownMenu.x + (dropdownMenu.width - textWidth) / 2;
    int textY = dropdownMenu.y + (dropdownMenu.height - 20) / 2;
    DrawText(sizeOptions[selectedSize], textX, textY, 20, BLACK);

    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, dropdownMenu)) {
            isDropdownOpen = !isDropdownOpen;
        }
    }

    if(isDropdownOpen){
        for (int i = 0; i < 5; i++) {
            Rectangle option = {dropdownMenu.x, dropdownMenu.y + dropdownMenu.height * (i + 1), dropdownMenu.width, dropdownMenu.height};
            bool isInOptionArea = CheckCollisionPointRec(GetMousePosition(), option);
            DrawRectangleRec(option, isInOptionArea ? LIGHTGRAY : GRAY);
            //тут тоже центрировать надо
            // к сожалению в цикле, хз пока что как по другому можно сделать
            int textWidth = MeasureText(sizeOptions[i], 20);
            int textX = option.x + (option.width - textWidth) / 2;
            int textY = option.y + (option.height - 20) / 2;
            DrawText(sizeOptions[i], textX, textY, 20, BLACK);

            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isInOptionArea && isPaused) {
                selectedSize = i;
                cellsInRow = availableSizes[selectedSize];
                updateGridSize();
                isDropdownOpen = false;
            }
        }
    }
}

void Life::updateGridSize() {
    cellSize = fieldSize / cellsInRow;
    life.resize(cellsInRow);
    for (auto& row : life) {
        row.resize(cellsInRow, false);
    }
    clear();
}