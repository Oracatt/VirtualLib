#include "../../Common/EngineDef.h"
#include "../Include/Engine.h"
#include "../Include/Renderer.h"
#include "../Include/RenderTarget.h"
#include <d3d11_4.h>

const char canvasShaderBlob[]
{
0x52,0x61,0x73,0x74,0x65,0x72,0x69,0x7a,0x65,0x72,
0x53,0x74,0x61,0x74,0x65,0x20,0x52,0x53,0x74,0x61,
0x74,0x65,0x0d,0x0a,0x7b,0x0d,0x0a,0x09,0x46,0x69,
0x6c,0x6c,0x4d,0x6f,0x64,0x65,0x20,0x3d,0x20,0x53,
0x6f,0x6c,0x69,0x64,0x3b,0x20,0x0d,0x0a,0x09,0x43,
0x75,0x6c,0x6c,0x4d,0x6f,0x64,0x65,0x20,0x3d,0x20,
0x42,0x61,0x63,0x6b,0x3b,0x20,0x0d,0x0a,0x09,0x46,
0x72,0x6f,0x6e,0x74,0x43,0x6f,0x75,0x6e,0x74,0x65,
0x72,0x43,0x6c,0x6f,0x63,0x6b,0x77,0x69,0x73,0x65,
0x20,0x3d,0x20,0x66,0x61,0x6c,0x73,0x65,0x3b,0x20,
0x0d,0x0a,0x09,0x44,0x65,0x70,0x74,0x68,0x43,0x6c,
0x69,0x70,0x45,0x6e,0x61,0x62,0x6c,0x65,0x20,0x3d,
0x20,0x66,0x61,0x6c,0x73,0x65,0x3b,0x0d,0x0a,0x7d,
0x3b,0x20,0x0d,0x0a,0x0d,0x0a,0x73,0x74,0x72,0x75,
0x63,0x74,0x20,0x56,0x65,0x72,0x74,0x65,0x78,0x49,
0x6e,0x0d,0x0a,0x7b,0x0d,0x0a,0x09,0x66,0x6c,0x6f,
0x61,0x74,0x33,0x20,0x50,0x6f,0x73,0x4c,0x20,0x20,
0x20,0x20,0x3a,0x20,0x50,0x4f,0x53,0x49,0x54,0x49,
0x4f,0x4e,0x3b,0x0d,0x0a,0x09,0x66,0x6c,0x6f,0x61,
0x74,0x32,0x20,0x54,0x65,0x78,0x20,0x20,0x20,0x20,
0x20,0x3a,0x20,0x54,0x45,0x58,0x43,0x4f,0x4f,0x52,
0x44,0x3b,0x0d,0x0a,0x7d,0x3b,0x0d,0x0a,0x0d,0x0a,
0x73,0x74,0x72,0x75,0x63,0x74,0x20,0x56,0x65,0x72,
0x74,0x65,0x78,0x4f,0x75,0x74,0x0d,0x0a,0x7b,0x0d,
0x0a,0x09,0x66,0x6c,0x6f,0x61,0x74,0x34,0x20,0x50,
0x6f,0x73,0x48,0x20,0x20,0x20,0x20,0x3a,0x20,0x53,
0x56,0x5f,0x50,0x4f,0x53,0x49,0x54,0x49,0x4f,0x4e,
0x3b,0x0d,0x0a,0x09,0x66,0x6c,0x6f,0x61,0x74,0x32,
0x20,0x54,0x65,0x78,0x20,0x20,0x20,0x20,0x20,0x3a,
0x20,0x54,0x45,0x58,0x43,0x4f,0x4f,0x52,0x44,0x3b,
0x0d,0x0a,0x7d,0x3b,0x0d,0x0a,0x0d,0x0a,0x63,0x62,
0x75,0x66,0x66,0x65,0x72,0x20,0x63,0x62,0x50,0x65,
0x72,0x4f,0x62,0x6a,0x65,0x63,0x74,0x0d,0x0a,0x7b,
0x0d,0x0a,0x09,0x66,0x6c,0x6f,0x61,0x74,0x34,0x78,
0x34,0x20,0x67,0x57,0x6f,0x72,0x6c,0x64,0x56,0x69,
0x65,0x77,0x50,0x72,0x6f,0x6a,0x3b,0x0d,0x0a,0x09,
0x66,0x6c,0x6f,0x61,0x74,0x34,0x78,0x34,0x20,0x67,
0x54,0x65,0x78,0x54,0x72,0x61,0x6e,0x73,0x66,0x6f,
0x72,0x6d,0x3b,0x0d,0x0a,0x7d,0x3b,0x20,0x0d,0x0a,
0x0d,0x0a,0x54,0x65,0x78,0x74,0x75,0x72,0x65,0x32,
0x44,0x20,0x67,0x44,0x69,0x66,0x66,0x75,0x73,0x65,
0x4d,0x61,0x70,0x3b,0x0d,0x0a,0x0d,0x0a,0x53,0x61,
0x6d,0x70,0x6c,0x65,0x72,0x53,0x74,0x61,0x74,0x65,
0x20,0x73,0x61,0x6d,0x41,0x6e,0x69,0x73,0x6f,0x74,
0x72,0x6f,0x70,0x69,0x63,0x0d,0x0a,0x7b,0x0d,0x0a,
0x09,0x46,0x69,0x6c,0x74,0x65,0x72,0x20,0x3d,0x20,
0x41,0x4e,0x49,0x53,0x4f,0x54,0x52,0x4f,0x50,0x49,
0x43,0x3b,0x0d,0x0a,0x09,0x4d,0x61,0x78,0x41,0x6e,
0x69,0x73,0x6f,0x74,0x72,0x6f,0x70,0x79,0x20,0x3d,
0x20,0x34,0x3b,0x0d,0x0a,0x0d,0x0a,0x09,0x41,0x64,
0x64,0x72,0x65,0x73,0x73,0x55,0x20,0x3d,0x20,0x57,
0x52,0x41,0x50,0x3b,0x0d,0x0a,0x09,0x41,0x64,0x64,
0x72,0x65,0x73,0x73,0x56,0x20,0x3d,0x20,0x57,0x52,
0x41,0x50,0x3b,0x0d,0x0a,0x7d,0x3b,0x0d,0x0a,0x0d,
0x0a,0x56,0x65,0x72,0x74,0x65,0x78,0x4f,0x75,0x74,
0x20,0x56,0x53,0x28,0x56,0x65,0x72,0x74,0x65,0x78,
0x49,0x6e,0x20,0x76,0x69,0x6e,0x29,0x0d,0x0a,0x7b,
0x0d,0x0a,0x09,0x56,0x65,0x72,0x74,0x65,0x78,0x4f,
0x75,0x74,0x20,0x76,0x6f,0x75,0x74,0x3b,0x0d,0x0a,
0x09,0x76,0x6f,0x75,0x74,0x2e,0x50,0x6f,0x73,0x48,
0x20,0x3d,0x20,0x6d,0x75,0x6c,0x28,0x66,0x6c,0x6f,
0x61,0x74,0x34,0x28,0x76,0x69,0x6e,0x2e,0x50,0x6f,
0x73,0x4c,0x2c,0x20,0x31,0x2e,0x30,0x66,0x29,0x2c,
0x20,0x67,0x57,0x6f,0x72,0x6c,0x64,0x56,0x69,0x65,
0x77,0x50,0x72,0x6f,0x6a,0x29,0x3b,0x0d,0x0a,0x09,
0x76,0x6f,0x75,0x74,0x2e,0x54,0x65,0x78,0x20,0x3d,
0x20,0x6d,0x75,0x6c,0x28,0x66,0x6c,0x6f,0x61,0x74,
0x34,0x28,0x76,0x69,0x6e,0x2e,0x54,0x65,0x78,0x2c,
0x20,0x30,0x2e,0x30,0x66,0x2c,0x20,0x31,0x2e,0x30,
0x66,0x29,0x2c,0x20,0x67,0x54,0x65,0x78,0x54,0x72,
0x61,0x6e,0x73,0x66,0x6f,0x72,0x6d,0x29,0x2e,0x78,
0x79,0x3b,0x0d,0x0a,0x09,0x72,0x65,0x74,0x75,0x72,
0x6e,0x20,0x76,0x6f,0x75,0x74,0x3b,0x0d,0x0a,0x7d,
0x0d,0x0a,0x0d,0x0a,0x66,0x6c,0x6f,0x61,0x74,0x34,
0x20,0x50,0x53,0x28,0x56,0x65,0x72,0x74,0x65,0x78,
0x4f,0x75,0x74,0x20,0x70,0x69,0x6e,0x29,0x20,0x3a,
0x20,0x53,0x56,0x5f,0x54,0x61,0x72,0x67,0x65,0x74,
0x0d,0x0a,0x7b,0x0d,0x0a,0x09,0x66,0x6c,0x6f,0x61,
0x74,0x34,0x20,0x74,0x65,0x78,0x43,0x6f,0x6c,0x6f,
0x72,0x20,0x3d,0x20,0x67,0x44,0x69,0x66,0x66,0x75,
0x73,0x65,0x4d,0x61,0x70,0x2e,0x53,0x61,0x6d,0x70,
0x6c,0x65,0x28,0x73,0x61,0x6d,0x41,0x6e,0x69,0x73,
0x6f,0x74,0x72,0x6f,0x70,0x69,0x63,0x2c,0x20,0x70,
0x69,0x6e,0x2e,0x54,0x65,0x78,0x29,0x3b,0x0d,0x0a,
0x09,0x74,0x65,0x78,0x43,0x6f,0x6c,0x6f,0x72,0x2e,
0x61,0x20,0x3d,0x20,0x31,0x3b,0x0d,0x0a,0x09,0x72,
0x65,0x74,0x75,0x72,0x6e,0x20,0x74,0x65,0x78,0x43,
0x6f,0x6c,0x6f,0x72,0x3b,0x0d,0x0a,0x7d,0x0d,0x0a,
0x0d,0x0a,0x0d,0x0a,0x74,0x65,0x63,0x68,0x6e,0x69,
0x71,0x75,0x65,0x31,0x31,0x20,0x54,0x30,0x0d,0x0a,
0x7b,0x0d,0x0a,0x09,0x70,0x61,0x73,0x73,0x20,0x50,
0x30,0x0d,0x0a,0x09,0x7b,0x0d,0x0a,0x09,0x09,0x53,
0x65,0x74,0x56,0x65,0x72,0x74,0x65,0x78,0x53,0x68,
0x61,0x64,0x65,0x72,0x28,0x43,0x6f,0x6d,0x70,0x69,
0x6c,0x65,0x53,0x68,0x61,0x64,0x65,0x72,0x28,0x76,
0x73,0x5f,0x35,0x5f,0x30,0x2c,0x20,0x56,0x53,0x28,
0x29,0x29,0x29,0x3b,0x0d,0x0a,0x09,0x09,0x53,0x65,
0x74,0x50,0x69,0x78,0x65,0x6c,0x53,0x68,0x61,0x64,
0x65,0x72,0x28,0x43,0x6f,0x6d,0x70,0x69,0x6c,0x65,
0x53,0x68,0x61,0x64,0x65,0x72,0x28,0x70,0x73,0x5f,
0x35,0x5f,0x30,0x2c,0x20,0x50,0x53,0x28,0x29,0x29,
0x29,0x3b,0x0d,0x0a,0x09,0x09,0x53,0x65,0x74,0x52,
0x61,0x73,0x74,0x65,0x72,0x69,0x7a,0x65,0x72,0x53,
0x74,0x61,0x74,0x65,0x28,0x52,0x53,0x74,0x61,0x74,
0x65,0x29,0x3b,0x0d,0x0a,0x09,0x7d,0x0d,0x0a,0x7d,
0x0d,0x0a
};

