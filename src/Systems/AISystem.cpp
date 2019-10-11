#include "AISystem.h"

AISystem::AISystem() {
	fsm = new Animation();
	std::cout << fsm->getCurrent() << std::endl;

}

void AISystem::addEntity(Entity * e) {
	m_entities.push_back(e);
}


void AISystem::removeEntity(int index)
{
	m_entities.pop_back();
}


void AISystem::recieveLevel(std::vector<std::pair<c2v, std::string>> walkpoints, std::vector<std::pair<c2v, std::string>> jumpPoints, std::vector<std::shared_ptr<Tile>> tiles, int width, int height)
{
	m_pathPoints = walkpoints;
	m_jumpPoints = jumpPoints;
	m_width = width;
	m_height = height;
	m_tiles = tiles;
}


void AISystem::receive(std::vector<Entity*> guns, std::vector<Entity*> players)
{
	
	c2v vec = { 0.0f, 0.0f };
	
	for (Entity * entity : m_entities) {
		
		AIComponent * ac = (AIComponent*)entity->getCompByType("AI");
		TagComponent * tag = (TagComponent*)entity->getCompByType("TAG");
		
		if (!tag->gotGunBool)
		{
			int count = 0;
			ac->m_distances.assign(guns.size(), std::make_pair(0.0, vec));
			m_dist = ac->m_distances;

			for (auto e = guns.begin(); e != guns.end(); ++e)
			{
				PositionComponent  * pos = (PositionComponent*)(*e)->getCompByType("POSITION");
				ControlComponent * con = (ControlComponent*)(*e)->getCompByType("CONTROL");
				TagComponent * tC = (TagComponent*)entity->getCompByType("TAG");


				if (tC->getGrabbed() == false) {
					m_position = c2v{ pos->getX(), pos->getY() };

					ac->m_distances[count].first = distance(ac->curPosition, m_position);

					vec.x = pos->getX();
					vec.y = pos->getY();


					ac->m_distances[count].second = vec;
				}

				count++;
			}
		}
		else
		{
			int count = 0;
			ac->m_distances.assign(players.size(), std::make_pair(0.0, vec));
			m_dist = ac->m_distances;

			for (auto e = players.begin(); e != players.end(); ++e)
			{
				PositionComponent  * pos = (PositionComponent*)(*e)->getCompByType("POSITION");
				ControlComponent * con = (ControlComponent*)(*e)->getCompByType("CONTROL");
			
				if (con->getAlive()) {
					m_position = c2v{ pos->getX(), pos->getY() };

					ac->m_distances[count].first = distance(ac->curPosition, m_position);

					vec.x = pos->getX();
					vec.y = pos->getY();

					ac->m_distances[count].second = vec;


					count++;
				}
			}
		}
	}
}


std::pair<double,c2v> AISystem::checkClosest(std::vector<std::pair<double, c2v>> distances, std::pair<double, c2v > real)
{

	double smallest = 10000;

	for (auto it = distances.begin(); it != distances.end(); it++)
	{
		if (it->first != 0)
		{
			if (it->first < smallest) {
				smallest = it->first;
				real = (*it);
			}
		}
	}
	
	return real;
}


std::pair<c2v, std::string> AISystem::checkPoints(std::vector<std::pair<c2v, std::string>> points, PositionComponent* pc)
{

	double smallest = 100000;

	c2v myPos = { pc->getX(), pc->getY() };
	c2v closestPosition = c2v{ 0,0 };
	std::string name = "";


	for (int i = 0; i < points.size(); i++)
	{ 

		c2v pos = { points[i].first.x, points[i].first.y };
		double dist = distance(myPos, pos);

		if (dist < smallest)
		{
			smallest = dist;
			closestPosition = pos;
			name = points[i].second;
		}
	}
	
	return std::make_pair(closestPosition, name);
}


