#pragma once
#include "IRender.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#define FAILED_ERROR(hr,str) if(FAILED(hr)){MessageBox(NULL, str, L"Error", MB_OK);}

class IndexDrawer:public IRender
{
public:
	IndexDrawer() {};
	~IndexDrawer() {};

	void Init();
	HRESULT Draw()override;
private:
	ID3D11InputLayout*pInputLayout;
	ID3D11VertexShader*pVS;
	ID3D11PixelShader*pPS;
	ID3D11Buffer*pCB;
	ID3D11Buffer*pVB;
	ID3D11Buffer*pIB;
};

struct Vertex
{
	DirectX::XMFLOAT3 pos;
};

struct CBuffer
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX proj;
};