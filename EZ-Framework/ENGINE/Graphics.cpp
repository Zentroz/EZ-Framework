#include"Engine/Graphics.h"

Graphics::Graphics() : 
    FeatureLevel(), 
    m_pd3dDevice(nullptr), 
    m_pSwapChain(nullptr), 
    m_pImmediateContext(nullptr), 
    m_pRenderTargetView(nullptr), 
    m_depthStencilState(nullptr), 
    m_depthStencilView(nullptr), 
    rasterState(nullptr) {
}

void Graphics::Init(HWND hWnd) {
    CreateSwapChain(hWnd);
    CreateRenderTargetView();
    CreateDepthStencilBuffer();
    CreateRasterizerState();
}

void Graphics::Shutdown() {
    if (m_pImmediateContext) m_pImmediateContext->ClearState();
    //if (m_pSwapChain) m_pSwapChain->SetFullscreenState(FALSE, NULL);
    if (m_pSwapChain) m_pSwapChain->Release();
    if (m_pd3dDevice) m_pd3dDevice->Release();
    if (m_pImmediateContext) m_pImmediateContext->Release();
}

bool Graphics::CreateSwapChain(HWND hWnd) {
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = WIDTH;
    sd.BufferDesc.Height = HEIGHT;
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

void Graphics::CreateDepthStencilBuffer() {
    // Creating depth stencil texture
    ID3D11Texture2D* pDepthStencil = nullptr;
    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = WIDTH;
    descDepth.Height = HEIGHT;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    CHECK_DXHR(m_pd3dDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil), "Failed to create depth texture");

    // Creating depth stencil view
    if (pDepthStencil != 0) CHECK_DXHR(m_pd3dDevice->CreateDepthStencilView(pDepthStencil, nullptr, m_depthStencilView.GetAddressOf()), "Failed to create depth stencil view");
    else EXCEPTION("DepthStencil is nullptr!");

    pDepthStencil->Release();

    // Creating depth stencil state
    D3D11_DEPTH_STENCIL_DESC dsDesc = {};
    dsDesc.DepthEnable = TRUE;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    dsDesc.StencilEnable = FALSE;
    CHECK_DXHR(m_pd3dDevice->CreateDepthStencilState(&dsDesc, &m_depthStencilState), "Failed to create depth stencil state");
}

void Graphics::CreateRasterizerState() {
    HRESULT hr;

    // Defining rasterizer state description
    D3D11_RASTERIZER_DESC rasterDesc = {};
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.FrontCounterClockwise = FALSE;
    rasterDesc.DepthClipEnable = TRUE;

    hr = m_pd3dDevice->CreateRasterizerState(&rasterDesc, &rasterState);

    CHECK_DXHR(hr, "Failed to create rasterizer");

    m_pImmediateContext->RSSetState(rasterState);
}


void Graphics::CreateRenderTargetView() {
    HRESULT hr;

    // Creating render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

    CHECK_DXHR(hr, "Failed to create back buffer!");

    if (pBackBuffer != 0) {
        hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
        CHECK_DXHR(hr, "Failed to create RenderTargetView!");
    }

    pBackBuffer->Release();
}

void Graphics::SetRenderTargetView() {
    // Set render target and depth buffer
    m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_depthStencilView.Get());
    m_pImmediateContext->OMSetDepthStencilState(m_depthStencilState, 0);
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
    m_pImmediateContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    SetRenderTargetView();
    SetViewport();

    m_pImmediateContext->RSSetState(rasterState);
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