void AISystem::update() {

	
	int speed = 0;
	int x = 0;
	int y = 0;

	for (Entity * entity : m_entities) {

		PositionComponent * pc = (PositionComponent*)entity->getCompByType("POSITION");
		SpriteComponent * sc = (SpriteComponent*)entity->getCompByType("SPRITE");
		AIComponent * ac = (AIComponent*)entity->getCompByType("AI");
		FState *state = (FState*)entity->getCompByType("STATE");
		CollisionComponent *coll = (CollisionComponent*)entity->getCompByType("COLLISION");
		TagComponent *tag = (TagComponent*)entity->getCompByType("TAG");
		ControlComponent * con = (ControlComponent*)entity->getCompByType("CONTROL");
		RayCastComponent * rayCast = (RayCastComponent*)entity->getCompByType("Ray");


		//allows the ai to know its own current position
		ac->curPosition.x = pc->getX();
		ac->curPosition.y = pc->getY();

		if (con->getAlive()) {

			ac->newYVel = pc->getVelY();

			//checks if the ai has landed
			if (ac->newYVel != ac->oldYVel)
			{
				ac->m_landed = false;
				ac->oldYVel = ac->newYVel;
			}
			else
			{
				ac->m_landed = true;
			}
			 
			// boundary detection on the right and the left of the screen 
			checkBoundaries(ac);
			
			//sets the initial direction in which the AI will move
			setStartingDirection(ac);

			//sets the closest target position
			ac->closestEnemy = checkClosest(ac->m_distances, ac->m_realDist);

			//continuely sets the position of the closest target 
			if (ac->closestEnemy.second.x > ac->curPosition.x)
			{
				ac->direction = "RIGHT";
			}
			else
			{
				ac->direction = "LEFT";
			}

			
			if (ac->m_landed) {

				calculateMovePoints(ac);
				//sets the closest walk and double jump points
				ac->closestWalkPoint = checkPoints(ac->curWalkPoints, pc);
				ac->closestJumpPoint = checkPoints(ac->curJumpPoints, pc);

			}

			//if the gun isnt in the players current line of sight execute 
			if (!ac->m_gunInSight)
			{
				if (ac->m_landed)
				{
					ac->lastPosition = ac->curPosition;

					checkWalkPoints(ac, pc);
					checkJumpPoints(ac, pc);
					
				}
			}
		
			if (!tag->getGotGunBool()) {
				if (ac->curPosition.y + 20 < ac->closestEnemy.second.y + 200 && ac->m_landed)
				{
					ac->jumping = false;
				}
				else
				{
					ac->jumping = true;
				}
			}
			else
			{
				if (ac->curPosition.y <= ac->closestEnemy.second.y)
				{
					ac->jumping = false;
				}
				else
				{
					ac->jumping = true;
				}
			}
			
			
			

			//ai shooting entities
			if (tag->gotGunBool)
			{
				rayCast->setStartPosition(ac->curPosition.x, ac->curPosition.y);
				rayCast->setCastPosition(ac->closestEnemy.second.x, ac->closestEnemy.second.y);

				ac->detect = tileCollision(rayCast->getStartPosition().x, rayCast->getStartPosition().y, rayCast->getCastPosition().x, rayCast->getCastPosition().y);

				double desired = getAngleToPlayer(ac->curPosition, ac->closestEnemy);

				con->setAngle(desired);

				if (con->getCurrentAngle() > desired - 5 && con->getCurrentAngle() < desired + 5)
				{
					con->setFire(true);
				}
				else
				{
					con->setFire(false);
				}

				if (!ac->detect && ac->initRecieve) 
				{
					if (tag->getGunGot() != "stabbyboy") {
						ac->setLeft(false);
						ac->setRight(false);
					}
					

				}
				else if (ac->detect)
				{
					ac->closestEnemy = checkClosest(ac->m_distances, ac->m_realDist);
				}

				
				
				
				if (!ac->initRecieve)
				{
					ac->initRecieve = true;
				}
			}	
			
			//if the gun is on the same level as the AI character
			if (ac->curPosition.y + 50 > ac->closestEnemy.second.y && ac->curPosition.y + 50 < ac->closestEnemy.second.y + 200 && ac->m_landed)
			{
				ac->m_gunInSight = true;

				if (ac->direction == "LEFT")
				{

					if (ac->curPosition.x > ac->closestEnemy.second.x)
					{
						ac->setRight(false);
						ac->setLeft(true);
					}
				}

				if (ac->direction == "RIGHT" )
				{
					if (ac->curPosition.x < ac->closestEnemy.second.x)
					{
						ac->setRight(true);
						ac->setLeft(false);
					}
				}

			}
			else
			{
				ac->m_gunInSight = false;
			}
			
		}
		else
		{
			con->setFire(false);
		
		}
	}
}


double AISystem::getAngleToPlayer(c2v pos , std::pair<double, c2v> enemy)
{
	c2v dir = c2Sub(pos, enemy.second);
	
	double angle;
	
	angle = atan2(-dir.y, dir.x);

	if ((angle * 180 / 3.14159) <= 90 && (angle * 180 / 3.14159) >= -90)
	{
		return (angle * 180 / 3.14159) - 90;
	}
	else {
		double fixAngle = (angle * 180 / 3.14159) + 270;
		if (fixAngle > 360)  // Math :)
		{
			return fixAngle - 360;
		}
		else
		{
			return fixAngle;
		}
	}

}


double AISystem::distance(c2v  vecOne, c2v vecTwo)
{
	return std::sqrt((vecOne.x - vecTwo.x) * (vecOne.x - vecTwo.x) + (vecOne.y - vecTwo.y) * (vecOne.y - vecTwo.y));
}


