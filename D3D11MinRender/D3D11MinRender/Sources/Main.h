#pragma once
#include <Windows.h>
#include <d3dcompiler.h>
#include <d3d11.h>
#include "Window.h"
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dCompiler.lib")
#include "Scene.h"


#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}


class Main
{
public:
	Main() {}
	~Main() {}

	static HRESULT InitWindow(HINSTANCE);
	static HRESULT InitD3D();
	static void InitScene();
	static void Loop();
private:
	static void App();
	static void Render();
	static void DestroyD3D();

public:
	static HWND hWnd;
	static ID3D11Device*pDevice;
	static ID3D11DeviceContext*pContext;
	static IDXGISwapChain*pSwapChain;
	static ID3D11RenderTargetView*pRTV;
	static ID3D11DepthStencilView*pDSV;
	static ID3D11Texture2D*pBackBufferDST;
	static Window*pWindow;

	static Scene* pScene;

	static constexpr UINT c_WWidth = 1980;
	static constexpr UINT c_WHeight = 1080;
};