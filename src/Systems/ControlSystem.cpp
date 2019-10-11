#include "ControlSystem.h"

ControlSystem::ControlSystem() {
	//init();
}

void ControlSystem::recieveLevel(int width, int height)
{
	m_width = width;
	m_height = height;
}

void ControlSystem::removeEntity(int index)
{
	m_entities.erase(m_entities.begin() + index);
}

SDL_Haptic* ControlSystem::init(SDL_GameController* controller) {
	//Initialization flag
	bool success = true;

	
	
	
	//Load joystick
		
	SDL_Joystick *j = SDL_GameControllerGetJoystick(controller);
	haptic = SDL_HapticOpenFromJoystick(j);
	if (controller == NULL)
	{
		printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
	}

	if (haptic == NULL)
	{
		printf("Warning: Controller does not support haptics! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		//Get initialize rumble
		if (SDL_HapticRumbleInit(haptic) < 0)
		{
			printf("Warning: Unable to initialize rumble! SDL Error: %s\n", SDL_GetError());
		}
	}
	return haptic;
	
}

void ControlSystem::addEntity(Entity * e) {
	m_entities.push_back(e);
}

void ControlSystem::update(SDL_Event e) {

	

	for (Entity * entity : m_entities) {

		double joystickAngle = 0;

		TagComponent * tc = (TagComponent*)entity->getCompByType("TAG");
		ControlComponent * cc = (ControlComponent*)entity->getCompByType("CONTROL");
		PositionComponent * pc = (PositionComponent*)entity->getCompByType("POSITION");

		if (tc->getTag() == "Player") {
			if (!cc->m_init) {
				cc->setHaptic(init(cc->gGameController));
				cc->m_init = true;
			}
		}
		
		int StickX = SDL_GameControllerGetAxis(cc->gGameController, SDL_CONTROLLER_AXIS_LEFTX);
		int StickY = SDL_GameControllerGetAxis(cc->gGameController, SDL_CONTROLLER_AXIS_LEFTY);

		int leftX = SDL_GameControllerGetAxis(cc->gGameController, SDL_CONTROLLER_AXIS_RIGHTX);
		int leftY = SDL_GameControllerGetAxis(cc->gGameController, SDL_CONTROLLER_AXIS_RIGHTY);

		setButtons(cc);

		if (StickX < -JOYSTICK_DEAD_ZONE) {
			cc->setLeft(true);
		}
		else {
			cc->setLeft(false);
		}

		

		if (StickX > JOYSTICK_DEAD_ZONE) {

			cc->setRight(true);

		}
		else {
			cc->setRight(false);
		}
		
		
		
		if (leftX > JOYSTICK_DEAD_ZONE || leftX < -JOYSTICK_DEAD_ZONE ||
			leftY > JOYSTICK_DEAD_ZONE || leftY < -JOYSTICK_DEAD_ZONE) {
			joystickAngle = atan2((double)leftX, (double)leftY) * (180.0 / M_PI);
		}
		
		if (joystickAngle != 0)
		{
			cc->setAngle(joystickAngle);
		}

		//std::cout << joystickAngle << std::endl;
	}
}

void ControlSystem::setButtons(ControlComponent * cc) {

	bool AButton = SDL_GameControllerGetButton(cc->gGameController, SDL_CONTROLLER_BUTTON_A);
	bool XButton = SDL_GameControllerGetButton(cc->gGameController, SDL_CONTROLLER_BUTTON_X);
	

	//int leftX = SDL_GameControllerGetAxis(gGameController, SDL_CONTROLLER_AXIS_RIGHTX);
	int RT = SDL_GameControllerGetAxis(cc->gGameController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
	
	if (RT > 5000) {
		cc->setFire(true);
		
	}
	else {
		cc->setFire(false);
	}

	
	

	if (AButton) {
		if (cc->aIndex == 0)
			cc->setJump(AButton);
		
		cc->aIndex++;
	}
	else {
		cc->aIndex = 0;
		cc->setJump(false);
	}
	if (XButton) {
		if (cc->xIndex == 0)
			cc->setThrowWeapon(true);
		cc->xIndex++;
	}
	else {
		cc->setThrowWeapon(false);
		cc->xIndex = 0;
	}
}

