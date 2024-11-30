#pragma once
#include "engineDefinitions.hpp"
#include <raylib.h>

class VirtualResWindow {
public:
	u32 m_gameScreenWidth = 16*60;
	u32 m_gameScreenHeight = 9*60;
	f32 m_screenScale;
	RenderTexture m_finalFBO;

	bool m_performanceOverview;

	Vector2 m_screenMouse;
	bool m_mouseAlreadyClickedOnSomething;

	Shader m_postProcessShader;

private:
	Rectangle m_fboSourceRec;
	Rectangle m_fboDestRec;

public:
	VirtualResWindow();
	~VirtualResWindow();

	bool init(u32 screenWidth, u32 screenHeight, u32 targetFPS);
	void updateVirtualMouse();
	void updateScreen();

	void drawFBOToWindow();
};

RenderTexture2D LoadRenderTexturePro(int width, int height, PixelFormat pixelFormat);