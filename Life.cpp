#include "Life.h"
#include <vector>
#include <iostream>
#include <ctime> // для рандомного заполнения
#include <chrono> // для milliseconds (указание времени) 
#include <thread> // для sleep_for (задержка)
#include <cstdlib> // очистка экрана

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
            if(life[i][j]){
                std::cout << "* ";
            }
            else{
                std::cout << "  ";
            }
        }
        std::cout << "\n";
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

void Life::generateLife(){
    randomFill();
    
    while (true){
        print();
        std::this_thread::sleep_for(std::chrono::milliseconds(speed));
        system( "cls" );
        nextGen();
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