#include"Renderer.h"

/*
std::vector<Mesh::Vertex> vertices = {
	{{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0, 0}}, // 0	// Front (+Z)
	{{ 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0, 0}}, // 1
	{{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0, 0}}, // 2
	{{-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0, 0}}, // 3
	{{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0, 0}}, // 4	// Back (-Z)
	{{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0, 0}}, // 5
	{{-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0, 0}}, // 6
	{{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0, 0}}, // 7
	{{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0, 0}}, // 8	// Left (-X)
	{{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {0, 0}}, // 9
	{{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {0, 0}}, // 10
	{{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0, 0}}, // 11
	{{ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {0, 0}}, // 12	// Right (+X)
	{{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {0, 0}}, // 13
	{{ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {0, 0}}, // 14
	{{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {0, 0}}, // 15
	{{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {0, 0}}, // 16	// Top (+Y)
	{{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {0, 0}}, // 17
	{{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {0, 0}}, // 18
	{{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {0, 0}}, // 19
	{{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {0, 0}}, // 20	// Bottom (-Y)
	{{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {0, 0}}, // 21
	{{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {0, 0}}, // 22
	{{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {0, 0}}, // 23
};

std::vector<uint16_t> indices = {
	// Front (+Z)
	0, 1, 2,
	0, 2, 3,

	// Back (-Z)
	4, 5, 6,
	4, 6, 7,

	// Left (-X)
	8, 9, 10,
	8, 10, 11,

	// Right (+X)
	12, 13, 14,
	12, 14, 15,

	// Top (+Y)
	16, 17, 18,
	16, 18, 19,

	// Bottom (-Y)
	20, 21, 22,
	20, 22, 23
};
*/


Renderer::Renderer(HWND hWnd) : graphics(hWnd), testMesh("Mesh/monkey.obj", "test"), testShader("test"), testMaterial(&testShader) {
	testMesh.CreateBuffers(graphics.GetDevice());
	testShader.CompileFromFile("Shaders/default.hlsl", graphics.GetDevice());

	testMaterial.ReflectShaderVariables();
	CreateGlobalBuffer();

	testMaterial.CreateMaterialBuffer(graphics.GetDevice());

	testMaterial.SetFloat("test", 0.5f);

	testMaterial.Upload(graphics.GetContext());

	camera.SetPosition(float3(0.0f, 0.0f, -3.0f));

	lightPosition = float3(0, 0, -3);
	testMeshPosition = float3(0, 0, 1);
}

void Renderer::CreateGlobalBuffer() {
	HRESULT hr;

	D3D11_BUFFER_DESC cbd = {};
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(GlobalConstantBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = 0;

	hr = graphics.GetDevice()->CreateBuffer(&cbd, nullptr, &globalBuffer);
}

void Renderer::SetGlobalBuffers() {
	GlobalConstantBuffer cbData;

	cbData.model = XMMatrixTranspose(
		XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), XMConvertToRadians(sin(Time::time / 6) * 360)) *
		XMMatrixTranslation(testMeshPosition.x, testMeshPosition.y, testMeshPosition.z)
	);

	cbData.view = XMMatrixTranspose(camera.GetViewMatrix());
	cbData.projection = XMMatrixTranspose(camera.GetProjectionMatrix(1.0f));
	cbData.cameraPosition = camera.GetPosition();
	cbData.lightDirection = (lightPosition - testMeshPosition).normalize();
	cbData.deltaTime = Time::deltaTime;
	cbData.time = Time::time;

	graphics.GetContext()->UpdateSubresource(globalBuffer, 0, nullptr, &cbData, 0, 0);
}

void Renderer::Render() {
	graphics.InitFrame();

	SetGlobalBuffers();

	testMaterial.Upload(graphics.GetContext());
	testMaterial.Bind(graphics.GetContext());

	testMesh.Bind(graphics.GetContext());

	graphics.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	testShader.Bind(graphics.GetContext());

	graphics.GetContext()->VSSetConstantBuffers(0, 1, &globalBuffer);
	graphics.GetContext()->PSSetConstantBuffers(0, 1, &globalBuffer);

	graphics.GetContext()->DrawIndexed(testMesh.GetIndexCount(), 0, 0);

	graphics.PresentFrame();
}