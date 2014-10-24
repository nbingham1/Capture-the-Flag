#include "material.h"

void materialhdl::init()
{
	first = NULL;
	last = NULL;

	vertex_name[0] = '\0';
	fragment_name[0] = '\0';

	vertex = NULL;
	fragment = NULL;
	program = NULL;
}

void materialhdl::init(char *vert, char *frag)
{
	init();
	int TextLen;
	char ErrText[256];

	TextLen = 256;
	vertex = LoadGLSLShader(GL_VERTEX_SHADER_ARB, vert, ErrText, &TextLen);
	printf("%s\n", ErrText);
	TextLen = 256;
	fragment = LoadGLSLShader(GL_FRAGMENT_SHADER_ARB, frag, ErrText, &TextLen);
	printf("%s\n", ErrText);

	program = glCreateProgramObjectARB();
	glAttachObjectARB(program, vertex);
	glAttachObjectARB(program, fragment);
	glLinkProgramARB(program);
}

void materialhdl::release()
{
	texturehdl *curr = first, *prev;
	while (curr != NULL)
	{
		prev = curr;
		curr = curr->next;
		glDeleteTextures(1, &prev->texture);
		delete prev;
		prev = NULL;
	}

	if (vertex != 0)
		glDeleteObjectARB(vertex);
	if (fragment != 0)
		glDeleteObjectARB(fragment);
	if (program != 0)
		glDeleteObjectARB(program);

	init();
}

void materialhdl::addtex(unsigned char *data, int width, int height, int depth, bool lod)
{
	texturehdl *ntex = new texturehdl;
	ntex->depth = depth;
	ntex->height = height;
	ntex->width = width;
	ntex->next = NULL;
	ntex->prev = NULL;

	glGenTextures(1, &ntex->texture);

	if (depth == 1)
	{
		glBindTexture(GL_TEXTURE_2D, ntex->texture);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, lod ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, lod ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		if (lod)
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		glBindTexture(GL_TEXTURE_3D, ntex->texture);
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, lod ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, lod ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, width, height, depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		if (lod)
			glGenerateMipmap(GL_TEXTURE_3D);
	}

	if (first == NULL)
	{
		first = ntex;
		last = ntex;
	}
	else
	{
		last->next = ntex;
		last->next->prev = last;
		last = last->next;
	}
}

void materialhdl::blank(int width, int height, int depth)
{
	unsigned char *data = new unsigned char[width*height*depth];
	for (int x = 0; x < width*height*depth; x++)
		data[x] = 0;

	addtex(data, width, height, depth, false);

	delete data;
}

void materialhdl::srend(texturehdl *n)
{
	if (n != NULL)
		glViewport(0, 0, n->width, n->height);
}

void materialhdl::erend(texturehdl *n, GLenum format)
{
	if (n == NULL)
		return;

	glBindTexture(GL_TEXTURE_2D,  n->texture);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, format, 0, 0, n->width, n->height, 0);

	glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

void materialhdl::bind()
{
	char id[32];
	texturehdl *curr = first;
	for (int x = 0; curr != NULL; x++)
	{
		glActiveTexture(GL_TEXTURE0 + x);
		glBindTexture(curr->depth == 1 ? GL_TEXTURE_2D : GL_TEXTURE_3D, curr->texture);
		sprintf(id, "tex_%d", x);
		glUniform1iARB(glGetUniformLocationARB(program, id), x);
		curr = curr->next;
	}

	glUseProgramObjectARB(program);
}
