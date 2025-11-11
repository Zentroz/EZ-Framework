#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<d3d11.h>
#include<d3dcompiler.h>
#include<wrl/client.h>

#include"ErrorHandler.h"

class Shader {
public:
	Shader(const char* name, const char* vertexEntryPoint = "VSMain", const char* pixelEntryPoint = "PSMain") noexcept;
	void CompileFromFile(const char* filePath, ID3D11Device* device);
	void CompileFromSource(const char* vs_src, const char* ps_src, ID3D11Device* device);
	void Bind(ID3D11DeviceContext* context);

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	const char* name;
	const char* vertexEntryPoint;
	const char* pixelEntryPoint;

private:
	void CreateInputLayout(ID3DBlob* vsBlob, ID3D11Device* device);
};

#endif