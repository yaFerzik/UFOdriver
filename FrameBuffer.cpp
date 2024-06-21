#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(int slot, int depthSlot, int win_width, int win_height)
{
	this->slot = slot;
	this->depthSlot = depthSlot;
	//glActiveTexture(GL_TEXTURE0);
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	// Create Framebuffer Texture

	glGenTextures(1, &framebufferTexture);
	glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, win_width, win_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	

	glGenTextures(1, &framebufferDepthTexture);
	glBindTexture(GL_TEXTURE_2D, framebufferDepthTexture);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, win_width, win_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, framebufferDepthTexture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);


	/*
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, win_width, win_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
	*/
	
	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error: " << fboStatus << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	float rectangleVertices[] =
	{
		// Coords    // texCoords
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,

		 1.0f,  1.0f,  1.0f, 1.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f
	};

	unsigned int rectVBO;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &rectVBO);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

}

void FrameBuffer::Activate()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void FrameBuffer::Deactivate()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Draw(Shader shader)
{
	shader.Activate();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	glUniform1i(glGetUniformLocation(shader.id, "screenTexture"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, framebufferDepthTexture);
	glUniform1i(glGetUniformLocation(shader.id, "depthTexture"), 1);

	glBindVertexArray(vao);
	glDisable(GL_DEPTH_TEST);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
}