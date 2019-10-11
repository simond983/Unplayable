#include "CollisionSystem.h"

CollisionSystem::CollisionSystem() {

}

void CollisionSystem::addEntity(Entity * e) {
	m_entities.push_back(e);
}



std::string CollisionSystem::rectCollision(c2AABB A, c2AABB B)
{

	float dx = (A.min.x + (A.max.x - A.min.x) / 2) - (B.min.x + (B.max.x - B.min.x) / 2);
	float dy = (A.min.y + (A.max.y - A.min.y) / 2) - (B.min.y + (B.max.y - B.min.y) / 2);
	float width = ((A.max.x - A.min.x) + (B.max.x - B.min.x)) / 2;
	float height = ((A.max.y - A.min.y) + (B.max.y - B.min.y)) / 2;
	float crossWidth = width * dy;
	float crossHeight = height * dx;
	std::string collision = "none";

	if (dx < 0) {
		dx = -dx;
	}
	if (dy < 0) {
		dy = -dy;
	}
	//
	if (dx <= width && dy <= height) {
		if (crossWidth > crossHeight) {
			collision = (crossWidth > (-crossHeight)) ? "bottom" : "left";
		}
		else {
			collision = (crossWidth > (-crossHeight)) ? "right" : "top";
		}
	}
	
	return(collision);
}

void CollisionSystem::update(std::vector<std::shared_ptr<Tile>> tiles, Camera* camera) {
	for (Entity * entity : m_entities) {
		
		TagComponent * tag = (TagComponent*)entity->getCompByType("TAG");

		if (tag->getTag() == "Player") {
			PositionComponent * pc = (PositionComponent*)entity->getCompByType("POSITION");
			CollisionComponent * cc = (CollisionComponent*)entity->getCompByType("COLLISION");

			cc->SetCollPos(pc->getX(), pc->getY());

			for (int i = 0; i < tiles.size(); i++) {
				
					std::string val;
					if (tiles.at(i)->dRect.x >= 0) {
						val = rectCollision(cc->getCollider(), tiles.at(i)->collider);
						if (val != "none") {
							if (val == "top") {
								pc->jumpNum = 0;
								pc->m_allowedJump = true;
								pc->setVelY(0);
								pc->m_onTop = true;
								pc->setY(tiles.at(i)->dRect.y - cc->getH());


							}

							else if (val == "bottom") {
								pc->setVelY(5);
								
							}

							else if (val == "left") {


								pc->setX(tiles.at(i)->dRect.x - cc->getW());

								pc->m_allowedJump = true;
								m_count = 0;

								pc->m_hitLeftSide = true;
							}

							else if (val == "right")
							{
								pc->setX(tiles.at(i)->dRect.x + cc->getW() + 35);
								pc->m_allowedJump = true;
								pc->m_hitRightSide = true;

							}
						}
									
					}
			}
		}
		else if (tag->getTag() == "Gun") {
			PositionComponent * pc = (PositionComponent*)entity->getCompByType("POSITION");
			CollisionComponent * cc = (CollisionComponent*)entity->getCompByType("COLLISION");

			cc->SetCollPos(pc->getX(), pc->getY());

			for (int i = 0; i < tiles.size(); i++) {
				
					std::string val;
					if (tiles.at(i)->dRect.x >= 0) {
						val = rectCollision(cc->getCollider(), tiles.at(i)->collider);
						if (val != "none") {
							if (val == "top") {
								pc->setVelY(0);
								pc->setY(tiles.at(i)->dRect.y - cc->getH());
								

							}
							else if (val == "bottom") {
								pc->setVelY(5);
								
							}
							else if (val == "right" || val == "left") {
								pc->setVelX(-(pc->getVelX()));
							}


						}

					}



				
			}

			checkBullets(pc, tiles, camera);

		}
		
	}
}

