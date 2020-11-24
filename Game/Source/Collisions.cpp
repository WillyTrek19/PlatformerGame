#include "Collisions.h"

#include "App.h"

#include "Defs.h"
#include "Log.h"

Collisions::Collisions()
{
	name.Create("collisions");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		colliders[i] = nullptr;
	}

	matrix[Collider::Type::SOLID][Collider::Type::NONE] = false;
	matrix[Collider::Type::SOLID][Collider::Type::SOLID] = false;
	matrix[Collider::Type::SOLID][Collider::Type::SPIKE] = false;
	matrix[Collider::Type::SOLID][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::SOLID][Collider::Type::FRUIT] = false;
	matrix[Collider::Type::SOLID][Collider::Type::SPAWN] = false;
	matrix[Collider::Type::SOLID][Collider::Type::GOAL] = false;
	matrix[Collider::Type::SOLID][Collider::Type::ENEMY] = false;

	matrix[Collider::Type::SPIKE][Collider::Type::NONE] = false;
	matrix[Collider::Type::SPIKE][Collider::Type::SOLID] = false;
	matrix[Collider::Type::SPIKE][Collider::Type::SPIKE] = false;
	matrix[Collider::Type::SPIKE][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::SPIKE][Collider::Type::FRUIT] = false;
	matrix[Collider::Type::SPIKE][Collider::Type::SPAWN] = false;
	matrix[Collider::Type::SPIKE][Collider::Type::GOAL] = false;
	matrix[Collider::Type::SPIKE][Collider::Type::ENEMY] = false;

	matrix[Collider::Type::PLAYER][Collider::Type::NONE] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::SOLID] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::SPIKE] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::FRUIT] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::SPAWN] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::GOAL] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::ENEMY] = true;

	matrix[Collider::Type::FRUIT][Collider::Type::NONE] = false;
	matrix[Collider::Type::FRUIT][Collider::Type::SOLID] = false;
	matrix[Collider::Type::FRUIT][Collider::Type::SPIKE] = false;
	matrix[Collider::Type::FRUIT][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::FRUIT][Collider::Type::FRUIT] = false;
	matrix[Collider::Type::FRUIT][Collider::Type::SPAWN] = false;
	matrix[Collider::Type::FRUIT][Collider::Type::GOAL] = false;
	matrix[Collider::Type::FRUIT][Collider::Type::ENEMY] = false;

	matrix[Collider::Type::SPAWN][Collider::Type::NONE] = false;
	matrix[Collider::Type::SPAWN][Collider::Type::SOLID] = false;
	matrix[Collider::Type::SPAWN][Collider::Type::SPIKE] = false;
	matrix[Collider::Type::SPAWN][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::SPAWN][Collider::Type::FRUIT] = false;
	matrix[Collider::Type::SPAWN][Collider::Type::SPAWN] = false;
	matrix[Collider::Type::SPAWN][Collider::Type::GOAL] = false;
	matrix[Collider::Type::SPAWN][Collider::Type::ENEMY] = false;

	matrix[Collider::Type::GOAL][Collider::Type::NONE] = false;
	matrix[Collider::Type::GOAL][Collider::Type::SOLID] = false;
	matrix[Collider::Type::GOAL][Collider::Type::SPIKE] = false;
	matrix[Collider::Type::GOAL][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::GOAL][Collider::Type::FRUIT] = false;
	matrix[Collider::Type::GOAL][Collider::Type::SPAWN] = false;
	matrix[Collider::Type::GOAL][Collider::Type::GOAL] = false;
	matrix[Collider::Type::GOAL][Collider::Type::ENEMY] = false;

	matrix[Collider::Type::ENEMY][Collider::Type::NONE] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::SOLID] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::SPIKE] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::FRUIT] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::SPAWN] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::GOAL] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::ENEMY] = false;
}

Collisions::~Collisions()
{}

void Collisions::Init()
{

}

bool Collisions::Start()
{
	return true;
}

bool Collisions::Awake(pugi::xml_node&)
{
	return true;
}

bool Collisions::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i) {
		if (colliders[i] != nullptr && colliders[i]->pendingToDelete == true) {
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i) {
		// skip empty colliders
		if (colliders[i] == nullptr) { continue; }

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k) {
			// skip empty colliders
			if (colliders[k] == nullptr) { continue; }

			c2 = colliders[k];

			if (c1->Intersects(c2->rect) && matrix[c1->type][c2->type]) {
				if (c1->listener) { c1->listener->OnCollision(c1, c2); }
				if (c2->listener) { c2->listener->OnCollision(c2, c1); }
			}
		}
	}

	return true;
}

bool Collisions::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i) {
		if (colliders[i] != nullptr) {
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

bool Collisions::Load(pugi::xml_node&)
{
	return true;
}

bool Collisions::Save(pugi::xml_node&)
{
	return true;
}

Collider* Collisions::AddCollider(SDL_Rect rect, Collider::Type type, Module* listener)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i) {
		if (colliders[i] == nullptr) {
			ret = colliders[i] = new Collider(rect, type, listener);
			break;
		}
	}

	return ret;
}

void Collider::SetPos(int x, int y, int w, int h)
{
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
}

bool Collider::Intersects(const SDL_Rect& r) const
{
	return (rect.x < r.x + r.w && rect.x + rect.w > r.x && rect.y < r.y + r.h && rect.h + rect.y > r.y);
}