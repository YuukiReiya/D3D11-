#include "Scene.h"
#include "Main.h"
#include "vs_test.h"
#include "ps_test.h"
#include "Render.h"
#include "Keyboard.h"

void Scene::Init()
{
	id = new IndexDrawer;
	id->Init();
}

void Scene::Update()
{
	Keyboard::UpdateBuffer();
	Render::Add(id);
	id->Update();
}

