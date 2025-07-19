#ifndef LIFE_H
#define LIFE_H

#include <vector>

class Life {
private:
    std::vector<std::vector<bool>> life;
    const int fieldSize = 800;

    int cellsInRow = 40;
    int cellSize;       

    const int panelWidth = 220; //ширина панели управления
    bool isPaused = true;
    const int speedLevels[5] = {1, 5, 10, 20, 30};
    int currentSpeedIndex = 2; //начальная скорость 10 fps
    bool drawingMode = false;

    //для выпадающего списка
    bool isDropdownOpen = false;
    Rectangle dropdownRect;
    const char* sizeOptions[5] = {"8x8", "10x10", "20x20", "30x30", "40x40"};
    const int availableSizes[5] = {8, 10, 20, 30, 40}; // Соответствующие значения
    int selectedSize = 4; // Индекс для 40x40 (по умолчанию)

    int getNeighbours(int x, int y);
    void print();
    void randomFill();
    void nextGen();
    void clear();
    void generateButton();
    void clearButton();
    void information();
    void draw();
    void drawButton();
    void drawButtonClick();
    void handleDropdown();
    void drawDropdown();
    void updateGridSize();
    
public:
    Life();
    ~Life() = default; // деструктор пустой, так как использую vector и он освобождает память сам

    void generateLife();
};

#endif