#ifndef LIFE_H
#define LIFE_H

#include <vector>

class Life {
private:
    std::vector<std::vector<bool>> life; //само пространство с клетками
    //размеры поля
    const int width = 800; 
    const int height = 800;

    const int cellSize = 20; //размер клетки
    const int X = width / cellSize; //количество клеток по горизонтали
    const int Y = height / cellSize; //количество клеток по вертикали

    const int panelWidth = 220; //ширина панели управления
    bool isPaused = true;
    const int speedLevels[5] = {1, 5, 10, 20, 30};
    int currentSpeedIndex = 2; //начальная скорость 10 fps
    bool drawingMode = false;

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
public:
    Life();
    ~Life() = default; // деструктор пустой, так как использую vector и он освобождает память сам

    void generateLife();
};

#endif