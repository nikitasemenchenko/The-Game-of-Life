#ifndef LIFE_H
#define LIFE_H

#include <vector>

class Life {
private:
    std::vector<std::vector<bool>> life; //само пространство с клетками
    //размеры поля
    const int width = 600; 
    const int height = 600;

    const int cellSize = 20; //размер клетки
    const int X = width / cellSize; //количество клеток по горизонтали
    const int Y = height / cellSize; //количество клеток по вертикали

    //скорость игры
    // const int fps = 2;
    int fps;
    bool isPaused;
    float sliderValue;

    int getNeighbours(int x, int y);
    void print();
    void randomFill();
    void nextGen();
    
public:
    Life();
    ~Life() = default; // деструктор пустой, так как использую vector и он освобождает память сам

    void generateLife();
};


#endif