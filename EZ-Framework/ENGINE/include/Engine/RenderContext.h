#ifndef RENDERCONTEXT_CLASS_H
#define RENDERCONTEXT_CLASS_H

#include"d3d11.h"

#include"Engine/Resources.h"

class RenderContext {
public:
    RenderContext(ID3D11DeviceContext* ctx);

    void SetRenderTarget(ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthView);
    void SetDepthStencilState(ID3D11DepthStencilState* state);
    void ClearRenderTarget(ID3D11RenderTargetView* renderTarget, float4 color);
    void SetShaderResource();
    void ClearDepthStencilView(ID3D11DepthStencilView* view);
    void SetTopology();
    void SetRasteriser(ID3D11RasterizerState* state);
    void SetViewport();
    void SetSamplerState(UINT slot, ID3D11SamplerState* const* samplerState);
    void SetPSShaderResource(UINT slot, ID3D11ShaderResourceView* const* srv);
    void SetShader(Shader* shader);
    void SetInputLayout(ID3D11InputLayout* layout);
    void UpdateMappedSubresource(ID3D11Buffer* buffer, void* data, size_t dataSize);
    void UpdateSubresource(ID3D11Buffer* buffer, const void* data);
    void SetVSConstantBuffer(ID3D11Buffer* buffer, UINT slot);
    void SetPSConstantBuffer(ID3D11Buffer* buffer, UINT slot);
    void SetFloat(const std::string& name, float value);
    void DrawMesh(Mesh* mesh);
    void DrawIndexed(uint32_t indexCount);

private:
	ID3D11DeviceContext* ctx;
};

#endif