#pragma once
#include <d3d11.h>
#include "IndexDrawer.h"

class Scene
{
public:
	void Init();
	void Update();

	Scene() = default;
	~Scene() = default;
private:

	IndexDrawer* id;
};

