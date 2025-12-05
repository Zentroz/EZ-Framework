#ifndef IMGUI_LAYER_CLASS_H
#define IMGUI_LAYER_CLASS_H

#include<vector>
#include<imgui/imgui.h>
#include<imgui/backends/imgui_impl_win32.h>
#include<imgui/backends/imgui_impl_dx11.h>

class ImGuiLayer {
public:

	static void Setup();
	static void NewFrame();
	static void EndFrame();

};

#endif