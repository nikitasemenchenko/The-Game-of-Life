#ifndef LIFE_H
#define LIFE_H

#include <vector>
#include <iostream>

//размеры поля
const int X = 15; 
const int Y = 15;

class Life {
private:
    std::vector<std::vector<bool>> life; //само пространство с клетками
    int getNeighbours(int x, int y);
    
public:
    Life();
    ~Life() = default; // деструктор пустой, так как использую vector и он освобождает память сам
    
    void print();
    void randomFill();
    void nextGen();
    void generateLife();
};


#endif