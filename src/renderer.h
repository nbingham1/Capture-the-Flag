#include "display.h"
#include "camera.h"

#include "Team.h"

#ifndef renderer_h
#define renderer_h

struct renderhdl
{
	displayhdl display;
	camerahdl  camera;

	Team a[10];
	int numteams;

	void init();
	void release();

	void displayf();
};

#endif
