#include"Engine/Core/ChiliWin.h"
#include<Engine/Engine.h>
#include"Editor/EditorLayer.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow) {
	ENGINE::Engine engine = ENGINE::Engine();
	engine.Init({ 1920, 1080, "Ez-Engine" });

	ENGINE::Engine::Get().LayerStack()->AttachLayer(new EditorLayer());

	engine.Run();
	engine.Shutdown();

	return 0;
}