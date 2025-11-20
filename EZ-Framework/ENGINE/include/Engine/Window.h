#ifndef WINDOW_CLASS_H
#define WINDOW_CLASS_H

#include"Core/ChiliWin.h"
#include"Input.h"
#include<stdexcept>

class Window {
private:
	class WindowClass {
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		static constexpr const char* wndClassName = "EZ-Framework Window Class";
		static WindowClass wndClass;
		HINSTANCE hInstance;
	};
public:
	bool isRunning;

	Window() = default;
	~Window() = default;

	void Init(int width, int height, const char* title);
	void Shutdown();
	void Run();
	HWND GetHwnd() const noexcept { return hWnd; }
	Input* GetInput() noexcept { return &input; }

private:
	int width;
	int height;
	const char* title;
	HWND hWnd;
	Input input;

	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
};

#endif