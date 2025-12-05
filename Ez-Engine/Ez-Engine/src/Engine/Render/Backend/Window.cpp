#include"Window.h"

std::wstring widen(const char* str) {
    return std::wstring(str, str + strlen(str));
}

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept : hInstance(GetModuleHandle(nullptr)) {
    WNDCLASSEX wc = { 0 };

	wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = HandleMsgSetup;
    wc.hInstance = GetInstance();
    wc.lpszClassName = GetNameWSTR();

    RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass() {
    UnregisterClass(GetNameWSTR(), GetInstance());
}

LPCWSTR Window::WindowClass::GetNameWSTR() noexcept
{
    return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
    return wndClass.hInstance;
}

void Window::Init(WindowInitData initData) {
    this->initData = initData;

    RECT wr;
    wr.left = 100;
    wr.right = initData.width + wr.left;
    wr.top = 100;
    wr.bottom = initData.height + wr.top;

    AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

    int width = wr.right - wr.left;
    int height = wr.bottom - wr.top;

    LPCWSTR wndClass = WindowClass::GetNameWSTR();

    hWnd = CreateWindowEx(
        0,                      // Optional window styles.
        wndClass, // Window class
        widen(initData.title).c_str(),                  // Window text
        WS_OVERLAPPEDWINDOW,    // Window style

        // Size and position
        0, 0, width, height,

        NULL,       // Parent window    
        NULL,       // Menu
        WindowClass::GetInstance(),  // Instance handle
        this        // Additional application data
    );

    if (hWnd == NULL)
    {
        throw std::runtime_error("Failed to create window.");
        return;
    }

    ShowWindow(hWnd, SW_SHOW);

    isRunning = true;
}

void Window::Run() {
    MSG msg = { };

    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            isRunning = false;
            break;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    input.UpdateMousePosition(GetHwnd());
}

void Window::Shutdown() {
    DestroyWindow(hWnd);
	hWnd = NULL;
    isRunning = false;
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
    if (msg == WM_NCCREATE)
    {
        // extract ptr to window class from creation data
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        // set WinAPI-managed user data to store ptr to window instance
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // set message proc to normal (non-setup) handler now that setup is finished
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
        // forward message to window instance handler
        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }
    // if we get a message before the WM_NCCREATE message, handle with default handler
    return DefWindowProc(hWnd, msg, wParam, lParam);
}


LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // retrieve ptr to window instance
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    // forward message to window instance handler
    return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;
    // (Your code process Win32 messages)
    // (You should discard mouse/keyboard messages in your game/engine when io.WantCaptureMouse/io.WantCaptureKeyboard are set.)

    switch (msg)
    {
    case WM_KEYDOWN:
		input.KeyDown(static_cast<unsigned char>(wParam));
        break;
    case WM_KEYUP:
		input.KeyUp(static_cast<unsigned char>(wParam));
        break;
    case WM_CLOSE:
		PostQuitMessage(0);
        break;
    }

	return DefWindowProc(hWnd, msg, wParam, lParam);
}