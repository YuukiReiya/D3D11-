#include "Render.h"

using namespace std;
vector<IRender*> Render::pRenders;

void Render::Draw()
{
	for (auto it:pRenders)
	{
		if (FAILED(it->Draw())) {
			MessageBox(NULL, L"draw miss!", L"dError", MB_OK);
		}
	}
	pRenders.clear();
}

void Render::Add(IRender * render)
{
	pRenders.push_back(render);
}
