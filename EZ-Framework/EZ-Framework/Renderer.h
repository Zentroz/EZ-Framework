#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include"Graphics.h"
#include"Mesh.h"
#include"Shader.h"

class Renderer {
public:
	Renderer(HWND hWnd);

	void Render();
private:
	Graphics graphics;
	Mesh testMesh;
	Shader testShader;
};

#endif