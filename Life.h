#ifndef LIFE_H
#define LIFE_H

#include <vector>

class Life {
private:
    std::vector<std::vector<bool>> life;
    const int fieldSize = 800;

    int cellsInRow;
    int cellSize;       

    bool isPaused = true;
    bool drawingMode = false;
    const int panelWidth = 220; //ширина панели управления
    const int speedLevels[5] = {1, 5, 10, 20, 30};
    int currentSpeedIndex = 2; //начальная скорость 10 фпс

    //для выпадающего списка
    bool isDropdownOpen = false;
    const char* sizeOptions[5] = {"25x25", "40x40","50x50", "80x80", "100x100"};
    const int availableSizes[5] = {25, 40, 50, 80, 100};
    int selectedSize = 2; // по умолчанию 50x50

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
    void drawDropdown();
    void updateGridSize();
    
public:
    Life();
    ~Life() = default; // деструктор пустой, так как использую vector и он освобождает память сам

    void generateLife();
};

#endif