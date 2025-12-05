/*
#include"Core/ChiliWin.h"
#include<commdlg.h>

#include<string>

namespace EDITOR {
    std::string OpenFileDialog(const char* filter, HWND hWnd) {
        OPENFILENAME ofn;       // Common dialog box structure
        TCHAR szFile[260] = { 0 }; // Buffer for file name and path

        // Initialize OPENFILENAME structure
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hWnd; // Handle to the owner window
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile) / sizeof(TCHAR); // Max size of the buffer
        ofn.lpstrFilter = "All Files\0*.*\0Text Files\0*.TXT\0"; // Filter string
        ofn.nFilterIndex = 1; // Default filter index (1-based)
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL; // Initial directory
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // Flags for dialog behavior

        if (GetOpenFileNameA(&ofn) == TRUE) {
            return ofn.lpstrFile;
        }

        return std::string();
    }
}

*/