void AISystem::calculateMovePoints(AIComponent * ac)
{
	//clears the vectors
	ac->curWalkPoints.clear();
	ac->curJumpPoints.clear();

	//checks if the node positions are on the players current level
	for (int i = 0; i < m_pathPoints.size(); i++)
	{
		if (m_pathPoints[i].first.y > ac->curPosition.y && m_pathPoints[i].first.y < ac->curPosition.y + 100)
		{
			ac->curWalkPoints.push_back(m_pathPoints[i]);
		}
	}

	for (int i = 0; i < m_jumpPoints.size(); i++)
	{
		if (m_jumpPoints[i].first.y > ac->curPosition.y && m_jumpPoints[i].first.y < ac->curPosition.y + 100)
		{
			ac->curJumpPoints.push_back(m_jumpPoints[i]);
		}
	}

}


void AISystem::checkBoundaries(AIComponent * ac)
{
	if (ac->curPosition.x >= m_width - 50)
	{
		ac->setLeft(true);
		ac->setRight(false);
	}
	if (ac->curPosition.x <= 0)
	{
		ac->setLeft(false);
		ac->setRight(true);
	}
}


void AISystem::setStartingDirection(AIComponent * ac)
{
	if (!ac->set)
	{
		ac->closestEnemy = checkClosest(ac->m_distances, ac->m_realDist);

		if (ac->closestEnemy.second.x > ac->curPosition.x)
		{
			ac->setLeft(true);
		}
		else
		{
			ac->setRight(true);
		}

		ac->set = true;
	}
}


void AISystem::checkWalkPoints(AIComponent * ac, PositionComponent * pc)
{
	if (ac->curPosition.x < ac->closestWalkPoint.first.x + 5 && ac->curPosition.x > ac->closestWalkPoint.first.x - 5)
	{
		if (ac->closestWalkPoint.second == "LEFT" && ac->jumping)
		{
			ac->setLeft(true);
			ac->setRight(false);

			if (pc->getVelX() < -7.8)
			{
				ac->setJump(true);
			}
			else
			{
				pc->setVelX(-8);
				ac->setJump(true);
			}

		}

		if (ac->closestWalkPoint.second == "RIGHT" && ac->jumping)
		{
			ac->setLeft(false);
			ac->setRight(true);

			if (pc->getVelX() > 7.8)
			{
				ac->setJump(true);
			}
			else
			{
				pc->setVelX(8);
				ac->setJump(true);
			}
		}
	}
}


void AISystem::checkJumpPoints(AIComponent * ac, PositionComponent * pc)
{
	if (ac->curPosition.x < ac->closestJumpPoint.first.x + 5 && ac->curPosition.x > ac->closestJumpPoint.first.x - 5)
	{
		if (ac->closestJumpPoint.second == "DOUBLERIGHT" && ac->jumping)
		{
			ac->setLeft(false);
			ac->setRight(true);

			if (pc->getVelX() > 7.8)
			{
				ac->setDoubleJump(true);

			}
			else
			{
				pc->setVelX(8);
				ac->setDoubleJump(true);

			}
		}

		if (ac->closestJumpPoint.second == "DOUBLELEFT" && ac->jumping)
		{
			ac->setLeft(true);
			ac->setRight(false);

			if (pc->getVelX() < -7.8)
			{
				ac->setDoubleJump(true);
			}
			else
			{
				pc->setVelX(-8);
				ac->setDoubleJump(true);
			}
		}
	}
}


bool AISystem::tileCollision(float x1, float y1, float x2, float y2)
{

	for (int i = 0; i < m_tiles.size(); i++) {

	
		float x = m_tiles.at(i)->position.x;
		float y = m_tiles.at(i)->position.y;
		float w = m_tiles.at(i)->width;
		float h = m_tiles.at(i)->height;

		bool left = lineLine(x1, y1, x2, y2, x, y, x, y + h);
		bool right = lineLine(x1, y1, x2, y2, x + w, y, x + w, y + h);
		bool top = lineLine(x1, y1, x2, y2, x, y, x + w, y);
		bool bottom = lineLine(x1, y1, x2, y2, x, y + h, x + w, y + h);

		// if ANY of the above are true, the line
		// has hit the rectangle
		if (left || right || top || bottom) {
			return true;
		}	
	}
	return false;
}


bool AISystem::lineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {

	// calculate the direction of the lines
	float uA = ((x4 - x3)*(y1 - y3) - (y4 - y3)*(x1 - x3)) / ((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1));
	float uB = ((x2 - x1)*(y1 - y3) - (y2 - y1)*(x1 - x3)) / ((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1));

	// if uA and uB are between 0-1, lines are colliding
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {

		return true;
	}
	return false;
}