#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"

struct SDL_Texture;
class Collider;
enum EnemyType;

class Enemy : public Entity
{
public:
	// Constructor
	Enemy(int x, int y, EnemyType enemyType, Entity* playerPointer);

	// Destructor
	virtual ~Enemy();

	// Called each loop iteration
	virtual bool Update(float dt);

	// Blit
	virtual bool Draw();

	// Collision response
	void OnCollision(Collider* c1, Collider* c2);

public:
	// Path related variables
	DynArray<iPoint> path;
	int pathCount = 0;

protected:
	Entity* player;

	// General enemy size
	int enemySize;

	//Pathfinding related variables
	iPoint pastDest;
	int i;
	int counterTile;

	bool attackChange = false;
	bool hurtChange = false;
};

#endif // __ENEMY_H__