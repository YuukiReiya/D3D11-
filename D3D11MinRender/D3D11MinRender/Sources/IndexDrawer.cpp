#include "IndexDrawer.h"
#include "Main.h"
#include "vs_test.h"
#include "ps_test.h"
#include "Render.h"

using namespace DirectX;

void IndexDrawer::Init()
{
	HRESULT hr;
	//	頂点レイアウト
	{
		//	頂点レイアウト
		D3D11_INPUT_ELEMENT_DESC vd[]
		{
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,	 0,							  0,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"COLOR",	0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		};
		uint32_t ne = sizeof(vd) / sizeof(*vd);
		hr = Main::pDevice->CreateInputLayout(
			vd,
			ARRAYSIZE(vd),
			g_vs_main,
			sizeof(g_vs_main),
			&pInputLayout
		);
		FAILED_ERROR(hr, L"inputlayout");
		if (FAILED(hr)) { return; }
	}

	//	頂点シェーダー
	{
		hr = Main::pDevice->CreateVertexShader(
			&g_vs_main,
			sizeof(g_vs_main),
			NULL,
			&pVS
		);
		FAILED_ERROR(hr, L"vertexshader");
		if (FAILED(hr)) { return; }
	}

	//	ピクセルシェーダー
	{
		hr = Main::pDevice->CreatePixelShader(
			&g_ps_main,
			sizeof(g_ps_main),
			0,
			&pPS
		);
		FAILED_ERROR(hr, L"pixelshader");
		if (FAILED(hr)) { return; }
	}

	//	コンスタントバッファ
	{
		D3D11_BUFFER_DESC cb;
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.ByteWidth = sizeof(CBuffer);
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.MiscFlags = 0;
		cb.StructureByteStride = 0;
		cb.Usage = D3D11_USAGE_DYNAMIC;
		hr = Main::pDevice->CreateBuffer(
			&cb,
			0,
			&pCB
		);
		FAILED_ERROR(hr, L"CBUFFER");
		if (FAILED(hr)) { return; }
	}

	//	頂点バッファ
	{
		Vertex v[] = {
			XMFLOAT3(-0.2f, 0.7f, 0.0f),
			XMFLOAT3(0.5f, 0.5f, 0.0f)
		};
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(Vertex) * 2;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = v;
		hr = Main::pDevice->CreateBuffer(
			&bd, &sd, &pVB
		);
		FAILED_ERROR(hr, L"vertexbuffer");
		if (FAILED(hr)) { return; }
		uint32_t stride = sizeof(Vertex);
		uint32_t offset = 0;
		Main::pContext->IASetVertexBuffers(
			0, 1, &pVB, &stride, &offset
		);
	}

	//	インデックスバッファ
	{
		int is[] = {
			0,1
		};

		D3D11_BUFFER_DESC bd;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int)*2;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		SecureZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = is;

		hr = Main::pDevice->CreateBuffer(&bd, &sd, &pIB);
		FAILED_ERROR(hr, L"indexbuffer");
		if (FAILED(hr)) { return; }

		Main::pContext->IASetIndexBuffer(pIB, DXGI_FORMAT_R32_UINT, 0);
	}
}

HRESULT IndexDrawer::Draw()
{
	HRESULT hr;
	XMMATRIX w, v, p;

	//	world
	{
		w = XMMatrixTranslation(0, 0, 0);
	}

	//	view
	{
		XMVECTOR eye, look, upvec;
		eye = { 0,0,-2 };
		look = { 0,0,0 };
		upvec = { 0,1,0 };
		v = XMMatrixLookAtLH(eye, look, upvec);
	}

	//	proj
	{
		float pi = 3.14159265358979323846f;
		p = XMMatrixPerspectiveFovLH(pi / 4, Main::c_WWidth / Main::c_WHeight, 0.1f, 100.0f);
	}

	//	シェーダーセット
	{
		Main::pContext->VSSetShader(pVS, 0, 0);
		Main::pContext->PSSetShader(pPS, 0, 0);
	}

	//	コンスタントバッファ送信
	{
		D3D11_MAPPED_SUBRESOURCE pData;
		CBuffer cb;
		hr = Main::pContext->Map(pCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		FAILED_ERROR(hr, L"map");

		cb.world = w;
		cb.view = v;
		cb.proj = p;

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		Main::pContext->Unmap(pCB, 0);
	}

	//	CBufferを使うシェーダーのバインド
	{
		Main::pContext->VSSetConstantBuffers(0, 1, &pCB);
		Main::pContext->PSSetConstantBuffers(0, 1, &pCB);
	}

	//	頂点レイアウトセット
	{
		Main::pContext->IASetInputLayout(pInputLayout);
	}

	//	トポロジー
	{
		Main::pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	}

	//Main::pContext->Draw(2, 0);
	Main::pContext->DrawIndexed(2, 0, 0);
	return S_OK;
}
