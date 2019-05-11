#include <d3d11.h>
#include <Windows.h>
#include "Main.h"


Main*g_pMain = nullptr;

int WINAPI WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,INT)
{
	//g_pMain = new Main;
	
	HRESULT hr;
	//hr = g_pMain->InitWindow(hInst);
	hr = Main::InitWindow(hInst);
	FAILED_ERROR(hr, L"window¶¬Ž¸”s");
	//hr = g_pMain->InitD3D();
	hr = Main::InitD3D();
	FAILED_ERROR(hr, L"d3d‰Šú‰»Ž¸”s");

	Main::InitScene();

	//g_pMain->Loop();
	Main::Loop();
	return 0;
}