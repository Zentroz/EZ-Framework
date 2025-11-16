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

	RenderTarget* GetMainRenderTarget() { return backRenderTarget; }

	HWND GetHwnd() const noexcept { return window.GetHwnd(); }
	ID3D11Device* GetDevice() { return device.GetDevice(); }
	RenderContext* GetRenderContext();

private:
	Input input;
	Window window;
	GraphicsDevice device;

	RenderTarget* backRenderTarget;
};