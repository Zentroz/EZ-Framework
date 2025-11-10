#include"Application.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow) {
	Application app(800, 800, "EZ Framework Application");
	app.Run();

	return 0;
}