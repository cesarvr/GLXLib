#ifndef GLX_ENGINE_H_
#define GLX_ENGINE_H_

#include <iostream>
#include <cstdlib>
#include <GL/gl.h>
#include <GL/glx.h>

#include <X11/Xlib.h>


// Get a matching FB config 
static int visual_attribs[] = 
{
	GLX_X_RENDERABLE, True,
	GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
	GLX_RENDER_TYPE, GLX_RGBA_BIT,
	GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
	GLX_RED_SIZE, 8,
	GLX_GREEN_SIZE, 8,
	GLX_BLUE_SIZE, 8,
	GLX_ALPHA_SIZE, 8,
	GLX_DEPTH_SIZE, 24,
	GLX_STENCIL_SIZE, 8,
      	GLX_DOUBLEBUFFER, True,
        //GLX_SAMPLE_BUFFERS  , 1,
	//GLX_SAMPLES         , 4,
	None
};



class GLXLib {
private:
	Display *display;
	GLXContext gl_context;	
	GLXFBConfig glx_config;	
	Window window;		

	int frame_buffer;
	int current_sampler;
	void get_framebuffers();
public:
	GLXLib();
		
	void init( Display *d );
	void get_version();
	void create_window(int x, int y, int x2, int y2);
	void exit();
};

#endif //GLX_ENGINE_H_
