#include "Main.h"
#include "Render.h"
////////////////////////
HWND Main::hWnd;
ID3D11Device*Main::pDevice;
ID3D11DeviceContext*Main::pContext;
IDXGISwapChain*Main::pSwapChain;
ID3D11RenderTargetView*Main::pRTV;
ID3D11DepthStencilView*Main::pDSV;
ID3D11Texture2D*Main::pBackBufferDST;
Window*Main::pWindow;
Scene*Main::pScene;
////////////////////////

HRESULT Main::InitWindow(HINSTANCE hInstance)
{
	pWindow = new Window;
	return pWindow->Init(&hWnd,hInstance,0,0,c_WWidth,c_WHeight,L"Test");
}

HRESULT Main::InitD3D()
{
	// �f�o�C�X�ƃX���b�v�`�F�[���̍쐬
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = c_WWidth;
	sd.BufferDesc.Height = c_WHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;
	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		0, &pFeatureLevels, 1, D3D11_SDK_VERSION, &sd, &pSwapChain, &pDevice,
		pFeatureLevel, &pContext)))
	{
		return E_FAIL;
	}

	//�e��e�N�X�`���[�ƁA����ɕt�т���e��r���[���쐬

	//�o�b�N�o�b�t�@�[�e�N�X�`���[���擾�i���ɂ���̂ō쐬�ł͂Ȃ��j
	ID3D11Texture2D *pBackBuffer_Tex;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer_Tex);
	//���̃e�N�X�`���[�ɑ΂������_�[�^�[�Q�b�g�r���[(RTV)���쐬
	pDevice->CreateRenderTargetView(pBackBuffer_Tex, NULL, &pRTV);
	SAFE_RELEASE(pBackBuffer_Tex);

	//�f�v�X�X�e���V���r���[�p�̃e�N�X�`���[���쐬
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = c_WWidth;
	descDepth.Height = c_WHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	pDevice->CreateTexture2D(&descDepth, NULL, &pBackBufferDST);
	//���̃e�N�X�`���[�ɑ΂��f�v�X�X�e���V���r���[(DSV)���쐬
	pDevice->CreateDepthStencilView(pBackBufferDST, NULL, &pDSV);

	//�����_�[�^�[�Q�b�g�r���[�Ɛ[�x�X�e���V���r���[���p�C�v���C���Ƀo�C���h
	pContext->OMSetRenderTargets(1, &pRTV, pDSV);
	//�r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = c_WWidth;
	vp.Height = c_WHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1, &vp);

	//���X�^���C�Y�ݒ�
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.CullMode = D3D11_CULL_NONE;
	rdc.FillMode = D3D11_FILL_SOLID;
	ID3D11RasterizerState* pIr = NULL;
	pDevice->CreateRasterizerState(&rdc, &pIr);
	pContext->RSSetState(pIr);


	return S_OK;
}

void Main::InitScene()
{
	pScene = new Scene();
	pScene->Init();
}

void Main::Loop()
{
	// ���b�Z�[�W���[�v
	MSG msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//�A�v���P�[�V�����̏����͂��������ԁB
			App();
		}
	}
	//�A�v���P�[�V�����̏I��
}

void Main::App()
{
	pScene->Update();
	Render();
}

void Main::Render()
{
	//��ʃN���A
	float ClearColor[4] = { 0,0,0.5,1 };// �N���A�F�쐬�@RGBA�̏�
	pContext->ClearRenderTargetView(pRTV, ClearColor);//�J���[�o�b�t�@�N���A
	pContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//�f�v�X�X�e���V���o�b�t�@�N���A

	Render::Draw();

	pSwapChain->Present(0, 0);
}

void Main::DestroyD3D()
{
	SAFE_RELEASE(pSwapChain);
	SAFE_RELEASE(pRTV);
	SAFE_RELEASE(pDSV);
	SAFE_RELEASE(pBackBufferDST);
	SAFE_RELEASE(pContext);
	SAFE_RELEASE(pDevice);

}