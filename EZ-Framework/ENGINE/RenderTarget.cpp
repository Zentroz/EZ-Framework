#include"Engine/RenderTarget.h"

RenderTarget::RenderTarget(ID3D11Device* device, int width, int height, DXGI_FORMAT format)
    : m_width(width), m_height(height)
{
    // Create texture description
    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = format;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    // Create the texture
    HRESULT hr = device->CreateTexture2D(&textureDesc, nullptr, &m_texture);

    CHECK_DXHR(hr, "Failed to create render target texture");

    // Create render target view
    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.Format = format;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Texture2D.MipSlice = 0;

    hr = device->CreateRenderTargetView(m_texture.Get(), &rtvDesc, &m_rtv);
    CHECK_DXHR(hr, "Failed to create render target view");

    // Create shader resource view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;

    hr = device->CreateShaderResourceView(m_texture.Get(), &srvDesc, &m_srv);
    CHECK_DXHR(hr, "Failed to create shader resource view");
}

RenderTarget::RenderTarget(ID3D11Device* device, ID3D11Texture2D* texture) {
    D3D11_TEXTURE2D_DESC desc;
    texture->GetDesc(&desc);

    m_width = desc.Width;
    m_height = desc.Height;

    if (texture != 0) {
        HRESULT hr = device->CreateRenderTargetView(texture, nullptr, &m_rtv);
        CHECK_DXHR(hr, "Failed to create RenderTargetView!");
    }

    texture->Release();
}

RenderTarget::~RenderTarget() {}

void RenderTarget::Clear(RenderContext* ctx, float4 color) {
    ctx->ClearRenderTarget(m_rtv.Get(), color);
}