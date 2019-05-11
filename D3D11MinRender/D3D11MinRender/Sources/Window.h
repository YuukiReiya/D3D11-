#pragma once
#include <Windows.h>


class Window
{
public:
	Window() = default;
	~Window() {};

	HRESULT Init(HWND*, HINSTANCE, INT, INT, INT, INT, LPCWSTR);
	LRESULT MsgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

private:
};