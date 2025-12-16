#include"Engine/Core/ChiliWin.h"
#include<Engine/Core/Application.h>
#include"Editor/EditorLayer.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow) {

	Application engine;
	engine.Init();

	engine.PushLayer(new EditorLayer());
	engine.Run();

	engine.Shutdown();

	return 0;
}