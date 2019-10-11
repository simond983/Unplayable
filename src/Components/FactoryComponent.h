#ifndef FACTORYCOMPONENT_H
#define FACTORYCOMPONENT_H

#include "Component.h"
#include "../Factory.h"
#include <vector>


class FactoryComponent : public Component
{
public:
	FactoryComponent();

	std::string m_tag = "FACTORY";

	std::string getType();


	Factory* factory = new BulletFactory();
	//std::vector<Entity*> projectiles;
	//void makeBullet(SDL_Renderer* renderer){projectiles.push_back(factory->CreateBullet(SDL_Renderer* renderer));}
	Bullet * makeBullet(SDL_Renderer* renderer, float xPos, float yPos, double angle, double xOffset, double yOffset, int ttl,SDL_Texture * bulletTexture);

private:

};
#endif // !FACTORYCOMPONENT_H
