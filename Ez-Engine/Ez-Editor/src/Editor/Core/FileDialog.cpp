#include"Editor/Core/FileDialog.h"
#include"Editor/Core/EditorWinApi.h"
#include<commdlg.h>
#include <shlobj.h>
#include <iostream>
#include <atlbase.h>

std::wstring OpenFileDialog(const char* filter, HWND hWnd) {
    OPENFILENAME ofn;       // Common dialog box structure
    TCHAR szFile[260] = { 0 }; // Buffer for file name and path

    // Initialize OPENFILENAME structure
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd; // Handle to the owner window
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile) / sizeof(TCHAR); // Max size of the buffer
    ofn.lpstrFilter = L"All Files\0*.*\0Text Files\0*.TXT\0"; // Filter string
    ofn.nFilterIndex = 1; // Default filter index (1-based)
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL; // Initial directory
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // Flags for dialog behavior

    if (GetOpenFileName(&ofn) == TRUE) {
        return ofn.lpstrFile;
    }

    return std::wstring();
}

struct ComInit {
    ComInit() { CoInitialize(nullptr); }
    ~ComInit() { CoUninitialize(); }
};

std::wstring OpenFolderDialog() {
    ComInit com; // Initializes COM

    CComPtr<IFileOpenDialog> pFolderDlg;
    HRESULT hr = pFolderDlg.CoCreateInstance(CLSID_FileOpenDialog);

    if (SUCCEEDED(hr)) {
        DWORD dwOptions;
        // Get current options and add FOS_PICKFOLDERS
        hr = pFolderDlg->GetOptions(&dwOptions);
        if (SUCCEEDED(hr)) {
            hr = pFolderDlg->SetOptions(dwOptions | FOS_PICKFOLDERS | FOS_PATHMUSTEXIST | FOS_FORCEFILESYSTEM);
        }

        if (SUCCEEDED(hr)) {
            // Show the dialog modally
            hr = pFolderDlg->Show(nullptr);
            if (SUCCEEDED(hr)) {
                // Obtain the result
                CComPtr<IShellItem> pSelectedItem;
                hr = pFolderDlg->GetResult(&pSelectedItem);

                if (SUCCEEDED(hr)) {
                    CComHeapPtr<wchar_t> pPath;
                    // Get the file system path
                    hr = pSelectedItem->GetDisplayName(SIGDN_FILESYSPATH, &pPath);

                    if (SUCCEEDED(hr)) {
                        return std::wstring(pPath.m_pData);
                    }
                }
            }
        }
    }

    return L""; // Return empty string if cancelled or error
}