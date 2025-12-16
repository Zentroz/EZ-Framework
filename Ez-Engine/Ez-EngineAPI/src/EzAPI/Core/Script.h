#pragma once

#include<string>
#include"GameObject.h"

namespace EZ {
	class Script {
	public:
		GameObject gameObject;
	public:
		virtual ~Script() = default;

		virtual void OnCreate() {}
		virtual void OnUpdate(float dt) {}
		virtual void OnDestroy() {}
	};
}