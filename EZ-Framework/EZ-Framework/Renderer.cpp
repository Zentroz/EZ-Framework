#include"Renderer.h"

// HLSL source for a trivial vertex+pixel shader
const char* g_vs_src =
"struct VSIn { float3 pos : POSITION; };\n"
"struct VSOut { float4 pos : SV_POSITION; };\n"
"VSOut VSMain(VSIn input) {\n"
"    VSOut o;\n"
"    o.pos = float4(input.pos, 1.0);\n"
"    return o;\n"
"}\n";

const char* g_ps_src =
"struct PSIn { float4 pos : SV_POSITION; };\n"
"float4 PSMain(PSIn input) : SV_TARGET { return float4(1, 1, 1, 1); }\n";

std::vector<float3> vertices = {
	float3(-0.5f, -0.5f, 0.0f),
	float3(0.0f, 0.5f, 0.0f),
	float3(0.5f, -0.5f, 0.0f)
};

std::vector<uint16_t> indices = {
	0, 1, 2
};

Renderer::Renderer(HWND hWnd) : graphics(hWnd), testMesh(vertices, indices), testShader("test") {
	testMesh.CreateBuffers(graphics.GetDevice());
	testShader.CompileFromSource(g_vs_src, g_ps_src, graphics.GetDevice());
}

void Renderer::Render() {
	graphics.InitFrame();
	graphics.SetRenderTargetView();
	graphics.SetViewport();

	testMesh.Bind(graphics.GetContext());

	graphics.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	testShader.Bind(graphics.GetContext());

	graphics.GetContext()->DrawIndexed(static_cast<UINT>(indices.size()), 0, 0);

	graphics.PresentFrame();
}