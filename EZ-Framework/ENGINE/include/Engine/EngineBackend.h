#pragma once

#include"Window.h"
#include"Engine/GraphicsDevice.h"
#include"Engine/RenderContext.h"
#include"Engine/RenderTarget.h"

class EngineBackend {
public:
	EngineBackend() = default;

	void Init(int width, int height, const char* title);
	void Shutdown();
	bool Run();
	void PresentFrame();

	ID3D11DepthStencilState* CreateDepthStencilState(D3D11_COMPARISON_FUNC func);
	ID3D11RasterizerState* CreateRasteriserState(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode);
	RenderTarget* GetMainRenderTarget() { return backRenderTarget; }

	HWND GetHwnd() const noexcept { return window.GetHwnd(); }
	ID3D11Device* GetDevice() { return device.GetDevice(); }
	RenderContext* GetRenderContext();

	Input* GetInput() { return window.GetInput(); }

private:
	Window window;
	GraphicsDevice device;

	RenderTarget* backRenderTarget = nullptr;
};