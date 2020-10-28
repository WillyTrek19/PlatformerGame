#include "Player.h"

#include "App.h"

#include "Textures.h"
#include "Input.h"
#include "Audio.h"
#include "Render.h"
#include "Scene.h"
#include "DeathScene.h"
#include "Transition.h"
#include "Map.h"
#include "Collisions.h"

#include "Log.h"

Player::Player() {}

Player::~Player() {}

void Player::Init()
{
	active = false;
}

bool Player::Start()
{
	godMode = true;
	keyPressed = false;
	isDead = false;
	invert = false;

	playerPhysics.axisX = false;
	playerPhysics.axisY = true;

	playerTex = app->tex->Load("Assets/textures/characterSpritesheet.png");
	deadFx = app->audio->LoadFx("Assets/audio/fx/lose.wav");
    return true;
}

bool Player::Awake(pugi::xml_node&)
{
	for (int i = 0; i != 9; ++i)
	{
		idle.PushBack({ 22 + (playerSize * i),1330,44,72 });
	}
	idle.SetSpeed(0.05f);
	idle.SetLoop(true);

	for (int i = 0; i != 8; ++i)
	{
		run.PushBack({ 22 + (playerSize * i),1202,50,75 });
	}
	run.SetSpeed(0.05f);
	run.SetLoop(true);

/*for (int i = 0; i != 7; ++i)
{
	jump.PushBack({ 22 + (playerSize * i),820,49,80 });
}
jump.SetSpeed(0.1f);
jump.SetLoop(false);*/

for (int i = 0; i != 5; ++i)
{
	death.PushBack({ 22 + (playerSize * i),184,76,66 });
}
death.SetSpeed(0.03f);
death.SetLoop(false);

wallJump.PushBack({ 648,170,55,79 });
wallJump.SetSpeed(0.0f);
wallJump.SetLoop(false);

return true;
}

bool Player::PreUpdate()
{
	return true;
}

bool Player::Update(float dt)
{
	currentAnimation->Update();
	keyPressed = false;

	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		currentAnimation = &death;
		isDead = true;
		app->audio->PlayFx(deadFx);
	}

	if (isDead == false)
	{
		if (godMode)
		{
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			{
				playerRect.y -= 1;
				keyPressed = true;
			}
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			{
				playerRect.y += 1;
				keyPressed = true;
			}
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT)
			{
				playerRect.x -= 1;
				currentAnimation = &run;
				if (invert == false)
				{
					invert = true;
				}
				keyPressed = true;
			}
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT)
			{
				playerRect.x += 1;
				currentAnimation = &run;
				if (invert == true)
				{
					invert = false;
				}
				keyPressed = true;
			}
			/*if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				currentAnimation = &jump;
				keyPressed = true;
			}*/
		}

		if (keyPressed == false)
		{
			run.Reset();
			//jump.Reset();
			currentAnimation = &idle;
		}
	}

	//Physics
	playerPhysics.UpdatePhysics(playerRect.x, playerRect.y, speed.x, speed.y);

	//Collisions
	int x = playerRect.x / 64;
	int y = playerRect.y / 64;

	if (y == 11) { // to enter the GetColliderId on its first iteration with the floor thanks to a breakpoint
		LOG("b");
	}
	//Floor
	//if (GetColliderId(x,y + 1) == Collider::TYPE::SOLID
	//	&& (GetColliderId(x - 1,y + 1) == Collider::TYPE::SOLID
	//	|| GetColliderId(x + 1, y + 1) == Collider::TYPE::SOLID)
	//	&& GetColliderId(x,y) == Collider::TYPE::AIR)
	if(GetColliderId(x,y + 1) == Collider::TYPE::SOLID)			// on second iteration it should work but it doesnt
	{
		//reset jump
		playerRect.y = y* 64;
		speed.y = 0;
		//LOG("FLOOR!");
	}
	else
	{
		//LOG("NOT FLOOR!");
	}
	/*
	//Left Wall
	if (GetColliderId(x - 1, y) == Collider::TYPE::SOLID
		&& (GetColliderId(x - 1, y + 1) == Collider::TYPE::SOLID
		|| GetColliderId(x - 1, y - 1) == Collider::TYPE::SOLID)
		&& GetColliderId(x, y) == Collider::TYPE::AIR)
	{
		//reset jump
		//playerRect.x = 2 * 64 * (x + 1) - playerRect.x;
		LOG("LEFT WALL!");
	}
	else
	{
		LOG("NOT LEFT WALL!");
	}

	//Right Wall
	if (GetColliderId(x + 1, y) == Collider::TYPE::SOLID
		&& (GetColliderId(x + 1, y + 1) == Collider::TYPE::SOLID
		|| GetColliderId(x + 1, y - 1) == Collider::TYPE::SOLID)
		&& GetColliderId(x, y) == Collider::TYPE::AIR)
	{
		//reset jump
		//playerRect.x = 2 * 64 * (x + 2) - 64 * 2 - playerRect.x;
		LOG("RIGHT WALL!");
	}
	else
	{
		LOG("NOT RIGHT WALL!");
	}

	//Ceiling
	if (GetColliderId(x, y - 1) == Collider::TYPE::SOLID
		&& (GetColliderId(x - 1, y - 1) == Collider::TYPE::SOLID
		|| GetColliderId(x + 1, y - 1) == Collider::TYPE::SOLID)
		&& GetColliderId(x, y) == Collider::TYPE::AIR)
	{
		speed.y = 0;
		LOG("CEILING!");
	}
	else
	{
		LOG("NOT CEILING!");
	}
	*/

	// Spawns
	//if (GetColliderId(x, y) == Collider::TYPE::SPAWN)
	//{
	//	spawnPoint = { x, y };
	//}

	// Dead
	//if (GetColliderId(x, y) == Collider::TYPE::SPIKE) {
	//	isDead = true;
	//}
	if (isDead)
	{
		if (currentAnimation->HasFinished())
		{
			app->transition->FadeEffect((Module*)app->scene, (Module*)app->deathScene, false, 600.0f);
		}
	}

	// Win condition
	//if (GetColliderId(x, y) == Collider::TYPE::GOAL)
	//{
	//	LOG("YOU WIN, YAY!\n");
	//}

    return true;
}

