#include <cassert>
#include "glx_engine.h"

GLXLib::GLXLib(){
	frame_buffer 	=-1;
	current_sampler =-1;
}


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

void GLXLib::get_framebuffers() {
	int fbcount;

	GLXFBConfig* fbc = glXChooseFBConfig(display, DefaultScreen(display), visual_attribs, &fbcount);
	std::cout << "looking for framebuffers: " << std::endl; 
	
	if (!fbc) {
		std::cout << "can't get the list of framebuffers." << std::endl; 
		assert(fbc!=NULL);
	}

	for(int fb_index = 0; fb_index < fbcount; fb_index++) {
		XVisualInfo *vi = glXGetVisualFromFBConfig( display, fbc[ fb_index ] );
		if ( vi ) {
			int samp_buf, samples;
			glXGetFBConfigAttrib( display, fbc[fb_index], GLX_SAMPLE_BUFFERS, &samp_buf );
			glXGetFBConfigAttrib( display, fbc[fb_index], GLX_SAMPLES       , &samples  );

			std::cout << "fbconfig: " << fb_index << std::endl; 
			std::cout << "visual:" << vi -> visualid << std::endl; 
			std::cout << "sample buffers: " << samp_buf << std::endl; 
			std::cout << "sample: " << samples << std::endl << std::endl;
		
			if ( frame_buffer < 0 || samples && 
				samples > current_sampler )
			       		frame_buffer = fb_index, current_sampler = samples;
		}
	
		XFree( vi );
	}
	
	glx_config = fbc[ frame_buffer ];

	// Be sure to free the FBConfig list allocated by glXChooseFBConfig()
	XFree( fbc );
} 
