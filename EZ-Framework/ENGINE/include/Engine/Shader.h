#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include"Core/ErrorHandler.h"

#include<d3d11.h>
#include<d3dcompiler.h>
#include<d3d11shader.h>
#include<wrl/client.h>

class Shader {
public:
	Shader() = default;
	Shader(const char* name, const char* vertexEntryPoint = "VSMain", const char* pixelEntryPoint = "PSMain") noexcept;

	void CompileFromFile(const char* filePath, ID3D11Device* device);
	void CompileFromSource(const char* vs_src, const char* ps_src, ID3D11Device* device);
	void ReleaseBlobs();
	void Bind(ID3D11DeviceContext* context);

	ID3DBlob* GetVertexShaderBlob() const;
	ID3DBlob* GetPixelShaderBlob() const;

	ID3D11VertexShader* GetVertexShader() const { return vertexShader.Get(); }
	ID3D11PixelShader* GetPixelShader() const { return pixelShader.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	// Blobs
	ID3DBlob* vsBlob;
	ID3DBlob* psBlob;
	ID3DBlob* errBlob;

	// Shader info
	const char* name;
	const char* vertexEntryPoint;
	const char* pixelEntryPoint;

private:
	void CreateInputLayout(ID3D11Device* device);
};

#endif