Renderer::Renderer(Engine* eng) :engine(eng) {}

HRESULT Renderer::InitDirect3D(VideoDesc& desc, HWND hwnd)
{
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if  defined(DEBUG)||defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevel;
	D3D_FEATURE_LEVEL createLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};

	HRESULT hr = D3D11CreateDevice(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		createDeviceFlags,
		createLevels, ARRAYSIZE(createLevels),
		D3D11_SDK_VERSION,
		&pDevice,
		&featureLevel,
		&pContext);
	if (FAILED(hr))
		return ERR_CREATED3D;

	ComPtr<ID3D11Multithread> pMultithread;
	pContext.As(&pMultithread);
	pMultithread->SetMultithreadProtected(true);

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		return ERR_D3DNOTSUPPORT;
	}

	int scrWidth = GetSystemMetrics(SM_CXSCREEN);
	int scrHeight = GetSystemMetrics(SM_CYSCREEN);
	UINT newWidth, newHeight;
	float newaspect = (float)scrWidth / (float)scrHeight;
	float oldaspect = (float)desc.bufferWidth / (float)desc.bufferHeight;

	if (desc.bufferWidth == 0 && desc.bufferHeight == 0 && !desc.windowed)
	{
		desc.bufferWidth = scrWidth;
		desc.bufferHeight = scrHeight;
		newWidth = desc.bufferWidth;
		newHeight = desc.bufferHeight;
	}
	else if (!desc.windowed)
	{
		if (desc.fixedAspect)
		{
			if (newaspect > oldaspect)
			{
				newWidth = (UINT)(oldaspect * scrHeight);
				newHeight = (UINT)scrHeight;
			}
			else
			{
				newWidth = (UINT)scrWidth;
				newHeight = (UINT)(scrWidth / oldaspect);
			}
		}
		else
		{
			newWidth = scrWidth;
			newHeight = scrHeight;
		}
	}
	else
	{
		newWidth = desc.bufferWidth;
		newHeight = desc.bufferHeight;
	}

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferDesc.Width = newWidth;
	sd.BufferDesc.Height = newHeight;
	sd.BufferDesc.RefreshRate.Numerator = desc.refreshRate;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	if (!desc.fixedAspect)
	{
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		lastSCM = SCALING_MODE_SCREENASPECT;
	}
	else
	{
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
		lastSCM = SCALING_MODE_FIXEDASPECT;
	}
	if (desc.msaaEnabled)
	{
		sd.SampleDesc.Count = desc.multiSampleCount;
		UINT  msaaQuality;
		if (FAILED(pDevice->CheckMultisampleQualityLevels(
			DXGI_FORMAT_B8G8R8A8_UNORM,
			desc.multiSampleCount,
			&msaaQuality)) || msaaQuality < 1)
		{
			return ERR_D3DCHECKMSAA;
		}
		sd.SampleDesc.Quality = msaaQuality - 1;
	}
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hwnd;
	sd.Windowed = desc.windowed;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	pDevice.As(&pDxgiDevice);
	pDxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pDxgiAdapter);
	pDxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pDxgiFactory);

	/*
	ComPtr<IDXGIAdapter> pAdapter;
	for (UINT i = 0; pDxgiFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		ComPtr<IDXGIOutput> pOutput;
		UINT j = 0;
		while (pAdapter->EnumOutputs(j, &pOutput) != DXGI_ERROR_NOT_FOUND)
		{
			UINT num = 0;
			DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM;
			UINT flags = DXGI_ENUM_MODES_INTERLACED;

			pOutput->GetDisplayModeList(format, flags, &num, 0);
			DXGI_MODE_DESC* pDescs = new DXGI_MODE_DESC[num];
			pOutput->GetDisplayModeList(format, flags, &num, pDescs);

			for (int k = 0; k < num; k++)
			{
				printf("%x %d %d %d %d %d %d\n", pAdapter, pDescs[k].Width, pDescs[k].Height, pDescs[k].RefreshRate.Numerator, pDescs[k].Scaling, pDescs[k].ScanlineOrdering,pDescs[k].Format);
			}

			j++;
		}
	}*/

	pDxgiFactory->CreateSwapChain(pDevice.Get(), &sd, &pSwapChain);
	pDxgiFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(&mRenderTargetBuffer));
	pDevice->CreateRenderTargetView(mRenderTargetBuffer.Get(), 0, &mRenderTargetRTView);
	pDevice->CreateShaderResourceView(mRenderTargetBuffer.Get(), 0, &mRenderTargetSRView);
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.Width = newWidth;
	depthStencilDesc.Height = newHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	if (desc.msaaEnabled)
	{
		depthStencilDesc.SampleDesc.Count = sd.SampleDesc.Count;
		depthStencilDesc.SampleDesc.Quality = sd.SampleDesc.Quality;
	}
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	pDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);
	pDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), 0, &mDepthStencilView);
	pContext->OMSetRenderTargets(1, mRenderTargetRTView.GetAddressOf(), mDepthStencilView.Get());
	isWindowed = desc.windowed;
	bufferSize = { newWidth,newHeight };

	D3D11_RENDER_TARGET_BLEND_DESC rtBlendDesc;
	rtBlendDesc.BlendEnable = true;
	rtBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
	rtBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	rtBlendDesc.DestBlendAlpha = D3D11_BLEND_ONE;
	rtBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	rtBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	rtBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	D3D11_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0] = rtBlendDesc;
	pDevice->CreateBlendState(&blendDesc, &mBlendStateNormal);
	rtBlendDesc.DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0] = rtBlendDesc;
	pDevice->CreateBlendState(&blendDesc, &mBlendStateBrighten);
	rtBlendDesc.BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	blendDesc.RenderTarget[0] = rtBlendDesc;
	pDevice->CreateBlendState(&blendDesc, &mBlendStateDarken);
	rtBlendDesc.DestBlend = D3D11_BLEND_ZERO;
	rtBlendDesc.SrcBlend = D3D11_BLEND_INV_DEST_COLOR;
	rtBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0] = rtBlendDesc;
	pDevice->CreateBlendState(&blendDesc, &mBlendStateInvDest);
	pContext->OMSetBlendState(mBlendStateNormal.Get(), 0, 0xffffffff);

	D3D11_TEXTURE2D_DESC canvasDesc;
	ZeroMemory(&canvasDesc, sizeof(canvasDesc));
	canvasDesc.ArraySize = 1;
	canvasDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	canvasDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	canvasDesc.Width = newWidth;
	canvasDesc.Height = newHeight;
	canvasDesc.MipLevels = 1;
	canvasDesc.MiscFlags = D3D11_RESOURCE_MISC_GDI_COMPATIBLE;
	canvasDesc.SampleDesc.Count = 1;
	canvasDesc.SampleDesc.Quality = 0;
	canvasDesc.Usage = D3D11_USAGE_DEFAULT;
	pDevice->CreateTexture2D(&canvasDesc, 0, &mCanvasBuffer);
	pDevice->CreateRenderTargetView(mCanvasBuffer.Get(), 0, &mCanvasRTView);
	pDevice->CreateShaderResourceView(mCanvasBuffer.Get(), 0, &mCanvasSRView);
	mCanvasBuffer.As(&mCanvasSurface);

	struct
	{
		Vector3 pos;
		Vector3 normal;
		Vector2 texCoord;
	}rectVertices[]{
		{{-0.5,0.5,0},{0,0,-1},{0,0}},
		{{0.5,0.5,0},{0,0,-1},{1,0}},
		{{-0.5,-0.5,0},{0,0,-1},{0,1}},
		{{-0.5,-0.5,0},{0,0,-1},{0,1}},
		{{0.5,0.5,0},{0,0,-1},{1,0}},
		{{0.5,-0.5,0},{0,0,-1},{1,1}}
	};
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(rectVertices);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = rectVertices;
	pDevice->CreateBuffer(&vbd, &vinitData, &rectVertexBuffer);

	struct
	{
		Vector3 pos;
		Vector2 texCoord;
	}canvasVertices[]{
		{{-0.5,0.5,0},{0,0}},
		{{0.5,0.5,0},{1,0}},
		{{-0.5,-0.5,0},{0,1}},
		{{-0.5,-0.5,0},{0,1}},
		{{0.5,0.5,0},{1,0}},
		{{0.5,-0.5,0},{1,1}}
	};
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(canvasVertices);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	vinitData.pSysMem = canvasVertices;
	pDevice->CreateBuffer(&vbd, &vinitData, &mCanvasVertexBuffer);

	DWORD shaderFlags = 0;
	ComPtr<ID3DBlob> compilationMsgs = 0;
	D3DX11CompileEffectFromMemory(canvasShaderBlob, sizeof(canvasShaderBlob), "tech.fx", 0,
		0, 0, 0, pDevice.Get(), &mCanvasEffect, &compilationMsgs);
	bool isContain(string str1, string str2);
	if (compilationMsgs != 0 && isContain((char*)compilationMsgs->GetBufferPointer(), "error"))
	{
		engine->ReportFatalError((char*)compilationMsgs->GetBufferPointer(), FE_UNKNOWN);
		return ERR_UNKNOWN;
	}

	mCanvasTech = mCanvasEffect->GetTechniqueByName("T0");
	mCanvasPass = mCanvasTech->GetPassByIndex(0);
	mCanvasMatWVP = mCanvasEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	mCanvasTexture = mCanvasEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mCanvasMatTexture = mCanvasEffect->GetVariableByName("gTexTransform")->AsMatrix();

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	D3DX11_PASS_DESC passDesc;
	mCanvasPass->GetDesc(&passDesc);
	pDevice->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &mCanvasInput);

	D2D1_FACTORY_OPTIONS options;
