#ifndef GRAPHICS_CLASS_H
#define GRAPHICS_CLASS_H

#include<d3d11.h>
#include<dxgi.h>
#include<d3dcompiler.h>
#include<wrl/client.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include"ErrorHandler.h"

class Graphics {
public:
	Graphics(HWND hWnd);
	~Graphics();
	
	void SetRenderTargetView();
	void SetViewport();

	void InitFrame();
	void PresentFrame();
	void Render();

	ID3D11Device* GetDevice() const { return m_pd3dDevice; }
	ID3D11DeviceContext* GetContext() const { return m_pImmediateContext; }
private:
	D3D_FEATURE_LEVEL FeatureLevel;
	IDXGISwapChain* m_pSwapChain;
	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_pImmediateContext;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilState* m_depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	ID3D11RasterizerState* rasterState;

	const int WIDTH = 600;
	const int HEIGHT = 600;

private:
	bool CreateSwapChain(HWND hWnd);
	void CreateDepthStencilBuffer();
	void CreateRasterizerState();
	void CreateRenderTargetView();
};

#endif