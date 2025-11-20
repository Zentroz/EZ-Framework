#include"Engine/EngineBackend.h"

void EngineBackend::Init(int width, int height, const char* title) {
	window.Init(width, height, title);
	device.Init(GetHwnd());

	ID3D11Texture2D* pBackBuffer = nullptr;
    HRESULT hr = device.GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
	CHECK_DXHR(hr, "Failed to retrieve backbuffer");
	backRenderTarget = new RenderTarget(device.GetDevice(), pBackBuffer);
}

void EngineBackend::Shutdown() {
	window.Shutdown();
}

ID3D11DepthStencilState* EngineBackend::CreateDepthStencilState(D3D11_COMPARISON_FUNC func) {
	ID3D11DepthStencilState* state;

	// Creating depth stencil state
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = func;
	dsDesc.StencilEnable = FALSE;
	CHECK_DXHR(device.GetDevice()->CreateDepthStencilState(&dsDesc, &state), "Failed to create depth stencil state");

	return state;
}

ID3D11RasterizerState* EngineBackend::CreateRasteriserState(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode) {
	ID3D11RasterizerState* state = nullptr;

	HRESULT hr;

	// Defining rasterizer state description
	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.FillMode = fillMode;
	rasterDesc.CullMode = cullMode;
	rasterDesc.FrontCounterClockwise = FALSE;
	rasterDesc.DepthClipEnable = TRUE;

	hr = device.GetDevice()->CreateRasterizerState(&rasterDesc, &state);

	CHECK_DXHR(hr, "Failed to create rasterizer");

	return state;
}

RenderContext* EngineBackend::GetRenderContext() {
	return new RenderContext(device.GetContext());
}

bool EngineBackend::Run() {
	window.Run();
	return window.isRunning;
}

void EngineBackend::PresentFrame() {
	device.GetSwapChain()->Present(1, 0);
}