#if  defined(DEBUG)||defined(_DEBUG)
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#else
	options.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#endif
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, options, pD2DFactory.ReleaseAndGetAddressOf())))
		return ERR_CREATED2D;
	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory5),
		reinterpret_cast<IUnknown**>(pDWriteFactory.ReleaseAndGetAddressOf()))))
		return ERR_CREATED2D;
	D2D1_RENDER_TARGET_PROPERTIES props = RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
		PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
	if (FAILED(pD2DFactory->CreateDxgiSurfaceRenderTarget(mCanvasSurface.Get(), &props, &mD2DRenderTarget)))
		return ERR_CREATED2D;
	pD2DRenderTarget = mD2DRenderTarget;
	if (desc.fixedAspect)
		pD2DRenderTarget->SetTransform(Matrix3x2F::Scale({ newWidth / (float)desc.bufferWidth,newHeight / (float)desc.bufferHeight }));
	pD2DRenderTarget->CreateSolidColorBrush({ 1,1,1,1 }, reinterpret_cast<ID2D1SolidColorBrush**>(pBodyBrush.ReleaseAndGetAddressOf()));
	pD2DRenderTarget->CreateSolidColorBrush({ 0,0,0,1 }, reinterpret_cast<ID2D1SolidColorBrush**>(pOutlineBrush.ReleaseAndGetAddressOf()));
	pCustomTextRenderer = new CustomTextRenderer(pD2DFactory.Get(), pD2DRenderTarget.Get(), pBodyBrush.Get(), pOutlineBrush.Get());
	engine->CreateTextFormat("ËÎÌו", 40, &pFormat);

	RenderTarget* rt = new RenderTarget;
	rt->resizable = false;
	rt->size = { newWidth,newHeight };
	rt->canvasSupported = true;
	rt->hasDepth = true;
	rt->msaaEnabled = desc.msaaEnabled;
	mRenderTargetBuffer.As(&rt->mBufferSurface);
	rt->mD2DRenderTarget = mD2DRenderTarget;
	rt->mDepthStencilBuffer = mDepthStencilBuffer;
	rt->mDepthStencilView = mDepthStencilView;
	rt->mRenderTargetBuffer = mRenderTargetBuffer;
	rt->mRenderTargetRTView = mRenderTargetRTView;
	rt->mRenderTargetSRView = mRenderTargetSRView;
	mRenderTarget = rt;
	pRenderTarget = mRenderTarget;

	D3D11_VIEWPORT vp;
	vp.Width = newWidth;
	vp.Height = newHeight;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1, &vp);

	return SUCCEED_OPERATE;
}

