#pragma once
#include "IRender.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include <fstream>
#include <string>

#define FAILED_ERROR(hr,str) if(FAILED(hr)){MessageBox(NULL, str, L"Error", MB_OK);}

struct alignas(16) Vertex
{
	DirectX::XMFLOAT3 pos;
	//DirectX::XMFLOAT2 uv;
};

struct CBuffer
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX proj;
	DirectX::XMFLOAT4 color;
};
class IndexDrawer:public IRender
{
public:
	IndexDrawer() { eyePt = { 0, 0, -15 }; lookPt = { 0,0,0 }; };
	~IndexDrawer() {};

	HRESULT CreateTexture(std::wstring path);
	HRESULT CreateSampler();
	void Init(std::string meshPath);
	void Update();
	HRESULT Draw()override;
private:
	void SetupSRV();
	void SetupSampler();
	ID3D11InputLayout*pInputLayout;
	ID3D11VertexShader*pVS;
	ID3D11PixelShader*pPS;
	ID3D11Buffer*pCB;
	ID3D11Buffer*pVB;
	ID3D11Buffer*pIB;
	std::vector<UINT>index;
	std::vector<Vertex>vertex;
	ID3D11ShaderResourceView*pSRV;
	ID3D11SamplerState* pSamp;
	DirectX::XMFLOAT3 eyePt;
	DirectX::XMFLOAT3 lookPt;
};

class MeshReadHelper
{
public:
	/*!
		@struct	ReadBuffer
		@brief	�ǂݍ��񂾃o�b�t�@
	*/
	struct ReadBuffer
	{
		/*!
			@var	vertices
			@brief	���_
			@TODO	�O���t�@�C���ɂ�double�^�ŏ������܂�Ă��邽�߁Afloat�^�ɃL���X�g����
		*/
		std::vector<Vertex>vertices;

		/*!
			@var	indices
			@brief	���_�C���f�b�N�X
		*/
		std::vector<uint32_t>indices;
	};
	static ReadBuffer Read(std::string path);

private:
	MeshReadHelper() = delete;
	~MeshReadHelper() = delete;
};