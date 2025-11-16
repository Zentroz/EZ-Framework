#include"Engine/Renderer.h"

Renderer::Renderer() : testMesh("Mesh/monkey.obj", "test"), testShader("test"), testMaterial(&testShader) {
	OutputDebugStringA("Renderer created\n");
}

void Renderer::Init(HWND hWnd) {
	graphics.Init(hWnd);

	testMesh.CreateBuffers(graphics.GetDevice());
	testShader.CompileFromFile("Shaders/default.hlsl", graphics.GetDevice());

	testMaterial.ReflectShaderVariables();
	CreateGlobalBuffer();

	testMaterial.CreateMaterialBuffer(graphics.GetDevice());

	testMaterial.SetFloat("test", 0.5f);

	testMaterial.Upload(graphics.GetContext());
}

void Renderer::Shutdown() {
	graphics.Shutdown();
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
	float3 lightPosition = float3(0, 0, -3);
	float3 testMeshPosition = float3(0, 0, 1);

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