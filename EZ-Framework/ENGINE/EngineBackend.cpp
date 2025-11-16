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