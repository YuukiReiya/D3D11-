#include "IndexDrawer.h"
#include "Main.h"
#include "vs_test.h"
#include "ps_test.h"
#include "Render.h"
#include <string>
#include "Keyboard.h"

using namespace DirectX;
using namespace std;
typedef Keyboard key;


void IndexDrawer::Init()
{
	HRESULT hr;
	//	���_���C�A�E�g
	{
		//	���_���C�A�E�g
		D3D11_INPUT_ELEMENT_DESC vd[]
		{
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,	 0,							  0,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"COLOR",	0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		};
		uint32_t en = sizeof(vd) / sizeof(*vd);
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

	//	���_�V�F�[�_�[
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

	//	�s�N�Z���V�F�[�_�[
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

	//	�R���X�^���g�o�b�t�@
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

	auto read = MeshReadHelper::Read("test.yfm");

	//	���_�o�b�t�@
	{
		//	���_
		{
			//�E��
			vertex.push_back({ { 0.5f, 0.5f,0},{1,0,0,1} });
			//�E��
			vertex.push_back({ { 0.5f, -0.5f,0},{1,0,0,1} });
			//����
			vertex.push_back({ {-0.5f, -0.5f,0},{1,0,0,1} });
			//����
			vertex.push_back({ {-0.5f, 0.5f,0},{1,0,0,1} });

			vertex.push_back({ {-0.0f, 1.0f,0},{1,0,0,1} });

			vertex.clear();
			int i = 0;
			while (i < read.vertices.size())
			{
				vertex.push_back({ { read.vertices[i].pos }, { 1,0,0,1 } });
				i++;
			}
		}
		//Vertex v[] = {
		//	//	�l�p�`
		//	{XMFLOAT3(0.0f, 0.5f,0),XMFLOAT4(1,0,0,1)},
		//	{XMFLOAT3(0.5f,-0.5f,0),XMFLOAT4(0,1,0,1)},
		//	{XMFLOAT3(-0.5f,-0.5f,0),XMFLOAT4(0,0,1,1)},
		//	//{XMFLOAT3(0.5f,0.5f,0),XMFLOAT4(1,1,1,1)},
		//};
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		//	bd.ByteWidth = sizeof(Vertex) * (sizeof(Vertex) / sizeof(*vertex));
		bd.ByteWidth = sizeof(Vertex) * vertex.size();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = vertex.data();
		hr = Main::pDevice->CreateBuffer(
			&bd, &sd, &pVB
		);
		FAILED_ERROR(hr, L"vertexbuffer");
		if (FAILED(hr)) { return; }
	}

	//	�C���f�b�N�X�o�b�t�@
	{
		//	�C���f�b�N�X
		{
			index.push_back(0);
			index.push_back(1);
			index.push_back(2);
			index.push_back(3);
			index.push_back(0);
			index.push_back(4);

			index.clear();
			index = read.indices;
			//index.push_back(1);
		}
		D3D11_BUFFER_DESC bd;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int)*index.size();
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		SecureZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index.data();

		hr = Main::pDevice->CreateBuffer(&bd, &sd, &pIB);
		FAILED_ERROR(hr, L"indexbuffer");
		if (FAILED(hr)) { return; }

		Main::pContext->IASetIndexBuffer(pIB, DXGI_FORMAT_R32_UINT, 0);
	}
}

void IndexDrawer::Update()
{
	float value = 0.1f;

	if (key::GetButton('A'))
	{
		eyePt.x -= value;
	}
	if (key::GetButton('D'))
	{
		eyePt.x += value;
	}
	if (key::GetButton('s'))
	{
		eyePt.y -= value;
	}
	if (key::GetButton('w'))
	{
		eyePt.y += value;
	}

}

HRESULT IndexDrawer::Draw()
{
	HRESULT hr;
	XMMATRIX w, v, p;

	//	world
	{
		w = XMMatrixTranslation(0, 0, 0);
		w = XMMatrixTranspose(w);
	}

	//	view
	{
		XMVECTOR eye, look, upvec;
		eye = { eyePt.x,eyePt.y,eyePt.z };
		look = { 0,0,0 };
		upvec = { 0,1,0 };
		v = XMMatrixLookAtLH(eye, look, upvec);
		v = XMMatrixTranspose(v);
	}

	//	proj
	{
		float pi = 3.14159265358979323846f;
		p = XMMatrixPerspectiveFovLH(pi / 4, Main::c_WWidth / Main::c_WHeight, 0.1f, 100.0f);
		p = XMMatrixTranspose(p);
	}

	//	�V�F�[�_�[�Z�b�g
	{
		Main::pContext->VSSetShader(pVS, 0, 0);
		Main::pContext->PSSetShader(pPS, 0, 0);
	}

	//	�R���X�^���g�o�b�t�@���M
	{
		D3D11_MAPPED_SUBRESOURCE pData;
		CBuffer cb;
		hr = Main::pContext->Map(pCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		FAILED_ERROR(hr, L"map");
		if (FAILED(hr)) { exit(0); }
		cb.world = w;
		cb.view = v;
		cb.proj = p;

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		Main::pContext->Unmap(pCB, 0);
	}

	//	CBuffer���g���V�F�[�_�[�̃o�C���h
	{
		Main::pContext->VSSetConstantBuffers(0, 1, &pCB);
		Main::pContext->PSSetConstantBuffers(0, 1, &pCB);
	}

	//	���_�o�b�t�@�Z�b�g
	{
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		Main::pContext->IASetVertexBuffers(0, 1, &pVB, &stride, &offset);
	}

	//	���_���C�A�E�g�Z�b�g
	{
		Main::pContext->IASetInputLayout(pInputLayout);
	}

	//	�g�|���W�[
	{
		//	���_����H(�|���S����)
		//Main::pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//	�Y��Ƀ|���S���\���o���Ă�����ۂ�
		Main::pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		//	�\���b�h(�������͑��v���ۂ�)
		Main::pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);

		//
	}

	//Main::pContext->Draw(3, 0);
	Main::pContext->DrawIndexed(index.size(), 0, 0);
	return S_OK;
}

MeshReadHelper::ReadBuffer MeshReadHelper::Read(std::string path)
{
	ifstream ifs(path);
	if (ifs.fail()) {
		string error = "Failed to read \"" + path + "\" file.";
		return ReadBuffer();
	}

	ReadBuffer ret;
	string buf;

	//	�o�b�t�@�ɕ�����l��
	getline(ifs, buf);
	string t = "}";

	//	{1 1 1}{2 2 2}

	//���_
	while (true)
	{
		auto a = buf.find(t);
		if (a == string::npos) { break; }

		//	{1 1 1
		auto b = buf.substr(0, a);

		auto c = b.find("{");
		if (c != string::npos) {
			b = b.substr(c + 1);
		}
		//	1 1 1
		{
			Vertex tmp;

			//	x
			auto d = b.find(" ");
			auto e = b.substr(0, d);

			tmp.pos.x = stof(e);
			b = b.substr(d + 1);

			//	y
			auto f = b.find(" ");
			auto g = b.substr(0, f + 1);

			tmp.pos.y = stof(g);
			b = b.substr(f + 1);

			//	z
			tmp.pos.z = stof(b);
			ret.vertices.push_back(tmp);
		}

		buf = buf.substr(a + 1);
	}

	getline(ifs, buf);
	//�C���f�b�N�X
	while (true)
	{
		auto a = buf.find(" ");
		if (a == string::npos) { break; }
		auto b = buf.substr(0, a);

		ret.indices.push_back(stoi(b));

		buf = buf.substr(a + 1);
	}
	return ret;
}
