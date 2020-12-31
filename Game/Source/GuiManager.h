#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "Module.h"
#include "GuiControl.h"

#include "List.h"

class GuiManager : public Module
{
public:

	GuiManager();

	// Destructor
	virtual ~GuiManager();

	// Called when program is executed
	void Init();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before quitting
	bool CleanUp();

	// Additional methods
	GuiControl* CreateGuiControl(GuiControlType type, uint32 id, SDL_Rect bounds, const char* text, Module* observer, int widthInUnits = 0);
	void DestroyGuiControl(GuiControl* entity);

	//void AddGuiControl(GuiControl* entity);

public:
	SString folderTexture;
	SDL_Texture* atlas;
	int defaultFont = -1;
	int titleFont = -1;
	int titleFontMedium = -1;
	int titleFontSmall = -1;
	int hoverFont = -1;
	int pressedFont = -1;
	int disabledFont = -1;

	List<GuiControl*> controls;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;
};

#endif // __GUIMANAGER_H__
