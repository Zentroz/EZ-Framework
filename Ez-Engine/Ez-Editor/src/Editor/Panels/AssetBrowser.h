#pragma once

#include<filesystem>

class AssetBrowser {
public:
	AssetBrowser();

	void OnRenderImGui();
	void SetDirectory(std::filesystem::path& path);
private:
	std::filesystem::path m_CurrentDir;
	std::filesystem::path m_AssetsDir;

	// Input buffers
	char textInput[256] = "";
	float floatInput = 0;
	float float2Input[2] = { 0, 0 };
	float float3Input[3] = { 0, 0, 0 };
	float float4Input[4] = { 0, 0, 0, 0 };
};