#include"Game/TestScript.h"

extern "C" __declspec(dllexport)
void RegisterGame() {
}

extern "C" __declspec(dllexport)
EZ::Script* Test() {
	return new TestScript();
}