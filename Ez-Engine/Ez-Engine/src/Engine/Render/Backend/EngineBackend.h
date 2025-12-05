#pragma once

#include"Window.h"
#include"Engine/Render/RenderTarget.h"
#include"Engine/Render/RenderContext.h"
#include"Engine/Render/Backend/GraphicsDevice.h"

namespace ENGINE {
	namespace RENDERER {

		class EngineBackend {
		public:
			EngineBackend() = default;

			void Init(Window::WindowInitData windowInitData);
			void Shutdown();
			bool Run();
			void PresentFrame();

			void CreateRenderTargetBackBuffer();
			void CreateRenderTargetTexture();

			RenderTarget* GetBackRenderTarget() { return backRenderTarget; }
			RenderTarget* GetCustomRenderTarget() { return customRenderTarget; }

			HWND GetHwnd() const noexcept { return window.GetHwnd(); }
			ID3D11Device* GetDevice() { return device.GetDevice(); }
			GraphicsDevice* GetGraphicDevice() { return &device; }
			RenderContext* GetRenderContext();

			Window::WindowInitData& GetWindowInitData() { return window.GetInitData(); }

			Input* GetInput() { return window.GetInput(); }

		private:
			Window window;
			GraphicsDevice device;

			RenderTarget* backRenderTarget = nullptr;
			RenderTarget* customRenderTarget = nullptr;
		};

	}
}