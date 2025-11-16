#include"Engine/Material.h"

Material::Material(Shader* shader) : shader(shader), bufferData(sizeof(MaterialConstantBuffer)) {
	isDirty = true;
}

void Material::SetFloat(const std::string& name, float value) {
	if (!variables.contains(name)) return;
	const auto& var = variables[name];
	memcpy(&bufferData[var.offset], &value, sizeof(float));

	isDirty = true;
}

void Material::CreateMaterialBuffer(ID3D11Device* device) {
	HRESULT hr;

	D3D11_BUFFER_DESC cbd = {};
	//cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.ByteWidth = sizeof(MaterialConstantBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	hr = device->CreateBuffer(&cbd, nullptr, &constantBuffer);

	CHECK_DXHR(hr, "Failed to create Material Buffer!");
}

void Material::ReflectShaderVariables() {
	ID3DBlob* psBlob = shader->GetPixelShaderBlob();

	Microsoft::WRL::ComPtr<ID3D11ShaderReflection> reflector;

	D3DReflect(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), IID_ID3D11ShaderReflection, &reflector);

	D3D11_SHADER_DESC shaderDesc;
	reflector->GetDesc(&shaderDesc);

	for (UINT i = 0; i < shaderDesc.ConstantBuffers; ++i) {
		ID3D11ShaderReflectionConstantBuffer* cb = reflector->GetConstantBufferByIndex(i);
		D3D11_SHADER_BUFFER_DESC cbDesc;
		cb->GetDesc(&cbDesc);

		if (strcmp(cbDesc.Name, "MaterialConstantBuffer") == 0) {
			for (UINT v = 0; v < cbDesc.Variables; ++v)
			{
				ID3D11ShaderReflectionVariable* var = cb->GetVariableByIndex(v);
				D3D11_SHADER_VARIABLE_DESC varDesc;
				var->GetDesc(&varDesc);

				ShaderVariable shaderVar;
				shaderVar.offset = varDesc.StartOffset;
				shaderVar.size = varDesc.Size;

				variables[varDesc.Name] = shaderVar;
			}
		}
	}

	shader->ReleaseBlobs();
}

void Material::Upload(ID3D11DeviceContext* context) {
	if (!isDirty) return;

	HRESULT hr;

	D3D11_MAPPED_SUBRESOURCE mapped;
	hr = context->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

	CHECK_DXHR(hr, "Failed to map constant buffer!");

	memcpy(mapped.pData, bufferData.data(), bufferData.size());

	context->Unmap(constantBuffer.Get(), 0);

	isDirty = false;
}

void Material::Bind(ID3D11DeviceContext* context) {
	context->PSSetConstantBuffers(1, 1, constantBuffer.GetAddressOf());
}