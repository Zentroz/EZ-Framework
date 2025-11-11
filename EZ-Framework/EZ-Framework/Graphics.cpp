#include"Graphics.h"

Graphics::Graphics(HWND hWnd) {
	CreateSwapChain(hWnd);
    CreateRenderTargetView();
}

Graphics::~Graphics() {
    if (m_pSwapChain) m_pSwapChain->Release();
    if (m_pd3dDevice) m_pd3dDevice->Release();
	if (m_pImmediateContext) m_pImmediateContext->Release();
}

bool Graphics::CreateSwapChain(HWND hWnd) {
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = 600;
    sd.BufferDesc.Height = 600;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    D3D_FEATURE_LEVEL FeatureLevels = D3D_FEATURE_LEVEL_11_0;

    D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        0,
        &FeatureLevels,
        1,
        D3D11_SDK_VERSION,
        &sd,
        &m_pSwapChain,
        &m_pd3dDevice,
        &FeatureLevel,
        &m_pImmediateContext
    );

    return true;
}

void Graphics::CreateRenderTargetView() {
    // Creating render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

    hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);

    pBackBuffer->Release();
}

void Graphics::SetRenderTargetView() {
    // Set render target and depth buffer
    m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);
}


void Graphics::SetViewport() {
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast<FLOAT>(600);
    viewport.Height = static_cast<FLOAT>(600);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    m_pImmediateContext->RSSetViewports(1, &viewport);
}

void Graphics::InitFrame() {
    float clearColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };

    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);
}

void Graphics::PresentFrame() {
    m_pSwapChain->Present(1, 0);
}

void Graphics::Render() {
    SetRenderTargetView();
    InitFrame();
    SetViewport();

    PresentFrame();
}