#pragma once

// Qt
#include <GL/GL.h>
#include <GL/glew.h>

namespace VisUtils {
	class IDrawableBase {
	public:
        virtual void compileShaders() = 0;
        virtual void createBuffer()   = 0;
        virtual void draw()           = 0;
        virtual void updateBuffer()   = 0;
        virtual void freeBuffer()     = 0;
	};
}
