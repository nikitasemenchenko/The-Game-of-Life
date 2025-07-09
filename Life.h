#ifndef LIFE_H
#define LIFE_H

#include <vector>

class Life {
private:
    std::vector<std::vector<bool>> life; //само пространство с клетками
    //размеры поля
    const int X = 25; 
    const int Y = 25;
    //скорость игры в миллисекундах
    const int speed = 500;

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