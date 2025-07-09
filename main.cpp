#include "Life.h"
#include <chrono> //для milliseconds (указание времени)
#include <thread> //для sleep_for (задержка)


void clearScreen() {
    std::cout << "\033[H";  // курсор в левый верхний угол
}

int main() {
    Life game;
    game.randomFill();

    while (true) {
        clearScreen(); 
        game.print();  
        game.nextGen();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    return 0;
}