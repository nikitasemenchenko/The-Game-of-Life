#ifndef LIFE_H
#define LIFE_H

#include <vector>
#include <iostream>
#include <ctime>
//размеры поля
const int X = 25; 
const int Y = 25;

class Life {
private:
    std::vector<std::vector<bool>> life; //само пространство с клетками

    void print();
    void randomFill();
    int getNeighbours(int x, int y);
public:
    Life();
    ~Life() = default; // деструктор пустой, так как использую vector и он освобождает память сам

    void generateLife();
};


#endif