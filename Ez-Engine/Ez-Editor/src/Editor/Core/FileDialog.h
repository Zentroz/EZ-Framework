#pragma once

#include<string>

#include"Editor/Core/EditorWinApi.h"

std::wstring OpenFileDialog(const char* filter, HWND hWnd);
std::wstring OpenFolderDialog();