#pragma once

#include<d3d11.h>
#include<DirectXMath.h>
#include"Engine/Core/ErrorHandler.h"
#include"Engine/Core/Math.h"
#include"Engine/Render/RenderContext.h"

namespace ENGINE {
	namespace RENDERER {
		class FrameDataBuffer {
		public:
			struct FrameData {
				DirectX::XMMATRIX view;
				DirectX::XMMATRIX projection;
				MATH::float4 lightDirection;
				MATH::float4 cameraPosition;
				MATH::float4 timeData;
			};

			void Init(ID3D11Device* device) {
				HRESULT hr;

				D3D11_BUFFER_DESC cbd = {};
				cbd.Usage = D3D11_USAGE_DYNAMIC;
				cbd.ByteWidth = sizeof(FrameData);
				cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
				cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

				hr = device->CreateBuffer(&cbd, nullptr, &m_Buffer);

				CHECK_DXHR(hr, "Failed to create global buffer.");
			}

			void UpdateAndBindData(FrameData frameData, RenderContext* renderContext) {
				renderContext->UpdateMappedSubresource(m_Buffer, &frameData, sizeof(FrameData));
			}

			ID3D11Buffer* GetBuffer() { return m_Buffer; }

		private:
			ID3D11Buffer* m_Buffer = nullptr;
		};
	}
}