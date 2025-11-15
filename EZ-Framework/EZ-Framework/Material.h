#ifndef MATERIAL_CLASS_H
#define MATERIAL_CLASS_H

#include<unordered_map>
#include"Shader.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

class Material {
public:
	Material(Shader* shader);

	void SetFloat(const std::string& name, float value);

	void CreateMaterialBuffer(ID3D11Device* device);
	void ReflectShaderVariables();
	void Upload(ID3D11DeviceContext* context);
	void Bind(ID3D11DeviceContext* context);

private:
	struct ShaderVariable {
		UINT offset;
		UINT size;
	};

	struct MaterialConstantBuffer {
		float test;
		float padding[3];
	};

	bool isDirty;

	std::unordered_map<std::string, ShaderVariable> variables;
	std::vector<uint8_t> bufferData;
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;

	Shader* shader;
};

#endif