void Renderer::ReleaseDirect3D()
{
	if (pSwapChain)
		pSwapChain->SetFullscreenState(false, 0);
	rendererDelegates.clear();
}

HRESULT Renderer::GetPointer(const IID& iid, void** ppvObject)
{
    if (iid == IID_ID3D11Device)
    {
		*ppvObject = pDevice.Get();
    }
    else if (iid == IID_ID3D11DeviceContext)
    {
		*ppvObject = pContext.Get();
    }
    else if (iid == IID_IDXGISwapChain)
    {
        *ppvObject = pSwapChain.Get();
    }
	else if (iid == IID_IDXGIDevice)
	{
		*ppvObject = pDxgiDevice.Get();
	}
	else if (iid == IID_IDXGIAdapter)
	{
		*ppvObject = pDxgiAdapter.Get();
	}
	else if (iid == IID_IDXGIFactory)
	{
		*ppvObject = pDxgiFactory.Get();
	}
	else if (iid == IID_ID2D1Factory)
	{
		*ppvObject = pD2DFactory.Get();
	}
	else if (iid == IID_ID2D1RenderTarget)
	{
		*ppvObject = pD2DRenderTarget.Get();
	}
	else if (iid == __uuidof(IDWriteFactory))
	{
		*ppvObject = pDWriteFactory.Get();
	}
    else
    {
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }
	static_cast<IUnknown*>(*ppvObject)->AddRef();
    return S_OK;
}