bool Player::PostUpdate()
{
	if (playerRect.x <= 0)
	{
		playerRect.x = 0;
	}
	if ((playerRect.x + playerRect.w) > (app->map->data.w * app->map->data.tileW))
	{
		--playerRect.x;
	}

	app->render->DrawRectangle({ playerRect.x, playerRect.y + 64, 64, 64 }, 255, 0, 0, 255); // temp
	app->render->DrawTexture(playerTex, playerRect.x, playerRect.y, false, &currentAnimation->GetCurrentFrame(), invert);
    return true;
}

bool Player::CleanUp()
{
	app->tex->UnLoad(playerTex);

	app->audio->UnloadFx(deadFx);

	return true;
}

bool Player::Load(pugi::xml_node&)
{
    return true;
}

bool Player::Save(pugi::xml_node&)
{
    return true;
}

int Player::GetColliderId(int x, int y, bool isFruit) const
{
	int ret;
	// MapLayer		<- this works
	ListItem <MapLayer*>* ML = app->map->data.mapLayer.start;
	SString collisions = "Collisions";
	while (ML != NULL) {
		if (ML->data->name == collisions) {
			break;
		}
		ML = ML->next;
	}

	// Tileset		<- this works
	ListItem <Tileset*>* T = app->map->data.tilesets.start;
	SString name;
	if (isFruit) {
		name = "Level1Tileset(64x64)";
	}
	else {
		name = "MetaData";
	}
	while (T != NULL) {
		if (T->data->name == name) {
			break;
		}
		T = T->next;
	}

	// Gets CollisionId
	int id = (int)(ML->data->Get(x, y) - T->data->firstgId);	//returns id of the tile
	Tile* currentTile = T->data->GetPropList(id);					//on second iteration there is no properties list (we think it gets destroyed)
	ret = currentTile->properties.GetProperty("CollisionId",0);						//since there is no getpropList it triggers breakpoint and explodes
	//LOG("%d - %d", id, ret);
	return ret;
}