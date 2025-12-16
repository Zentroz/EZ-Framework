#pragma once

#include<EzAPI/Core/Script.h>

class TestScript : public EZ::Script {
public:
	void OnUpdate(float dt) override;
};