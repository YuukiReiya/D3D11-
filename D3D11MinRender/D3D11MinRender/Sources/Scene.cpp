#include "Scene.h"
#include "Main.h"
#include "vs_test.h"
#include "ps_test.h"
#include "Render.h"
#include "Keyboard.h"

void Scene::Init()
{
	id = new IndexDrawer;
	id->Init("test.yfm");
	//id->CreateTexture(L"textures.png");
	FAILED_ERROR(id->CreateTexture(L"hoge.png"),L"cretae texture");
	id->CreateSampler();
}

void Scene::Update()
{
	Keyboard::UpdateBuffer();
	Render::Add(id);
	id->Update();
}

