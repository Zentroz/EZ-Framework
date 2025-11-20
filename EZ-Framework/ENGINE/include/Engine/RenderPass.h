#ifndef RENDERPASS_CLASS_H
#define RENDERPASS_CLASS_H

#include"Engine/RenderContext.h"

class RenderPass {
public:
	virtual ~RenderPass() = default;
	virtual void Execute(RenderContext* ctx) = 0;
};

#endif