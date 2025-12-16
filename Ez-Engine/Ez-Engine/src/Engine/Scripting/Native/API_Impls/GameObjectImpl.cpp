#include<EzAPI/Core/GameObject.h>
#include"Engine/Engine.h"

namespace EZ {
	void GameObject::AddComponentRaw(uint8_t componentType) {
		ENGINE::Engine::Get().Registry().AddComponent();
	}
}