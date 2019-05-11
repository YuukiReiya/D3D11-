#pragma once
#include <vector>
#include "IRender.h"

class Render
{
	Render() = delete;
	~Render() = delete;
public:
	static void Draw();

	static void Add(IRender*render);
private:
	static std::vector<IRender*>pRenders;
};
