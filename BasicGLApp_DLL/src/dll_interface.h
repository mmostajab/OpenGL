#ifdef SIMPLETEXTUREOUTDLL_EXPORTS
#define SIMPLETEXTUREOUTDLL_API __declspec(dllexport) 
#else
#define SIMPLETEXTUREOUTDLL_API __declspec(dllimport) 
#endif

#ifdef __cplusplus
	extern "C" {
#endif

SIMPLETEXTUREOUTDLL_API void init_dll(unsigned int w, unsigned int h);
SIMPLETEXTUREOUTDLL_API void create();
SIMPLETEXTUREOUTDLL_API int  getColorTextureID();
SIMPLETEXTUREOUTDLL_API void render(float time, float elapsedTime);
SIMPLETEXTUREOUTDLL_API void run();
//SIMPLETEXTUREOUTDLL_API void shutdown();

#ifdef __cplusplus
	 }
#endif