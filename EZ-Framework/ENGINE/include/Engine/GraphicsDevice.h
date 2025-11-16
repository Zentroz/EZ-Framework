#ifndef GRAPHICS_DEVICE_CLASS_H
#define GRAPHICS_DEVICE_CLASS_H

#include<d3d11.h>
#include<dxgi.h>
#include<d3dcompiler.h>
#include<wrl/client.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

class GraphicsDevice {
public:
    GraphicsDevice() = default;

    void Init(HWND hwnd);
    ID3D11Device* GetDevice() const { return device; }
    ID3D11DeviceContext* GetContext() const { return context; }
    IDXGISwapChain* GetSwapChain() const { return swapChain; }

private:
    D3D_FEATURE_LEVEL FeatureLevel;
    IDXGISwapChain* swapChain;
    ID3D11Device* device;
    ID3D11DeviceContext* context;

    const int WIDTH = 600;
    const int HEIGHT = 600;
};

#endif