XMMATRIX Renderer::GetTextureMatrix()
{
	return matTexture;
}

XMMATRIX Renderer::GetWorldMatrix()
{
	return matWorld;
}

XMMATRIX Renderer::GetViewMatrix()
{
	return matView;
}

XMMATRIX Renderer::GetProjectionMatrix()
{
	return matProj;
}

XMMATRIX Renderer::GetWVPMatrix()
{
	return matWVP;
}

void Renderer::SetTextureMatrix(const XMMATRIX& mat)
{
	matTexture = mat;
}

void Renderer::SetWorldMatrix(const XMMATRIX& mat)
{
	matWorld = mat;
	matWVP = matWorld * matView * matProj;
}

void Renderer::SetViewMatrix(const XMMATRIX& mat)
{
	matView = mat;
	matWVP = matWorld * matView * matProj;
}

void Renderer::SetProjectionMatrix(const XMMATRIX& mat)
{
	matProj = mat;
	matWVP = matWorld * matView * matProj;
}

void Renderer::StartRender()
{
	for (auto e : engine->eventCallBacks)
		e->OnStartRender();
}

bool Renderer::GetWindowed()
{
	return isWindowed;
}

void Renderer::SetWindowed(bool windowed, SCALING_MODE scalingMode)
{
	switchModeIsSignalled = true;
	this->setWindowed = windowed;
	this->scalingMode = scalingMode;
}

