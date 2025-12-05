#ifndef WINDOW_CLASS_H
#define WINDOW_CLASS_H

#include<stdexcept>
#include<imgui/backends/imgui_impl_win32.h>

#include"Engine/Core/ChiliWin.h"
#include"Engine/Render/Backend/Input.h"

class Window {
private:
	class WindowClass {
	public:
		static LPCWSTR GetNameWSTR() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		static constexpr LPCWSTR wndClassName = L"EZ-Editor Window Class";
		static WindowClass wndClass;
		HINSTANCE hInstance;
	};
public:
	bool isRunning;

	Window() = default;
	~Window() = default;

	struct WindowInitData {
		int width; int height; const char* title;
	};

	void Init(WindowInitData initData);
	void Shutdown();
	void Run();
	HWND GetHwnd() const noexcept { return hWnd; }
	ENGINE::Input * GetInput() noexcept { return &input; }
	WindowInitData& GetInitData() { return initData; }

private:
	HWND hWnd;
	ENGINE::Input input;
	WindowInitData initData;

	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
};

#endif