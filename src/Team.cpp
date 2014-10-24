/*
 *  Team.cpp
 *  AI Algorithms Class
 *
 *  Created by Ned Bingham on 7/9/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "Team.h"

void Team::Init(int s)
{
	size = s;
	players = new AI[s];

	for (int x = 0; x < 3; x++)
		color[x] = GLfloat(rand()%100)/100.0;

	color[3] = 1.0;

	FlagPos.x = double(rand()%(SpaceLimit*2) - SpaceLimit);
	FlagPos.y = double(rand()%(SpaceLimit*2) - SpaceLimit);
	FlagPos.z = double(rand()%(SpaceLimit*2) - SpaceLimit);

	ReturnPos = FlagPos + vec(0, 10, 0);

	for (int x = 0; x < size; x++)
	{
		players[x].Setup(1.0, this);
		players[x].Position = FlagPos + vec(double(rand()%100)/10.0, 10.0 + double(rand()%100)/10.0, double(rand()%100)/10.0);
	}

	captured = false;
	target = NULL;
	ident = rand();
}

void Team::Render(Team *teams, int num)
{
	if (!captured)
	{
		int r = rand()%num;
		int temp_r = (r+1)%num;

		if (target == NULL || target->captured)
		{
			target = &teams[r];
			while ((target->ident == ident || target->captured) && (temp_r != r))
			{
				target = &teams[temp_r];
				temp_r = (temp_r+1)%num;
			}

			for (int x = 0; x < size; x++)
				players[x].step = 0;
		}

		GLfloat n[] = {0.0, 0.0, 0.0, 0.0};
		for (int x = 0; x < size; x++)
		{
			players[x].Update(target);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, n);
			players[x].Render();
		}

		FlagPos += LinearVelocity;
		LinearVelocity *= 0.9;

		if ((FlagPos.x > SpaceLimit && LinearVelocity.x > 0) || (FlagPos.x < -SpaceLimit && LinearVelocity.x < 0))
			LinearVelocity.x *= -1;
		if ((FlagPos.y > SpaceLimit && LinearVelocity.y > 0) || (FlagPos.y < -SpaceLimit && LinearVelocity.y < 0))
			LinearVelocity.y *= -1;
		if ((FlagPos.z > SpaceLimit && LinearVelocity.z > 0) || (FlagPos.z < -SpaceLimit && LinearVelocity.z < 0))
			LinearVelocity.z *= -1;

		glPushMatrix();
		glTranslatef(FlagPos.x, FlagPos.y, FlagPos.z);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, n);
		glutSolidSphere(5.0, 8, 8);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(ReturnPos.x, ReturnPos.y, ReturnPos.z);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color);
		glutSolidSphere(5.0, 8, 8);
		glPopMatrix();
	}
}

void Team::Release()
{
	delete [] players;
}
