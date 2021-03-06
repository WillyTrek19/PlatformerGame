#ifndef __COLLISIONS_H__
#define __COLLISIONS_H__

#include "Module.h"
#include "SDL.h"

#define MAX_COLLIDERS 250

class Collider
{
public:
	// Collision Type enum
	enum Type
	{
		NONE = -1,
		AIR,
		SOLID,
		SPIKE,
		PLAYER,
		FRUIT,
		COIN,
		CHECKPOINT,
		GOAL,
		ENEMY,
		ATTACK,
		MAX
	};

	// Constructor
	Collider(SDL_Rect rect, Type type, Module* listener = nullptr) : rect(rect), type(type), listener(listener)
	{}

	// Sets the position of the construct
	void SetPos(int x, int y, int w, int h);

	// Checks if two rects are intersecting
	bool Intersects(const SDL_Rect& r) const;

	SDL_Rect rect;
	bool pendingToDelete = false;
	Type type;
	Module* listener = nullptr;
};

class Collisions : public Module
{
public:
	// Constructor
	Collisions();

	// Called when program is executed
	void Init();

	// Destructor
	virtual ~Collisions();

	// Called before player is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Save/Load
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);

	// Adds a new collider to the list
	Collider* AddCollider(SDL_Rect rect, Collider::Type type, Module* listener = nullptr);

private:
	// All existing colliders in the scene
	Collider* colliders[MAX_COLLIDERS] = { nullptr };

	// The collision matrix. Defines the interaction for two collider types
	// If set to false, collider 1 will ignore collider 2
	bool matrix[Collider::Type::MAX][Collider::Type::MAX];
};

#endif // !__COLLISIONS_H__