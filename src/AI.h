/*
 *  AI.h
 *  AI Algorithms Class
 *
 *  Created by Ned Bingham on 7/9/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef AI_h
#define AI_h

#include "standard.h"
#include "vector.h"
#include "graphics.h"

#define SpaceLimit 150

enum status
{
	attacking,
	defending,
	wondering,
	spasming
};

struct AI
{
	vec Position;
	vec LinearVelocity;
	GLfloat Radius;
	status state;

	int step;
	int check_t;
	vec TargetPos;
	void *team;

	void Attack(void *target);
	void Defend();

	void Setup(GLfloat r, void *t);
	void Update(void *target);
	void Render();
};

#endif
