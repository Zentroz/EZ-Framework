#ifndef RENDER_TARGET_CLASS_H 
#define RENDER_TARGET_CLASS_H

#include<d3d11.h>
#include<wrl/client.h>
#include"Engine/RenderContext.h"

class RenderTarget {
public:
    RenderTarget(ID3D11Device* device, int width, int height, DXGI_FORMAT format);
    RenderTarget(ID3D11Device* device, ID3D11Texture2D* texture);
    ~RenderTarget();

    //void Resize(int newWidth, int newHeight);
    void Clear(RenderContext* ctx, float4 color);

    ID3D11RenderTargetView* GetRTV() const { return m_rtv.Get(); }
    ID3D11ShaderResourceView* GetSRV() const { return m_srv.Get(); }

private:
    Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;
    int m_width, m_height;
};

#endif