#include "Life.h"
#include <vector>
#include <iostream>
#include <ctime>

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
    print();
}

void Life::nextGen() {
    std::vector<std::vector<bool>> newLife(Y, std::vector<bool> (X,false));
    for (int i = 0; i < Y; ++i){
        for (int j = 0; j < X; ++j){
            int neighbs = getNeighbours(i,j);
            
            if (life[i][j]){
                //живые
                newLife[i][j] = (neighbs == 2 || neighbs == 3);
            } else {
                //мёртвые
                newLife[i][j] = (neighbs == 3);
            }
        }
    }
    life = newLife;
}