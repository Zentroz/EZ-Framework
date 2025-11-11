#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include<Windows.h>
#include<iostream>
#include<string>
#include<d3d11.h>
#include<comdef.h>

inline void CheckDXHRException(HRESULT hr, const char* msg, const char* file, int line) {
    if (FAILED(hr)) {
        _com_error err(hr);
        std::string wStr = err.ErrorMessage();

        // Create error message with file and line info
        std::string errorMsg = "[DX11 Error] " + std::string(msg) +
            " | " + wStr +
            "\nFile: " + file +
            "\nLine: " + std::to_string(line);

        OutputDebugStringA(errorMsg.c_str());

        MessageBoxA(nullptr, errorMsg.c_str(), "DX11 Error", MB_OK | MB_ICONERROR);
        PostQuitMessage(-1); // Exit the application
    }
}

inline void CheckShaderCompilation(HRESULT hr, ID3DBlob* errorBlob, const char* shaderName, const char* file, int line) {
    if (FAILED(hr)) {
        std::string errorMsg;
        if (errorBlob != nullptr) {
            errorMsg = std::string((char*)errorBlob->GetBufferPointer());
            errorBlob->Release();
        }
        else {
            errorMsg = "Unknown shader error (no error blob)";
        }

        std::string fullError = "[Shader Error] " + std::string(shaderName) +
            "\nError: " + errorMsg +
            "\nFile: " + file +
            "\nLine: " + std::to_string(line);

        OutputDebugStringA(fullError.c_str());
        MessageBoxA(nullptr, fullError.c_str(), "Shader Error", MB_OK | MB_ICONERROR);
        PostQuitMessage(0);
    }
}

inline void ThrowException(const char* msg, const char* file, int line) {
    std::string fullError = std::string("[Exception] ") +
        "\nError: " + msg +
        "\nFile: " + file +
        "\nLine: " + std::to_string(line);

    OutputDebugStringA(fullError.c_str());
    MessageBoxA(nullptr, fullError.c_str(), "Exception Thrown", MB_OK | MB_ICONERROR);
    PostQuitMessage(-1);
}

inline void ThrowWarning(const char* msg, const char* file, int line) {
    std::string fullError = std::string("[Warning] ") +
        "\nError: " + msg +
        "\nFile: " + file +
        "\nLine: " + std::to_string(line);

    OutputDebugStringA(fullError.c_str());
    MessageBoxA(nullptr, fullError.c_str(), "Warning Thrown", MB_OK | MB_ICONWARNING);
}

#define CHECK_DXHR(hr, msg) CheckDXHRException(hr, msg, __FILE__, __LINE__)
#define CHECK_SHADER(hr, errorBlob, shaderName) CheckShaderCompilation(hr, errorBlob, shaderName, __FILE__, __LINE__)
#define EXCEPTION(msg) ThrowException(msg, __FILE__, __LINE__)
#define WARNING(msg) ThrowWarning(msg, __FILE__, __LINE__)


#endif