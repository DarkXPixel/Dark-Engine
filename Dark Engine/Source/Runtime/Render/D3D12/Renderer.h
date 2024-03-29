#pragma once
#include <stdint.h>
#include <Core.h>

#define D3D12
#define RENDER_API 


class D3D12Scene;


class DENGINE_API Renderer
{
public:
	virtual int32_t Init() = 0;
	virtual void Shutdown() = 0;

	virtual void Render(D3D12Scene*) = 0;
	virtual void EndFrame() = 0;
	virtual void SetResolution(int32 Width, int32 Height, bool IsFullScreen) = 0;

	virtual void SetVsync(int vsync);




protected:
	bool bVsync = false;

};

#ifdef _WIN32
#ifdef D3D12
#include "D3D12/D3D12Render.h"
#define RENDER_API D3D12Renderer
#endif
#ifdef GL
#include "OpenGL/OpenGLRender.h"
#define RENDER_API OpenGLRender
#endif
#endif // WIN32
