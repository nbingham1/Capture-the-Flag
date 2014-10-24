#include "renderer.h"

void renderhdl::init()
{
	display.init();
	camera.init(&display.framespersecond);

	numteams = 3;
	srand(time(0));

	glEnable(GL_LIGHTING);
	{
		GLfloat pos[] =  {1.0, 0.0, 0.0, 0.0};
		GLfloat spec[] = {1.0, 1.0, 1.0, 1.0};
		GLfloat ambi[] = {0.5, 0.5, 0.5, 1.0};
		GLfloat diff[] = {1.0, 1.0, 1.0, 1.0};

		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambi);
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
		//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
		glEnable(GL_LIGHT0);
		{
			glLightfv(GL_LIGHT0, GL_POSITION, pos);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
			glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
			glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, -1);
			glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, -1);
			glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, -1);
		}
	}

	for (int x = 0; x < numteams; x++)
		a[x].Init(100);
}

void renderhdl::release()
{
	for (int x = 0; x < numteams; x++)
		a[x].Release();
}

void renderhdl::displayf()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	camera.setproj();

	GLfloat pos[] = {.97, 0.24, 0.0, 0.0};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	GLfloat distance = 0.0;
	for (int x = 0; x < numteams; x++)
		for (int y = 0; y < a[x].size; y++)
			if (!a[x].captured)
				for (int z = 0; z < numteams; z++)
				{
					vec pos = a[x].players[y].Position + a[x].players[y].LinearVelocity;
					distance = dist(pos, a[z].FlagPos);

					if (distance <= 5.0 + a[x].players[y].Radius)
					{
						vec xaxis, U1x, U1y, U2x, U2y;
						double b, c;
						xaxis = norm(a[z].FlagPos-pos);
						b = dot(xaxis, a[x].players[y].LinearVelocity);
						U1x = xaxis*b;
						U1y = a[x].players[y].LinearVelocity - U1x;
						xaxis = norm(pos-a[z].FlagPos);
						c = dot(xaxis, a[z].LinearVelocity);
						U2x = xaxis*c;
						U2y = a[z].LinearVelocity - U2x;

						a[z].LinearVelocity = U2x+U1y;
						a[x].players[y].LinearVelocity = (U1x+U2y);

						a[z].FlagPos += .5*(distance - (a[x].players[y].Radius + 5.0))*norm(pos - a[z].FlagPos);
						a[x].players[y].Position -= .5*(distance - (a[x].players[y].Radius + 5.0))*norm(pos - a[z].FlagPos);
					}

					for (int w = 0; w < a[z].size; w++)
					{
						if (!a[z].captured && !(z == x && y == w))
						{
							vec pos = a[x].players[y].Position + a[x].players[y].LinearVelocity;
							vec pos2 = a[z].players[w].Position + a[z].players[w].LinearVelocity;
							distance = dist(pos, pos2);

							if (distance <= a[z].players[w].Radius + a[x].players[y].Radius)
							{
								vec xaxis, U1x, U1y, U2x, U2y;
								double b, c;
								xaxis = norm(pos2-pos);
								b = dot(xaxis, a[x].players[y].LinearVelocity);
								U1x = xaxis*b;
								U1y = a[x].players[y].LinearVelocity - U1x;
								xaxis = norm(pos-pos2);
								c = dot(xaxis, a[z].players[w].LinearVelocity);
								U2x = xaxis*c;
								U2y = a[z].players[w].LinearVelocity - U2x;

								a[z].players[w].LinearVelocity = (U2x+U1y);
								a[x].players[y].LinearVelocity = (U1x+U2y);

								a[z].players[w].Position += .5*(distance - (a[x].players[y].Radius + a[z].players[w].Radius))*norm(pos - pos2);
								a[x].players[y].Position -= .5*(distance - (a[x].players[y].Radius + a[z].players[w].Radius))*norm(pos - pos2);
							}
						}
					}
				}

	for (int x = 0; x < numteams; x++)
		a[x].Render(a, numteams);

	display.render();

	glutSwapBuffers();
}
