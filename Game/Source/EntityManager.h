#ifndef __MODULE_ENTITY_MANAGER_H__
#define __MODULE_ENTITY_MANAGER_H__

#include "Module.h"

#include "Entity.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"
#include "List.h"

#define MAX_ENTITIES 100

class Entity;

class EntityManager : public Module
{
public:
	// Constructor
	EntityManager();

	// Destructor
	virtual ~EntityManager();

	// Called when program is executed
	void Init();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	// Loads the necessary textures for the entities
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Create/Destroy entity
	Entity* CreateEntity(int x, int y, EntityType type, Entity* playerPointer = nullptr, EnemyType eType = EnemyType::NO_TYPE);
	void DestroyEntity(Entity* entity);

	// Calls Update for every entity
	bool UpdateAll(float dt, bool doLogic);

	// Collision response
	void OnCollision(Collider* c1, Collider* c2);

	// On mouse click response
	bool OnGuiMouseClickEvent(GuiControl* control);

	// Load/Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);

public:
	List<Entity*> entities;

	bool doLogic = false;

	SString folderTexture;
	SString folderAudioFx;
	SString folderMap;

	// Fx
	int enemyDestroyedFx = 0;
	uint deadFx;
	uint jumpFx;
	uint doubleJumpFx;
	uint fruitFx;
	uint hitFx;
	uint slashFx;
	uint checkpointFx;
	uint coinFx;

	// Tex
	SDL_Texture* playerTex;
	SDL_Texture* playerHeart;
	SDL_Texture* ground;
	SDL_Texture* flying;
	SDL_Texture* tileSetTex;

	// GUI
	bool pause;
	iPoint cameraPos;
	iPoint cameraSize;

	GuiButton* btnSettings;
	SString settingsTitle;
	bool settings;
	int offsetSettings;
	GuiSlider* sldrMusic;
	GuiSlider* sldrFx;
	GuiCheckBox* chckFullscreen;
	GuiCheckBox* chckVsync;
	GuiButton* btnBack;

	int pauseFont;
	SString pauseTitle;
	int offsetTitle;
	GuiButton* btnResume;
	GuiButton* btnTitle;
	float dtTmp;
	GuiButton* btnExit;
	bool exitRequest;
};

#endif // __MODULE_ENTITY_MANAGER_H__