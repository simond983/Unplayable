#pragma once

#include <SDL.h>
#include <SDL_image.h>

enum class GameState
{
	None,
	Splash,
	Menu,
	Options,
	Online,
	Game,
	Credits,
	End
};

class State
{
public:

	State() {}
	~State() {}
private:
	
};