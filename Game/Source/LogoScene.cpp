#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "LogoScene.h"
#include "Transition.h"
#include "Defs.h"

#include "Log.h"

LogoScene::LogoScene() : Module()
{
	name.Create("logoScene");
}

LogoScene::~LogoScene()
{}

void LogoScene::Init()
{
	active = true;
}

bool LogoScene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;
	timer = config.child("timer").attribute("value").as_int(0);

	folderTexture.Create(config.child("folderTexture").child_value());
	folderAudioFx.Create(config.child("folderAudioFx").child_value());

	return ret;
}

bool LogoScene::Start()
{
	SString tmp("%s%s", folderTexture.GetString(), "logo_screen.jpg");
	logo = app->tex->Load(tmp.GetString());
	tmp.Clear();
	tmp.Create("%s%s", folderAudioFx.GetString(), "logo.wav");
	logoFx = app->audio->LoadFx(tmp.GetString());
	app->audio->SetFxVolume(logoFx);
	return true;
}

bool LogoScene::PreUpdate()
{
	return true;
}

bool LogoScene::Update(float dt)
{
	if (once)
	{
		once = false;
		app->transition->FadeEffect(nullptr, this, true, floor(80.0f * dt));
	}

	if (timer < (15000 * dt))
	{
		timer++;
		if (timer >= (7500 * dt) && onceTimer)
		{
			app->audio->PlayFx(logoFx);
			onceTimer = false;
		}
	}
	else
	{
		app->transition->FadeEffect(this, (Module*)app->titleScene, false, floor(1200.0f * dt));
	}
	return true;
}

bool LogoScene::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		ret = false;
	}

	app->render->DrawTexture(logo, NULL, NULL, true);

	return ret;
}

bool LogoScene::CleanUp()
{
	LOG("Freeing scene");

	app->tex->UnLoad(logo);
	app->audio->UnloadFx(logoFx);

	return true;
}