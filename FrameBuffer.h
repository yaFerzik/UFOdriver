#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Mesh.h"

class FrameBuffer
{
public:
	unsigned int FBO;
	unsigned int framebufferTexture;
	unsigned int framebufferDepthTexture;
	unsigned int RBO;
	int slot;
	int depthSlot;

	unsigned int vao;

	FrameBuffer() {};

	FrameBuffer(int slot, int depthSlot, int win_width, int win_height);

	void Activate();

	void Deactivate();

	void Draw(Shader shader);

	void Destroy();
};

#endif