#include"Engine/ResourceManager.h"

Microsoft::WRL::ComPtr<ID3D11DepthStencilView> ResourceManager::CreateDepthStencilBuffer() {
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> view = nullptr;

    // Creating depth stencil texture
    ID3D11Texture2D* pDepthStencil = nullptr;
    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = 600;
    descDepth.Height = 600;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    CHECK_DXHR(device->CreateTexture2D(&descDepth, nullptr, &pDepthStencil), "Failed to create depth texture");

    // Creating depth stencil view
    if (pDepthStencil != 0) CHECK_DXHR(device->CreateDepthStencilView(pDepthStencil, nullptr, view.GetAddressOf()), "Failed to create depth stencil view");
    else EXCEPTION("DepthStencil is nullptr!");

    pDepthStencil->Release();

    return view;
}