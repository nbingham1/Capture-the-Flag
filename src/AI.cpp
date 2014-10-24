/*
 *  AI.cpp
 *  AI Algorithms Class
 *
 *  Created by Ned Bingham on 7/9/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "AI.h"
#include "Team.h"

void AI::Attack(void *target)
{
	if (((Team*)target) != ((Team*)team))
	{
		vec v = norm(((Team*)(target))->FlagPos - ((Team*)(team))->ReturnPos);

		if (step == 0)
			TargetPos = ((Team*)(target))->FlagPos + 20.0*vec(v.y, v.z, v.x);
		else if (step == 1)
			TargetPos = ((Team*)(target))->FlagPos + 20.0*v;
		else if (step == 2)
		{
			if (dist(Position, ((Team*)(target))->FlagPos) < 25.0)
				TargetPos = ((Team*)(team))->ReturnPos;
			else
				step = 0;
		}

		if (dist(((Team*)(team))->ReturnPos, ((Team*)(target))->FlagPos) < 10.0)
			((Team*)(target))->captured = true;

		if (dist(Position, TargetPos) < 5.0 && time(0) != check_t)
		{
			step++;
			if (step == 3)
				step = 0;
			check_t = time(0);
		}
	}
	else
		TargetPos = ((Team*)(team))->ReturnPos;
}

void AI::Defend()
{
	if (dist(((Team*)(team))->FlagPos, ((Team*)(team))->ReturnPos - vec(0.0, 10.0, 0.0)) > 10.0)
	{
		vec v = norm(((Team*)(team))->FlagPos - (((Team*)(team))->ReturnPos - vec(0.0, 10.0, 0.0)));

		if (step == 0)
			TargetPos = ((Team*)(team))->FlagPos + 20.0*vec(v.y, v.z, v.x);
		else if (step == 1)
			TargetPos = ((Team*)(team))->FlagPos + 20.0*v;
		else if (step == 2)
		{
			if (dist(Position, ((Team*)(team))->FlagPos) < 25.0)
				TargetPos = (((Team*)(team))->ReturnPos - vec(0.0, 10.0, 0.0));
			else
				step = 0;
		}

		if (dist(Position, TargetPos) < 5.0 && time(0) != check_t)
		{
			step++;
			if (step == 3)
				step = 0;
			check_t = time(0);
		}
	}
	else
		TargetPos = ((Team*)(team))->ReturnPos;
}

void AI::Setup(GLfloat r, void *t)
{
	Radius = r;
	team = t;
	step = 0;
	if (rand()%2 == 1)
		state = defending;
	else
		state = attacking;
}

void AI::Update(void *target)
{
	if (state == attacking)
		Attack(target);
	else
		Defend();

	LinearVelocity *= 0.9;
	if (state == attacking)
		LinearVelocity += .1*norm(TargetPos - Position);
	else
		LinearVelocity += .11*norm(TargetPos - Position);

	/*if ((Position.x > SpaceLimit && LinearVelocity.x > 0) || (Position.x < -SpaceLimit && LinearVelocity.x < 0))
		LinearVelocity.x *= -1;
	if ((Position.y > SpaceLimit && LinearVelocity.y > 0) || (Position.y < -SpaceLimit && LinearVelocity.y < 0))
		LinearVelocity.y *= -1;
	if ((Position.z > SpaceLimit && LinearVelocity.z > 0) || (Position.z < -SpaceLimit && LinearVelocity.z < 0))
		LinearVelocity.z *= -1;*/

	Position += LinearVelocity;
}

void AI::Render()
{
	glPushMatrix();
	glTranslatef(Position.x, Position.y, Position.z);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ((Team*)(team))->color);
	glutSolidSphere(Radius, 5, 5);
	glPopMatrix();
}