void Renderer::SendShaderData(UINT type, LPVOID data1, LPVOID data2)
{
	for (auto e : engine->eventCallBacks)
		e->RecvShaderData(type, data1, data2);
}

USize Renderer::GetBufferSize()
{
	return bufferSize;
}

void Renderer::SetBlendState(BLEND_STATE blendState)
{
	switch (blendState)
	{
	case BLEND_STATE_NORMAL:
		pContext->OMSetBlendState(mBlendStateNormal.Get(), 0, 0xffffffff);
		break;
	case BLEND_STATE_BRIGHTEN:
		pContext->OMSetBlendState(mBlendStateBrighten.Get(), 0, 0xffffffff);
		break;
	case BLEND_STATE_DARKEN:
		pContext->OMSetBlendState(mBlendStateDarken.Get(), 0, 0xffffffff);
		break;
	case BLEND_STATE_INVDEST:
		pContext->OMSetBlendState(mBlendStateInvDest.Get(), 0, 0xffffffff);
		break;
	}
}

IRenderTarget* Renderer::GetRenderTarget()
{
	return pRenderTarget.Get();
}

void Renderer::SetRenderTarget(IRenderTarget* pRT)
{
	if (!pRT)
	{
		pContext->OMSetRenderTargets(1, mRenderTargetRTView.GetAddressOf(), mDepthStencilView.Get());
		pD2DRenderTarget = mD2DRenderTarget;
		pRenderTarget = mRenderTarget;
	}
	else
	{
		ComPtr<ID3D11RenderTargetView> rtView;
		ComPtr<ID3D11DepthStencilView> depthView;
		pRT->GetBuffer(IID_ID3D11RenderTargetView, &rtView);
		pRT->GetBuffer(IID_ID3D11DepthStencilView, &depthView);
		pContext->OMSetRenderTargets(1, rtView.GetAddressOf(), depthView.Get());
		ComPtr<ID2D1RenderTarget> d2drt;
		pRT->GetBuffer(IID_ID2D1RenderTarget, &d2drt);
		if (d2drt)
		{
			pD2DRenderTarget = d2drt;
		}
		pRenderTarget = pRT;
	}
	USize size = pRenderTarget->GetSize();
	D3D11_VIEWPORT vp;
	vp.Width = size.width;
	vp.Height = size.height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1, &vp);
}

