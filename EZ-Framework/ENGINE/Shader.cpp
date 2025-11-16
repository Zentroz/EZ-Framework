#include"Engine/Shader.h"

Shader::Shader(const char* name, const char* vertexEntryPoint, const char* pixelEntryPoint) noexcept : vsBlob(nullptr), psBlob(nullptr), errBlob(nullptr) {
	this->name = name;
	this->vertexEntryPoint = vertexEntryPoint;
	this->pixelEntryPoint = pixelEntryPoint;
}

void Shader::CompileFromFile(const char* filePath, ID3D11Device* device) {
	std::string file(filePath);
	std::wstring wfile(file.begin(), file.end());

	HRESULT hr;

	hr = D3DCompileFromFile(wfile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, vertexEntryPoint, "vs_5_0", 0, 0, &vsBlob, &errBlob);
	CHECK_SHADER(hr, errBlob, name);

	hr = D3DCompileFromFile(wfile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, pixelEntryPoint, "ps_5_0", 0, 0, &psBlob, &errBlob);
	CHECK_SHADER(hr, errBlob, name);

	hr = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader);
	CHECK_DXHR(hr, (std::string("Failed to create vertex shader. SHADER: ") + name).c_str());

	hr = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader);
	CHECK_DXHR(hr, (std::string("Failed to create pixel shader. SHADER: ") + name).c_str());

	CreateInputLayout(device);
}

void Shader::CompileFromSource(const char* vs_src, const char* ps_src, ID3D11Device* device) {
	HRESULT hr;

	// Compile vertex shader
	hr = D3DCompile(vs_src, strlen(vs_src), nullptr, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, vertexEntryPoint, "vs_5_0", 0, 0, &vsBlob, &errBlob);
	CHECK_SHADER(hr, errBlob, "Failed to compile vertex shader");

	// Compile pixel shader
	hr = D3DCompile(ps_src, strlen(ps_src), nullptr, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, pixelEntryPoint, "ps_5_0", 0, 0, &psBlob, &errBlob);
	CHECK_SHADER(hr, errBlob, "Failed to compile pixel shader");

	hr = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader);
	CHECK_DXHR(hr, (std::string("Failed to create vertex shader. SHADER: ") + name).c_str());

	hr = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader);
	CHECK_DXHR(hr, (std::string("Failed to create pixel shader. SHADER: ") + name).c_str());

	CreateInputLayout(device);
}

void Shader::CreateInputLayout(ID3D11Device* device) {
	HRESULT hr;

	// Input layout
	D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	hr = device->CreateInputLayout(layoutDesc, ARRAYSIZE(layoutDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
	CHECK_DXHR(hr, "Failed to create input layout");
}

ID3DBlob* Shader::GetVertexShaderBlob() const {
	return vsBlob;
}

ID3DBlob* Shader::GetPixelShaderBlob() const {
	return psBlob;
}

void Shader::ReleaseBlobs() {
	if (vsBlob) vsBlob->Release();
	if (psBlob) psBlob->Release();
	if (errBlob) errBlob->Release();
}

void Shader::Bind(ID3D11DeviceContext* context) {
	context->IASetInputLayout(inputLayout.Get());

	context->VSSetShader(vertexShader.Get(), nullptr, 0);
	context->PSSetShader(pixelShader.Get(), nullptr, 0);
}