#include "Scene.h"
#include "Main.h"
#include "vs_test.h"
#include "ps_test.h"
#include "Render.h"


void Scene::Init()
{
	id = new IndexDrawer;
	id->Init();
}

void Scene::Update()
{
	Render::Add(id);
}

