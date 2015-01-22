#include <iostream>
#include <string>
#include <cstdlib>

#include <X11/Xlib.h>

#include "glx_engine.h"

int main(void){
	Display *d;
	Window w;
	XEvent e;
	GLXLib libglx;

	std::string msg = "hello world";

	d = XOpenDisplay(NULL);
	
	
	libglx.init(d);
	libglx.get_version();
	libglx.get_framebuffers();
	if (d==NULL){
		std::cout << "no screen error" << std::endl;
		exit(1);
	}
	
	int s = DefaultScreen(d);
	std::cout << "default screen: " << s << std::endl;
	
	w = XCreateSimpleWindow(d, RootWindow(d,s), 10, 10, 100, 100, 1, BlackPixel(d, s), WhitePixel(d, s));
	
	XSelectInput(d, w, ExposureMask | KeyPressMask);
	XMapWindow(d, w);
	
	while (1) {
		XNextEvent(d, &e);
		if (e.type == Expose) {
			XFillRectangle(d, w, DefaultGC(d, s), 20, 20, 10, 10);
			XDrawString(d, w, DefaultGC(d, s), 10, 50, msg.c_str(), msg.length() );
			XDrawString(d, RootWindow(d,s), DefaultGC(d,s), 20, 50, msg.c_str(), msg.length() );
		}
		
		if (e.type == KeyPress)
			break;
	}
	
	XCloseDisplay(d);
	return 0;
}
