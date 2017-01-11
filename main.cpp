#include <iostream>
#include "GenMap.hpp"
#include <chrono>
#include <thread>
#include "Game.hpp"

int main() {
    srand(time(nullptr));
    try {
	Game game;
	std::thread logicThread([&game]() {
	    while (game.IsRunning()) {
		game.UpdateLogic();
	    }
	});
	while (game.IsRunning()) {
	    game.DrawGraphics();
	    game.EventLoop();
	}
	logicThread.join();
    } catch (std::exception & ex) {
	std::cerr << ex.what() << std::endl;
    }
}
