#ifndef PIPELINE_STATE_MANAGER_H
#define PIPELINE_STATE_MANAGER_H

#include<d3d11.h>

#include"Engine/Core/ErrorHandler.h"

class PipelineStateManager {
public:
    // Rasteriser States
    ID3D11RasterizerState* wireframe = nullptr;
    ID3D11RasterizerState* solid = nullptr;

    // Depth Stencil States
    ID3D11DepthStencilState* depthRead = nullptr;
    ID3D11DepthStencilState* depthWrite = nullptr;

    // Blend States
    ID3D11BlendState* alphaBlend = nullptr;

    // Sampler States
    Microsoft::WRL::ComPtr<ID3D11SamplerState> pointSampler;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> linerSampler;

    void Init(ID3D11Device* device) {
        HRESULT hr;

        // Rasterizer
        D3D11_RASTERIZER_DESC wireFrameDesc;
        ZeroMemory(&wireFrameDesc, sizeof(wireFrameDesc));
        wireFrameDesc.CullMode = D3D11_CULL_BACK;
        wireFrameDesc.FillMode = D3D11_FILL_WIREFRAME;
        wireFrameDesc.FrontCounterClockwise = FALSE;
        wireFrameDesc.DepthClipEnable = TRUE;

        hr = device->CreateRasterizerState(&wireFrameDesc, &wireframe);
        CHECK_DXHR(hr, "Failed to create Wireframe State!");

        D3D11_RASTERIZER_DESC solidDesc;
        ZeroMemory(&solidDesc, sizeof(solidDesc));
        solidDesc.CullMode = D3D11_CULL_BACK;
        solidDesc.FillMode = D3D11_FILL_SOLID;
        solidDesc.FrontCounterClockwise = FALSE;
        solidDesc.DepthClipEnable = TRUE;

        hr = device->CreateRasterizerState(&solidDesc, &solid);
        CHECK_DXHR(hr, "Failed to create Solid State!");

        // Depth Stencil
        D3D11_DEPTH_STENCIL_DESC depthReadDesc;
        ZeroMemory(&depthReadDesc, sizeof(depthReadDesc));
        depthReadDesc.DepthEnable = TRUE;
        depthReadDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        depthReadDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
        depthReadDesc.StencilEnable = FALSE;

        hr = device->CreateDepthStencilState(&depthReadDesc, &depthRead);
        CHECK_DXHR(hr, "Failed to create Depth Read State!");

        D3D11_DEPTH_STENCIL_DESC depthWriteDesc;
        ZeroMemory(&depthWriteDesc, sizeof(depthWriteDesc));
        depthWriteDesc.DepthEnable = TRUE;
        depthWriteDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthWriteDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
        depthWriteDesc.StencilEnable = FALSE;

        hr = device->CreateDepthStencilState(&depthWriteDesc, &depthWrite);
        CHECK_DXHR(hr, "Failed to create Depth Write State!");

        // Sampler
        D3D11_SAMPLER_DESC linearSDesc;
        linearSDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        linearSDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        linearSDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        linearSDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

        device->CreateSamplerState(&linearSDesc, &linerSampler);

        D3D11_SAMPLER_DESC pointSDesc;
        pointSDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        pointSDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        pointSDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        pointSDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

        device->CreateSamplerState(&pointSDesc, &pointSampler);
    }

    void Destory() {
        if (wireframe) {
            delete wireframe;
        }
        if (solid) {
            delete solid;
        }

        if (depthRead) {
            delete depthRead;
        }
        if (depthWrite) {
            delete depthWrite;
        }
        if (alphaBlend) {
            delete alphaBlend;
        }
    }
};

#endif