#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include"Registry.h"

class Game {
public:
	Game();

	Registry* GetRegistry() { return &registry; }
private:
	Registry registry;
};

#endif