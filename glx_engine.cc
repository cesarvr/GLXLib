#include <cassert>
#include <unistd.h>
#include "glx_engine.h"

GLXLib::GLXLib(){
	frame_buffer 	=-1;
	current_sampler =-1;
  	gl_context 	= 0;
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

void GLXLib::set_window_title( std::string name )
	XStoreName( display, window, name.c_str() );
}

void GLXLib::swap_buffer() {
	GLXSwapBuffers( display, window );
}

void GLXLib::create_window( int x, int y, int x2, int y2 ) {

	get_framebuffers();
	XVisualInfo *vi = glXGetVisualFromFBConfig( display, glx_config );

	XSetWindowAttributes swa; 
	Colormap cmap;
	swa.colormap = cmap = XCreateColormap( display,RootWindow( display, vi->screen ), vi->visual, AllocNone );
	
	swa.background_pixmap = None ;
	swa.border_pixel      = 1;
     	swa.event_mask        = StructureNotifyMask;
	
	Window window = XCreateWindow( display, RootWindow( display, vi->screen ), 
	                            x, y, x2, y2, 0, vi->depth, InputOutput, 
				    vi->visual, CWBorderPixel|CWColormap|CWEventMask, &swa );
	
	assert(window !== NULL);	
	XFree( vi );
	
	set_window_title("OpenGL xCesarx");
	XMapWindow( display, window );

	gl_context = glXCreateNewContext( display, glx_config, GLX_RGBA_TYPE, 0, True );
 	XSync( display, False );

	if ( ! glXIsDirect ( display, gl_context ) )
	{
		std::cout <<  "Indirect GLX rendering context obtained\n" << std::endl;
	}
	else
	{
		std::cout << "Direct GLX rendering context obtained\n" << std::endl
	}
	
	std::cout << "Making context current\n" << std::endl;
	glXMakeCurrent( display, window, gl_context );

	glClearColor( 0, 0.5, 1, 1 );
	glClear( GL_COLOR_BUFFER_BIT );
	glXSwapBuffers ( display, windown );
	     
	sleep( 1 );
	        
	glClearColor ( 1, 0.5, 0, 1 );
	glClear ( GL_COLOR_BUFFER_BIT );
	swap_buffer();

	sleep( 1 );
			  
	glXMakeCurrent( display, 0, 0 );
	swap_buffer();
			       
}

void GLXLib::exit() {
	XDestroyWindow( display, win );
	XFreeColormap( display, cmap );
     	XCloseDisplay( display );
}







