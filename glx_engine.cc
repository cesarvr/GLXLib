#include "glx_engine.h"

GLXLib::GLXLib(){}

void GLXLib::init(Display *d) {
	display = d;
}

void GLXLib::get_version() {
	int glx_major, glx_minor;
	 
	// FBConfigs were added in GLX version 1.3.
	if ( !glXQueryVersion( display, &glx_major, &glx_minor ) || 
			( ( glx_major == 1 ) && ( glx_minor < 3 ) ) || ( glx_major < 1 ) ) {
		std::cout << "Invalid GLX version" << std::endl;
		exit(1);
	}
	std::cout << "OpenGL Version " << glx_major << "." << glx_minor << std::endl;
}

