#include "vector.h"
#include "mathdef.h"
#include "standard.h"
#include "graphics.h"
#include "camera.h"
#include "shader.h"

#ifndef material_h
#define material_h

struct texturehdl
{
	GLuint texture;
	int width;
	int height;
	int depth;

	texturehdl *next, *prev;
};

struct materialhdl
{
	texturehdl *first, *last;

	GLhandleARB vertex;
	GLhandleARB fragment;
	GLhandleARB program;

	char vertex_name[256];
	char fragment_name[256];

	void init();
	void init(char *vert, char *frag);
	void release();
	void addtex(unsigned char *data, int width, int height, int depth, bool lod);

	void blank(int width, int height, int depth);

	void srend(texturehdl *n);
	void erend(texturehdl *n, GLenum format);

	void bind();
};

#endif