void Renderer::ClearRenderTarget(IRenderTarget* pRT, const Color& color)
{
	if (!pRT)
	{
		pContext->ClearRenderTargetView(mRenderTargetRTView.Get(), (float*)&color);
		pContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
	else
	{
		ComPtr<ID3D11RenderTargetView> rtView;
		ComPtr<ID3D11DepthStencilView> depthView;
		pRT->GetBuffer(IID_ID3D11RenderTargetView, &rtView);
		pRT->GetBuffer(IID_ID3D11DepthStencilView, &depthView);
		pContext->ClearRenderTargetView(rtView.Get(), (float*)&color);
		if(depthView)
			pContext->ClearDepthStencilView(depthView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}

void Renderer::SetRenderTargetTreeView(IRenderTargetTreeView* pView)
{
	mRenderTargetTreeView = pView;
}

IRenderTargetTreeView* Renderer::GetRenderTargetTreeView()
{
	return mRenderTargetTreeView.Get();
}

void Renderer::Invoke(IDelegate* pDelegate)
{
	if (engine->IsValid())
	{
		WaitForSingleObject(rendererDelegateMutex, INFINITE);
		rendererDelegates.emplace_back(pDelegate);
		ReleaseMutex(rendererDelegateMutex);
	}
	else
	{
		pDelegate->AddRef();
		pDelegate->Release();
	}
}