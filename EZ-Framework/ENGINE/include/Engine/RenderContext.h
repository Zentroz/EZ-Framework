#ifndef RENDERCONTEXT_CLASS_H
#define RENDERCONTEXT_CLASS_H

#include"d3d11.h"

#include"Engine/Mesh.h"
#include"Engine/Shader.h"
#include"Engine/Mesh.h"

class RenderContext {
public:
    RenderContext(ID3D11DeviceContext* ctx);

    void SetRenderTarget(ID3D11RenderTargetView* renderTarget);
    void ClearRenderTarget(ID3D11RenderTargetView* renderTarget, float4 color);
    void SetTopology();
    void SetShader(Shader* shader);
    void SetConstantBuffer(ID3D11Buffer* buffer, const void* data);
    void SetVSConstantBuffer(ID3D11Buffer* buffer, UINT slot);
    void SetPSConstantBuffer(ID3D11Buffer* buffer, UINT slot);
    void SetFloat(const std::string& name, float value);
    void DrawMesh(Mesh* mesh);
    void DrawIndexed(uint32_t indexCount);

private:
	ID3D11DeviceContext* ctx;
};

#endif