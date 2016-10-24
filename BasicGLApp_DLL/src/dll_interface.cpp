#include "dll_interface.h"
#include "Application.h"

#include <Windows.h>

Application* application = 0;

SIMPLETEXTUREOUTDLL_API void init_dll(unsigned int w, unsigned int h) {
	HGLRC mainGLRC = wglGetCurrentContext();

	application = new Application();
	application->init(w, h, mainGLRC);
	application->create();

}

SIMPLETEXTUREOUTDLL_API void create() {
	application->create();
}

SIMPLETEXTUREOUTDLL_API int  getColorTextureID()
{
	return application->fbo_textures[0];
}

SIMPLETEXTUREOUTDLL_API void render(float time, float elapsedTime){
	application->run_onestep();
}

SIMPLETEXTUREOUTDLL_API void run(){
	application->run();
}

SIMPLETEXTUREOUTDLL_API void shutdown(){
	application->shutdown();
}