void CollisionSystem::checkBullets(PositionComponent * poc, std::vector<std::shared_ptr<Tile>> tiles, Camera* camera) {

	std::vector<Bullet *> * bullets = &poc->bullets;
	
	for (Entity * entity : m_entities) {
		TagComponent * tag = (TagComponent*)entity->getCompByType("TAG");

		if (tag->getTag() == "Player") {
			CollisionComponent * cc = (CollisionComponent*)entity->getCompByType("COLLISION");
			PositionComponent * pc = (PositionComponent*)entity->getCompByType("POSITION");
			SpriteComponent * sc = (SpriteComponent*)entity->getCompByType("SPRITE");
			ControlComponent * control = (ControlComponent*)entity->getCompByType("CONTROL");

			if (pc->getY() > 3000 && tag->getTag() == "Player") {
				ControlComponent * control = (ControlComponent*)entity->getCompByType("CONTROL");
				control->setAlive(false);
			}
			for (int i = 0; i < bullets->size(); i++) {
				std::string val = rectCollision(cc->getCollider(), bullets->at(i)->collider);
				c2v bPos = bullets->at(i)->m_spriteComponent->getPosition();
				if (bPos.x > 2000 || bPos.x < -200 || bPos.y < -200 || bPos.y > 3000) {
					Bullet * temp = bullets->at(i);
					bullets->erase(bullets->begin() + i);
					delete temp;
				}
				if (val != "none" && control->getAlive()) {
					Bullet * temp = bullets->at(i);
					bullets->erase(bullets->begin() + i);
					delete temp;
					

					if (tag->getTag() == "Player") {
						PositionComponent * pc = (PositionComponent*)entity->getCompByType("POSITION");

						if (val == "right") {
							control->setAlive(false);
							control->setHitFrom("right");
							if (!control->isDead)
							{
								c2v * screenPos = new c2v{ 0,0 };
								auto particle = new ParticleExample();
								screenPos->x = pc->getX() - camera->getCamera()->x;
								screenPos->y = pc->getY() - camera->getCamera()->y;
								particle->setRenderer(m_renderer);
								particle->setStyle(ParticleExample::BLOOD);
								particle->setPosition(screenPos->x, screenPos->y);
								m_blood.push_back(particle);
								control->isDead = true;
							}
						}
						if (val == "left") {
							control->setAlive(false);
							control->setHitFrom("left");
							if (!control->isDead)
							{
								c2v * screenPos = new c2v{ 0,0 };
								auto particle = new ParticleExample();
								screenPos->x = pc->getX() - camera->getCamera()->x;
								screenPos->y = pc->getY() - camera->getCamera()->y;
								particle->setRenderer(m_renderer);
								particle->setStyle(ParticleExample::BLOOD);
								particle->setPosition(screenPos->x, screenPos->y);
								m_blood.push_back(particle);
								control->isDead = true;
							}
						}
					}
				}
			}
		}
	}


	
	for (int i= 0; i < tiles.size(); i++) {
		std::string val;
		if (tiles.at(i)->dRect.x >= 0) {
			for (int j = 0; j < bullets->size(); j++) {
				val = rectCollision(bullets->at(j)->collider, tiles.at(i)->collider);
				if (val != "none") {
					tiles.at(i)->health--;
					if (tiles.at(i)->health <= 0) {
						tiles.at(i)->dead = true;
					}
					c2v screenPos = c2v{ 0,0 };
					auto particle = new ParticleExample();
					particle->setRenderer(m_renderer);
					particle->setStyle(ParticleExample::SMOKE);
					particle->setPosition(bullets->at(j)->m_spriteComponent->getPosition().x, bullets->at(j)->m_spriteComponent->getPosition().y);
					m_particles.push_back(particle);
					Bullet * temp = bullets->at(j);
					bullets->erase(bullets->begin() + j);
					delete temp;
				}
			}
		}
	}
}	


void CollisionSystem::setRenderer(SDL_Renderer * renderer)
{
	m_renderer = renderer;
}

void CollisionSystem::animateExplosion()
{

	for (int i = 0; i < m_particles.size(); i++)
	{
		m_particles[i]->count++;


		m_particles[i]->setStartSpin(0);
		m_particles[i]->setStartSpinVar(0);
		m_particles[i]->setEndSpin(90);
		m_particles[i]->setDuration(.5);
		m_particles[i]->setStartSize(50);
		m_particles[i]->setEndSize(50);
		m_particles[i]->setStartSpinVar(0);


		m_particles[i]->update();
		m_particles[i]->draw();
		
		if (m_particles[i]->count > 5)
		{
			ParticleExample * temp = m_particles.at(i);
			m_particles.erase(m_particles.begin() + i);
			delete temp;
			m_particles.resize(m_particles.size());
		}

	}	
}


void CollisionSystem::animateBlood()
{

	for (int i = 0; i < m_blood.size(); i++)
	{
		m_blood[i]->count++;

		m_blood[i]->setStartSpin(90);
		m_blood[i]->setEndSpin(90);
		m_blood[i]->setDuration(.2);
		m_blood[i]->setStartSize(15);
		m_blood[i]->setStartSpinVar(180);// set the renderer
		m_blood[i]->setSpeed(100);
		m_blood[i]->setSpeedVar(100);

		m_blood[i]->update();
		m_blood[i]->draw();

		if (m_blood[i]->count > 15)
		{
			ParticleExample * temp = m_blood.at(i);
			m_blood.erase(m_blood.begin() + i);
			delete temp;
			m_blood.resize(m_blood.size());
		}

	}
}


void CollisionSystem::render()
{
	animateExplosion();
	animateBlood();
}