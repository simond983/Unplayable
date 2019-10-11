#include "..\src\AI.h"
AI::AI()
{

}


AI::AI(SDL_Renderer* renderer,float xPos,float yPos, int noOfPlayers)
{
	//Set up Sprite component and add to entity component vector
	oldY = 0;
	m_spriteComponent = new SpriteComponent(0, 0, 67, 150);
	m_spriteComponent->loadFromFile("assets/bodyTall.png", renderer);
	m_spriteComponent->setPosition(c2v{ xPos, yPos });
	m_spriteComponent->setScale(c2v{ 0.5f, 0.5f });
	m_spriteComponent->setColor(255, 255, 0);
	this->addComponent(m_spriteComponent);

	m_spriteComponentHead = new SpriteComponent(0, 0, 330, 330);
	m_spriteComponentCrown = new SpriteComponent(0, 0, 404, 255);
	m_spriteComponentHead2 = new SpriteComponent(0, 0, 330, 330);
	m_spriteComponentHead3 = new SpriteComponent(0, 0, 330, 330);
	m_spriteComponentHead4 = new SpriteComponent(0, 0, 330, 330);
	m_spriteComponentHeadNorm = new SpriteComponent(0, 0, 330, 330);
	m_spriteComponentHead5 = new SpriteComponent(0, 0, 330, 330);
	m_spriteComponentHead6 = new SpriteComponent(0, 0, 330, 330);

	m_spriteComponentHead->loadFromFile("assets/art/character/finished_character_assets/PlayerHead.png", renderer);
	m_spriteComponentHead2->loadFromFile("assets/art/character/finished_character_assets/PlayerHeadNinja.png", renderer);
	m_spriteComponentHead3->loadFromFile("assets/art/character/finished_character_assets/PlayerHeadRambo.png", renderer);
	m_spriteComponentHead4->loadFromFile("assets/art/character/finished_character_assets/PlayerHeadArmy.png", renderer);
	m_spriteComponentHeadNorm->loadFromFile("assets/art/character/finished_character_assets/PlayerHead.png", renderer);
	m_spriteComponentHead5->loadFromFile("assets/art/character/finished_character_assets/PlayerHeadHillbilly.png", renderer);
	m_spriteComponentHead6->loadFromFile("assets/art/character/finished_character_assets/PistolHead.png", renderer);



	//m_spriteComponent->loadFromFile("assets/art/character/BlueBody.png", renderer);
	m_spriteComponentCrown->loadFromFile("assets/art/character/finished_character_assets/crown.png", renderer);

	m_spriteComponentHead->setPosition(c2v{ xPos, yPos });
	m_spriteComponentHead->setScale(c2v{ 0.7f, 0.7f });

	m_spriteComponentCrown->setPosition(c2v{ xPos, yPos });
	m_spriteComponentCrown->setScale(c2v{ 0.2f, 0.2f });

	m_spriteComponentHead2->setPosition(c2v{ xPos, yPos });
	m_spriteComponentHead2->setScale(c2v{ 0.7f, 0.7f });

	m_spriteComponentHead3->setPosition(c2v{ xPos, yPos });
	m_spriteComponentHead3->setScale(c2v{ 0.7f, 0.7f });

	m_spriteComponentHead4->setPosition(c2v{ xPos, yPos });
	m_spriteComponentHead4->setScale(c2v{ 0.7f, 0.7f });

	m_spriteComponentHead5->setPosition(c2v{ xPos, yPos });
	m_spriteComponentHead5->setScale(c2v{ 0.7f, 0.7f });

	m_spriteComponentHeadNorm->setPosition(c2v{ xPos, yPos });
	m_spriteComponentHeadNorm->setScale(c2v{ 0.7f, 0.7f });


	m_spriteComponentLeftFoot = new SpriteComponent(0, 0, 107, 91);
	m_spriteComponentLeftFoot->loadFromFile("assets/Foot.png", renderer);
	m_spriteComponentLeftFoot->setPosition(c2v{ xPos, yPos });
	m_spriteComponentLeftFoot->setScale(c2v{ 0.5f, 0.5f });

	m_spriteComponentRightFoot = new SpriteComponent(0, 0, 107, 91);
	m_spriteComponentRightFoot->loadFromFile("assets/Foot.png", renderer);
	m_spriteComponentRightFoot->setPosition(c2v{ xPos, yPos });
	m_spriteComponentRightFoot->setScale(c2v{ 0.5f, 0.5f });

	m_spriteComponentHead->setColor(255, 255, 255);
	m_spriteComponentCrown->setColor(255, 255, 255);
	m_spriteComponentHead2->setColor(255, 255, 255);
	m_spriteComponentHead3->setColor(255, 255, 255);
	m_spriteComponentHead4->setColor(255, 255, 255);
	m_spriteComponentHead5->setColor(255, 255, 255);
	m_spriteComponentHeadNorm->setColor(255, 255, 255);

	control = new ControlComponent();
	this->addComponent(control);

	//creating and adding ray cast for AI
	m_rayCastComp = new RayCastComponent();
	this->addComponent(m_rayCastComp);


	this->addComponent(new FState());
	this->addComponent(new HealthComponent(10));
	tag = new TagComponent("Player");
	if (noOfPlayers == 0)
	{
		tag->setSubTag("Player 1");
		tag->setSubTag2("AI_Player");
		
		
	}
	else if (noOfPlayers == 1)
	{
		tag->setSubTag("Player 2");
		tag->setSubTag2("AI_Player");
		m_spriteComponent->loadFromFile("assets/art/character/BlueBody.png", renderer);
		
	}
	else if (noOfPlayers == 2)
	{
		tag->setSubTag("Player 3");
		tag->setSubTag2("AI_Player");
		m_spriteComponent->loadFromFile("assets/art/character/YellowBody.png", renderer);
		
	}
	else if (noOfPlayers == 3)
	{
		tag->setSubTag("Player 4");
		tag->setSubTag2("AI_Player");
		m_spriteComponent->loadFromFile("assets/art/character/PinkBody.png", renderer);
		
	}

	this->addComponent(tag);
	controlComp = new AIComponent();
	this->addComponent(controlComp);
	positionComp = new PositionComponent(xPos, yPos);
	this->addComponent(positionComp);
	this->addComponent(new CollisionComponent(xPos, yPos, m_spriteComponent->getWidth(), m_spriteComponent->getHeight()));
}
void AI::render(SDL_Renderer* renderer, Camera* camera) {

	if (!control->getAlive()) {

		if (!startDeath) {
			randNum = (rand() % 2) + 1;
			startDeath = true;
		}
		

		if (control->getHitFrom() == "right")
		{
			if (randNum == 1) {
				m_spriteComponentLeftFoot->setPosition(c2v{ positionComp->getX() + runCount - offSet,positionComp->getY() + 52 + offSet });
				m_spriteComponentRightFoot->setPosition(c2v{ positionComp->getX() - runCount + offSet,positionComp->getY() + 52 + offSet });
				m_spriteComponentHead->setPosition(c2v{ positionComp->getX() + headCount / 2 - offSet,positionComp->getY() - 50 + headCount - offSet });
				m_spriteComponentCrown->setPosition(c2v{ m_spriteComponentCrown->getPosition().x + offSet, m_spriteComponentCrown->getPosition().y - offSet });

				m_spriteComponentHead->rotate(3);
				m_spriteComponentCrown->rotate(3);
				m_spriteComponentLeftFoot->rotate(3);
				m_spriteComponentRightFoot->rotate(3);
			}
			else {
				m_spriteComponentHead->setPosition(c2v{ positionComp->getX() + headCount / 2 - offSet,positionComp->getY() - 50 + headCount - offSet });

				m_spriteComponentLeftFoot->setPosition(c2v{ positionComp->getX() + runCount,positionComp->getY() + 52 });
				m_spriteComponentRightFoot->setPosition(c2v{ positionComp->getX() - runCount,positionComp->getY() + 52 });
				m_spriteComponentCrown->setPosition(c2v{ m_spriteComponentCrown->getPosition().x, m_spriteComponentCrown->getPosition().y });
			}
			
		}
		else if (control->getHitFrom() == "left")
		{
			if (randNum == 1) {
				m_spriteComponentLeftFoot->setPosition(c2v{ positionComp->getX() + runCount + offSet,positionComp->getY() + 52 - offSet });
				m_spriteComponentRightFoot->setPosition(c2v{ positionComp->getX() - runCount - offSet,positionComp->getY() + 52 - offSet });
				m_spriteComponentHead->setPosition(c2v{ positionComp->getX() + headCount / 2 + offSet,positionComp->getY() - 50 + headCount + offSet });
				m_spriteComponentCrown->setPosition(c2v{ m_spriteComponentCrown->getPosition().x - offSet, m_spriteComponentCrown->getPosition().y - offSet });

				m_spriteComponentHead->rotate(-3);
				m_spriteComponentCrown->rotate(-3);
				m_spriteComponentLeftFoot->rotate(-3);
				m_spriteComponentRightFoot->rotate(-3);
			}
			else {
				m_spriteComponentHead->setPosition(c2v{ positionComp->getX() + headCount / 2 + offSet,positionComp->getY() - 50 + headCount + offSet });

				m_spriteComponentLeftFoot->setPosition(c2v{ positionComp->getX() + runCount,positionComp->getY() + 52 });
				m_spriteComponentRightFoot->setPosition(c2v{ positionComp->getX() - runCount,positionComp->getY() + 52 });
				m_spriteComponentCrown->setPosition(c2v{ m_spriteComponentCrown->getPosition().x, m_spriteComponentCrown->getPosition().y });

			}

			
		}
		else
		{
			if (randNum == 1) {
				m_spriteComponentLeftFoot->setPosition(c2v{ positionComp->getX() + runCount - offSet,positionComp->getY() + 52 - offSet });
				m_spriteComponentRightFoot->setPosition(c2v{ positionComp->getX() - runCount - offSet,positionComp->getY() + 52 - offSet });
				m_spriteComponentHead->setPosition(c2v{ positionComp->getX() + headCount / 2 + offSet,positionComp->getY() - 50 + headCount + offSet });
				m_spriteComponentCrown->setPosition(c2v{ m_spriteComponentCrown->getPosition().x + offSet, m_spriteComponentCrown->getPosition().y - offSet });


				m_spriteComponentHead->rotate(3);
				m_spriteComponentCrown->rotate(3);
				m_spriteComponentLeftFoot->rotate(3);
				m_spriteComponentRightFoot->rotate(3);
			}
			else {
				m_spriteComponentHead->setPosition(c2v{ positionComp->getX() + headCount / 2 + offSet,positionComp->getY() - 50 + headCount + offSet });

				m_spriteComponentLeftFoot->setPosition(c2v{ positionComp->getX() + runCount,positionComp->getY() + 52 });
				m_spriteComponentRightFoot->setPosition(c2v{ positionComp->getX() - runCount,positionComp->getY() + 52 });
				m_spriteComponentCrown->setPosition(c2v{ m_spriteComponentCrown->getPosition().x, m_spriteComponentCrown->getPosition().y });

			}
			
		}
		offSet += 15;
		controlComp->setRight(false);
		controlComp->setLeft(false);

	}
	else if (control->getAlive())
	{
		offSet = 1;
		// Animation loop for head when hitting ground
		if (animateHead == true)
		{
			if (totalHeadTime < 20)
			{
				totalHeadTime = totalHeadTime + 1;
				if (headCount > 10)
				{
					animateHeadUp = false;
				}
				else if (headCount < -10)
				{
					animateHeadUp = true;
				}
				if (animateHeadUp == true)
				{
					headCount = headCount + 1;
				}
				else if (animateHeadUp == false)
				{
					headCount = headCount - 1;

				}
			}
			else {
				headCount = 0;
				animateHead = false;
				animateHeadUp = true;
			}
		}
		if (controlComp->getJump())
		{
			rumbleCount = 0;
		}
		newY = positionComp->getVelY();
		if (newY != oldY)
		{
			falling = true;
			oldY = newY;
			if (rumbleCount < .99) {
				rumbleCount += 0.01;
			}
		}
		else {
			if (falling)
			{

				animateHead = true;
				totalHeadTime = 0;
				rumbleCount = 0;
			}
			falling = false;


		}
		m_spriteComponentHead->m_flipValue = m_spriteComponent->m_flipValue;
		m_spriteComponentLeftFoot->m_flipValue = m_spriteComponent->m_flipValue;
		m_spriteComponentRightFoot->m_flipValue = m_spriteComponent->m_flipValue;
		if (m_spriteComponentHead->m_flipValue == SDL_FLIP_NONE)
		{
			if (tag->getGunGot() == "shotgun")
			{
				m_spriteComponentHead->setPosition(c2v{ positionComp->getX() - 40 + headCount / 2,positionComp->getY() - 90 + headCount });
			}
			else {
				m_spriteComponentHead->setPosition(c2v{ positionComp->getX() - 10 + headCount / 2,positionComp->getY() - 60 + headCount });
			}
			m_spriteComponentCrown->setPosition(c2v{ positionComp->getX() - 45 + headCount / 2,positionComp->getY() - 100 + headCount });
			m_spriteComponentCrown->setRotation(-headCount - 20);
			m_spriteComponentHead->setRotation(-headCount);


			m_spriteComponentLeftFoot->setPosition(c2v{ positionComp->getX() + runCount,positionComp->getY() + 52 });
			m_spriteComponentLeftFoot->setRotation(-runCount);

			m_spriteComponentRightFoot->setPosition(c2v{ positionComp->getX() - runCount,positionComp->getY() + 52 });
			m_spriteComponentRightFoot->setRotation(runCount);
		}
		else {
			if (tag->getGunGot() == "shotgun")
			{
				m_spriteComponentHead->setPosition(c2v{ positionComp->getX() - 50 + headCount / 2,positionComp->getY() - 90 + headCount });
			}
			else {
				m_spriteComponentHead->setPosition(c2v{ positionComp->getX() - 20 - headCount / 2,positionComp->getY() - 60 + headCount });
			}
			m_spriteComponentCrown->setPosition(c2v{ positionComp->getX() + 15 - headCount / 2,positionComp->getY() - 100 + headCount });
			m_spriteComponentCrown->setRotation(headCount + 20);
			m_spriteComponentHead->setRotation(headCount);

			m_spriteComponentLeftFoot->setPosition(c2v{ positionComp->getX() - 20 + runCount,positionComp->getY() + 52 });  // (Position - player offset + animationCount)
			m_spriteComponentLeftFoot->setRotation(-runCount);

			m_spriteComponentRightFoot->setPosition(c2v{ positionComp->getX() - 20 - runCount,positionComp->getY() + 52 });
			m_spriteComponentRightFoot->setRotation(runCount);
		}

		// Garbage make head go red.
		if (controlComp->getFire())
		{
			m_spriteComponentHead->setColor(255, 255 - firingCount, 255 - firingCount);
			if (firingCount < 255)
			{
				firingCount = firingCount + 1;
			}
		}
		else {
			m_spriteComponentHead->setColor(255, 255, 255);
			firingCount = 0;
		}

		// Checks if guy is moving on y but not x
		//if (positionComp->getVelY() >= 1
		//|| positionComp->getVelY() <= -1 ) 
		if (falling == true) {
			if (fallCount > 20)
			{
				fallingBool = false;
			}
			else if (fallCount < -20)
			{
				fallingBool = true;
			}
			if (fallingBool == true)
			{
				fallCount = fallCount + 3;
			}
			else if (fallingBool == false)
			{
				fallCount = fallCount - 3;

			}
			if (m_spriteComponentHead->m_flipValue == SDL_FLIP_NONE)
			{
				m_spriteComponentLeftFoot->setRotation(45 + fallCount);
				m_spriteComponentRightFoot->setRotation(45 - fallCount);
			}
			else {
				m_spriteComponentLeftFoot->setRotation(-45 + fallCount);
				m_spriteComponentRightFoot->setRotation(-45 - fallCount);
			}
		}
		// If checks if guy is moving on x but not y
		else if (positionComp->getVelX() >= 0.1 && positionComp->getVelY() <= 1 && positionComp->getVelY() >= -1
			|| positionComp->getVelX() <= -0.1 && positionComp->getVelY() <= 1 && positionComp->getVelY() >= -1)
		{
			// Controls what direction the feet move
			if (runCount > 30)
			{
				animationBool = false;
			}
			else if (runCount < -30)
			{
				animationBool = true;
			}
			if (animationBool == true)
			{
				float speed = positionComp->getVelX();
				speed = speed / 2;
				if (speed < 0)
				{
					speed = speed * -1;  // make sure is possitive so dont rip legs apart :).
				}
				runCount = runCount + speed;
			}
			else if (animationBool == false)
			{
				float speed = positionComp->getVelX();
				speed = speed / 2;
				if (speed < 0)
				{
					speed = speed * -1;  // make sure is possitive so dont rip legs apart :).
				}
				runCount = runCount - speed;

			}
			//std::cout << "RUN = " << runCount << std::endl;
			m_spriteComponentLeftFoot->setRotation(-runCount);
			m_spriteComponentRightFoot->setRotation(runCount);
		}
		// Guy is not moving at all.
		else {
			runCount = 0;
		}
	}
	
	if (tag->getGunGot() == "stabbyboy")
	{
		m_spriteComponentHead->setTexture(m_spriteComponentHead2->getTexture(), 104, 110);
	}
	else if (tag->getGunGot() == "juicer")
	{
		m_spriteComponentHead->setTexture(m_spriteComponentHead3->getTexture(), 104, 110);
	}
	else if (tag->getGunGot() == "grenade")
	{
		m_spriteComponentHead->setTexture(m_spriteComponentHead4->getTexture(), 110, 110);
	}
	else if (tag->getGunGot() == "shotgun") {
		m_spriteComponentHead->setTexture(m_spriteComponentHead5->getTexture(), 180, 140);
	}
	else if (tag->getGunGot() == "pistol") {
		m_spriteComponentHead->setTexture(m_spriteComponentHead6->getTexture(), 100, 100);
	}
	else if (tag->getGunGot() == "none") {
		m_spriteComponentHead->setTexture(m_spriteComponentHeadNorm->getTexture(), 100, 100);
	}
	c2v* screenPos = new c2v{ m_spriteComponentHead->getPosition().x - camera->getCamera()->x, m_spriteComponentHead->getPosition().y - camera->getCamera()->y };
	m_spriteComponentHead->setPosition(*screenPos);
	m_spriteComponentHead->render(renderer);
	

	screenPos->x = static_cast<float>(m_spriteComponentLeftFoot->getPosition().x - camera->getCamera()->x);
	screenPos->y = static_cast<float>(m_spriteComponentLeftFoot->getPosition().y - camera->getCamera()->y);
	m_spriteComponentLeftFoot->setPosition(*screenPos);
	m_spriteComponentLeftFoot->render(renderer);

	screenPos->x = static_cast<float>(m_spriteComponentRightFoot->getPosition().x - camera->getCamera()->x);
	screenPos->y = static_cast<float>(m_spriteComponentRightFoot->getPosition().y - camera->getCamera()->y);
	m_spriteComponentRightFoot->setPosition(*screenPos);
	m_spriteComponentRightFoot->render(renderer);

	delete screenPos;



	//SDL_RenderDrawLine(renderer, m_rayCastComp->getStartPosition().x, m_rayCastComp->getStartPosition().y, m_rayCastComp->getCastPosition().x, m_rayCastComp->getCastPosition().y);

}