#ifndef RESOURCE_CLASSES_H
#define RESOURCE_CLASSES_H

#include<vector>

// Shader
#include<d3d11.h>
#include<d3dcompiler.h>
#include<d3d11shader.h>
#include<wrl/client.h>

#include"Engine/Core/Math.h"
#include"Engine/Core/Logger.h"
#include"Engine/Core/ErrorHandler.h"
#include"Engine/Assets/Assets.h"
#include"Engine/Utils/EUID.h"

using namespace MATH;

namespace ENGINE {
	namespace RENDERER {

		class Resource {
		public:
			EUID euid;

			virtual ~Resource() = default;
			virtual bool LoadFromAsset(std::shared_ptr<ASSET::Asset> asset, ID3D11Device* device) = 0;
		};

		class Shader : public Resource {
		public:
			~Shader();
			bool LoadFromAsset(std::shared_ptr<ASSET::Asset> asset, ID3D11Device* device) override;

			ID3D11VertexShader* GetVertexShader() const { return vertexShader; }
			ID3D11PixelShader* GetPixelShader() const { return pixelShader; }
			ID3D11InputLayout* GetInputLayout() const { return inputLayout; }

		private:
			ID3D11VertexShader* vertexShader;
			ID3D11PixelShader* pixelShader;
			ID3D11InputLayout* inputLayout;
		};

		class Mesh : public Resource {
		public:
			~Mesh();
			bool LoadFromAsset(std::shared_ptr<ASSET::Asset> asset, ID3D11Device* device) override;

			ID3D11Buffer* GetVertexBuffer() { return vertexBuffer; }
			ID3D11Buffer* GetIndexBuffer() { return indexBuffer; }

			UINT GetIndexCount() const { return indexCount; }
			UINT GetOffset() { return 0; }
			UINT GetStride() { return sizeof(Vertex); }
		private:
			ID3D11Buffer* vertexBuffer;
			ID3D11Buffer* indexBuffer;
			UINT indexCount;
		};

		class Texture : public Resource {
		public:
			~Texture();
			bool LoadFromAsset(std::shared_ptr<ASSET::Asset> asset, ID3D11Device* device) override;

			ID3D11ShaderResourceView* GetSRV() { return m_textureSRV.Get(); }
			ID3D11ShaderResourceView* const* GetSRVAddress() { return m_textureSRV.GetAddressOf(); }
		private:
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureSRV;

		};

	}
}

#endif