#include"EngineBackend.h"

namespace ENGINE {
	namespace RENDERER {

		void EngineBackend::Init(Window::WindowInitData initData) {
			window.Init(initData);
			device.Init(GetHwnd());
		}

		void EngineBackend::Shutdown() {
			window.Shutdown();
		}

		RenderContext* EngineBackend::GetRenderContext() {
			return new RenderContext(device.GetContext(), window.GetInitData().width, window.GetInitData().height);
		}

		bool EngineBackend::Run() {
			window.Run();
			return window.isRunning;
		}

		void EngineBackend::PresentFrame() {
			device.GetSwapChain()->Present(1, 0);
		}

		void EngineBackend::CreateRenderTargetBackBuffer() {
			ID3D11Texture2D* pBackBuffer = nullptr;
			HRESULT hr = device.GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
			CHECK_DXHR(hr, "Failed to retrieve backbuffer");
			backRenderTarget = new RenderTarget(device.GetDevice(), pBackBuffer);
		}

		void EngineBackend::CreateRenderTargetTexture() {
			customRenderTarget = new RenderTarget(GetDevice(), window.GetInitData().width, window.GetInitData().height, DXGI_FORMAT_B8G8R8A8_UNORM);
		}
	}
}