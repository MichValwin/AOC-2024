#include "virtualResWindow.hpp"
#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include <GLFW/glfw3.h>
#include <algorithm>

VirtualResWindow::VirtualResWindow() {}

bool VirtualResWindow::init(u32 screenWidth, u32 screenHeight, u32 targetFPS) {
	m_gameScreenWidth = screenWidth;
	m_gameScreenHeight = screenHeight;

	InitWindow(m_gameScreenWidth, m_gameScreenHeight, "RPG FILL IDLE");
	SetWindowState(FLAG_WINDOW_RESIZABLE);

	m_performanceOverview = false;

	// Monitor
	u32 monitorCount = GetMonitorCount();
	LOG(LOG_INFO, "Monitor count: %d", monitorCount);
	if(monitorCount > 0)SetWindowMonitor(0);

	m_finalFBO = LoadRenderTexturePro(m_gameScreenWidth, m_gameScreenHeight, PixelFormat::PIXELFORMAT_UNCOMPRESSED_R8G8B8);
		
	//m_postProcessShader = LoadShader(0, TextFormat("./resources/shaders/postprocessCRT.fs"));

	SetTargetFPS(targetFPS);

	updateScreen();
	updateVirtualMouse();

	return true;
}

VirtualResWindow::~VirtualResWindow() {
	UnloadRenderTexture(m_finalFBO);
	//UnloadShader(m_postProcessShader);
	CloseWindow();
}

void VirtualResWindow::updateVirtualMouse() {
	// Update Screen mouse
	Vector2 mouse = GetMousePosition();
	m_screenMouse.x = (mouse.x - (GetScreenWidth() - (m_gameScreenWidth*m_screenScale))*0.5f)/m_screenScale;
	m_screenMouse.y = (mouse.y - (GetScreenHeight() - (m_gameScreenHeight*m_screenScale))*0.5f)/m_screenScale;
	m_screenMouse = Vector2Clamp(m_screenMouse, {0, 0}, {(f32)m_gameScreenWidth, (f32)m_gameScreenHeight});
}

void VirtualResWindow::updateScreen() {
	LOG(LOG_INFO, "Calling to update screen, with the new screen size of: %d,%d", GetScreenWidth(), GetScreenHeight());

	// We have to keep fboAspectRatio and center the picture
	// Scale
	m_screenScale = std::min((f32)GetScreenWidth()/m_gameScreenWidth, (f32)GetScreenHeight()/m_gameScreenHeight);
	// Rectangles to pass to Draw DrawTexturePro
	m_fboSourceRec = { 0.0f, 0.0f, (f32)m_finalFBO.texture.width, -(f32)m_finalFBO.texture.height }; // Invert height to flip it
	m_fboDestRec = { 
		(GetScreenWidth() - ((f32)m_gameScreenWidth*m_screenScale))*0.5f, 
		(GetScreenHeight() - ((f32)m_gameScreenHeight*m_screenScale))*0.5f,
		(f32)m_gameScreenWidth*m_screenScale,
		(f32)m_gameScreenHeight*m_screenScale 
	};

	// Update global shader
	/*
	float UnifScreenSizeLoc = GetShaderLocation(m_postProcessShader, "u_fboSize");
	float screenSize[2] = {m_fboDestRec.width, m_fboDestRec.height};
	SetShaderValue(m_postProcessShader, UnifScreenSizeLoc, screenSize, SHADER_UNIFORM_VEC2);
	*/
}

void VirtualResWindow::drawFBOToWindow() {
	BeginDrawing();
		ClearBackground(PINK);
		Camera2D screenSpaceCamera = { 0 }; // Smoothing camera
		screenSpaceCamera.zoom = 1.0f;
		
		BeginMode2D(screenSpaceCamera);
			//BeginShaderMode(m_postProcessShader);
				DrawTexturePro(m_finalFBO.texture, m_fboSourceRec, m_fboDestRec, {0,0}, 0.0f, WHITE);
			//EndShaderMode();
		EndMode2D();
	EndDrawing();
}

// Load a Render texture object
RenderTexture2D LoadRenderTexturePro(int width, int height, PixelFormat pixelFormat) {
	RenderTexture2D target = { 0 };

	target.id = rlLoadFramebuffer(); // Load an empty framebuffer

	if (target.id > 0) {
		rlEnableFramebuffer(target.id);

		// Create color texture (default to RGBA)
		target.texture.id = rlLoadTexture(NULL, width, height, pixelFormat, 1);
		target.texture.width = width;
		target.texture.height = height;
		target.texture.format = pixelFormat;
		target.texture.mipmaps = 1;

		/* No depth
		// Create depth renderbuffer/texture
		target.depth.id = rlLoadTextureDepth(width, height, true);
		target.depth.width = width;
		target.depth.height = height;
		target.depth.format = 19;       //DEPTH_COMPONENT_24BIT?
		target.depth.mipmaps = 1;
		*/

		// Attach color texture and depth renderbuffer/texture to FBO
		rlFramebufferAttach(target.id, target.texture.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
		rlFramebufferAttach(target.id, target.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_RENDERBUFFER, 0);

		// Check if fbo is complete with attachments (valid)
		if (rlFramebufferComplete(target.id)) TRACELOG(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", target.id);

		rlDisableFramebuffer();
	}
	else TRACELOG(LOG_WARNING, "FBO: Framebuffer object can not be created");

	return target;
}